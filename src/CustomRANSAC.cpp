#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<ctime>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/nonfree/features2d.hpp>
#include<opencv2/legacy/legacy.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/nonfree/nonfree.hpp>

using namespace std;
using namespace cv;
#define T_DIST 2

//Normalize Feature Points Coordinates before DLT Implementation
vector<Point2f> Normalize(const vector<Point2f> & Key, Mat &T)
{
    vector<Point2f> KeyNorm( Key );
    int Num = Key.size();
     if(Num > 2000)
         Num = 2000;
    int i = 0;
    double scale, tx, ty;//scale, translation x, y
    double xm,ym; //The mean value along x,y coordinate
    for(i = 0; i < Num; i++){
        xm += Key[i].x;
        ym += Key[i].y;
    }
    xm = static_cast<double>(xm / Num);
    ym = static_cast<double>(ym / Num);

    double range = 0.0;
    for(i = 0; i < Num; i++){
        range += std::sqrt(std::pow(Key[i].x - xm, 2.0) + std::pow(Key[i].y - ym, 2.0));
    }
    range = static_cast<double>(range / Num);
    scale = std::sqrt(2.0) / range;
    tx = -scale * xm;
    ty = -scale * ym;

    T = Mat::zeros(3,3,CV_64FC1);
    T.at<double>(0,0) = scale;
    T.at<double>(0,2) = tx;
    T.at<double>(1,1) = scale;
    T.at<double>(1,2) = ty;
    T.at<double>(2,2) = 1.0;
    //Note the operation is under inhomography coordinates, ommit the third
    //parameter which equals 1
    Mat X(3,1,CV_64FC1);
    Mat Y(3,1,CV_64FC1);
    X = Mat::zeros(3,1,CV_64FC1);
    Y = Mat::zeros(3,1,CV_64FC1);
    for(i = 0; i < Num; i++){
        X.at<double>(0,0) = Key[i].x;
        X.at<double>(1,0) = Key[i].y;
        X.at<double>(2,0) = 1;
        Y = T*X;
        KeyNorm[i].x = Y.at<double>(0,0) / Y.at<double>(2,0);
        KeyNorm[i].y = Y.at<double>(1,0) / Y.at<double>(2,0);
    }
    //cout<<"Normalize OK"<<endl;
    return KeyNorm;
}

//Feature Corresponding Points Calculated
void FeatureExtraction(const Mat& img1, const Mat& img2, vector<Point2f> & Key1, vector<Point2f> & Key2)
{
    Mat M1(img1);
    Mat M2(img2);
    vector<KeyPoint>  Keypoint1;
    vector<KeyPoint>  Keypoint2;
    SurfFeatureDetector detector(400);
    //detecting keypoints
    detector.detect(img1,Keypoint1);
    detector.detect(img2,Keypoint2);
    //computing descriptors
    SurfDescriptorExtractor extractor;
    Mat descriptor1, descriptor2;
    extractor.compute(img1, Keypoint1, descriptor1);
    extractor.compute(img2, Keypoint2, descriptor2);

    //matching descriptors
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptor1, descriptor2, matches);
    double max_dist = 0;
    double min_dist = 100;
    for (int i = 0; i < descriptor1.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }
    //cout<<"The Max dist and Min Dist are : "<< max_dist<<" "<<min_dist<<endl;
    for(int i = 0; i < descriptor1.rows; i++)
    {
        if(matches[i].distance < 3*min_dist && Key1.size() < 2000)
        {
            Key1.push_back(Keypoint1[ matches[i].queryIdx ].pt);
            Key2.push_back(Keypoint2[ matches[i].trainIdx ].pt);
        }
    }

    M1.release();
    M2.release();
    descriptor1.release();
    descriptor2.release();
    //cout<<"Feature Extraction OK"<<endl;
}

//Compute the Homography Matrix through DLT on 4 corresponding pairs of keypoints
void DLT(Mat& H, const vector<Point2f> & KeyNorm1, const vector<Point2f> & KeyNorm2, const vector<int> &orderin)
{
    int rowN = 2*orderin.size();
//    cout<<"orderin size is:"<<orderin[0]<<" "<<orderin[1]<<" "<<orderin[2]<< " "<< orderin[3]<<endl;
    Mat A= Mat::zeros(rowN,9,CV_64FC1);
    Mat U(rowN,rowN,CV_64FC1);
    Mat D(rowN,9,CV_64FC1);
    Mat VT(9,9,CV_64FC1);
    //Mat V(9,9,CV_64FC1);

    for(int i = 0; i < orderin.size(); i++){
        A.at<double>(2*i, 3) = -KeyNorm1[ orderin[i] ].x;
        A.at<double>(2*i, 4) = -KeyNorm1[ orderin[i] ].y;
        A.at<double>(2*i, 5) = -1;
        A.at<double>(2*i, 6) = KeyNorm2[ orderin[i] ].y * KeyNorm1[ orderin[i] ].x;
        A.at<double>(2*i, 7) = KeyNorm2[ orderin[i] ].y * KeyNorm1[ orderin[i] ].y;
        A.at<double>(2*i, 8) = KeyNorm2[ orderin[i] ].y;

        A.at<double>(2*i+1, 0) =  KeyNorm1[ orderin[i] ].x;
        A.at<double>(2*i+1, 1) =  KeyNorm1[ orderin[i] ].y;
        A.at<double>(2*i+1, 2) = 1;
        A.at<double>(2*i+1, 6) = -KeyNorm2[ orderin[i] ].x * KeyNorm1[ orderin[i] ].x;
        A.at<double>(2*i+1, 7) = -KeyNorm2[ orderin[i] ].x * KeyNorm1[ orderin[i] ].y;
        A.at<double>(2*i+1, 8) = -KeyNorm2[ orderin[i] ].x;
        //cout<<"keynorm at 2i, 8 : "<<KeyNorm2[ orderin[i] ].x<<endl;;

    }
    SVD::compute(A, D, U, VT,SVD::FULL_UV);//note the V is the V^T
    //cout<<"SVD OK"<<endl;
    //The homography matrix from the last column of V
    for(int i = 0; i < 9; i++){
        H.at<double>(i/3, i%3) = VT.at<double>(8,i);
    }

    A.release();
    U.release();
    D.release();
    VT.release();
    //cout<<"DLT OK"<<endl;
}

int InliersCount(const Mat& H, const vector<Point2f> & KeyNorm1, const vector<Point2f> & KeyNorm2, vector<int> & inliers, double & dist_std)
{
    double Current_Dist, Sum_Dist, Mean_Dist;
    int numInlier = 0;
    int num = 0;
    int i;
    if(KeyNorm1.size() == KeyNorm2.size()){
         num = KeyNorm1.size();
    }

    Mat x(3,1,CV_64FC1);
    Mat xp(3,1,CV_64FC1);
    Mat Mid(3,1,CV_64FC1);
    Mat Dist(num,1,CV_64FC1);
    Point2f  Temp;
    Mat invH(3,3,CV_64FC1);
    cv::invert(H, invH);
    for(i = 0; i < num; i++){
        Current_Dist = 0.0;
       x.at<double>(0,0) = KeyNorm1[i].x;
       x.at<double>(1,0) = KeyNorm1[i].y;
       x.at<double>(2,0) = 1;

       xp.at<double>(0,0) = KeyNorm2[i].x;
       xp.at<double>(1,0) = KeyNorm2[i].y;
       xp.at<double>(2,0) = 1;

      Mid = H*x;
      Temp.x = static_cast<int>(Mid.at<double>(0,0) / Mid.at<double>(2,0));
      Temp.y = static_cast<int>(Mid.at<double>(1,0) / Mid.at<double>(2,0));
      Current_Dist += std::pow(Temp.x - KeyNorm2[i].x, 2.0) + std::pow(Temp.y - KeyNorm2[i].y, 2.0);
      Mid = invH*xp;
      Temp.x = static_cast<int>(Mid.at<double>(0,0) / Mid.at<double>(2,0));
      Temp.y = static_cast<int>(Mid.at<double>(1,0) / Mid.at<double>(2,0));

      Current_Dist += std::pow(Temp.x - KeyNorm1[i].x, 2.0) + std::pow(Temp.y - KeyNorm1[i].y, 2.0);
      //cout<<"The current dist is: "<<Current_Dist<<endl;
     if(Current_Dist < T_DIST){
         numInlier++;
         inliers.push_back(i);
         Dist.at<double>(i,0) = Current_Dist;
         Sum_Dist += Current_Dist;
     }
    }

    dist_std = 0;
    Mean_Dist = Sum_Dist / (double) numInlier;
    int j =0;
    for(i = 0; i < inliers.size(); i++){
        j = inliers[i];
        dist_std += std::pow(Dist.at<double>(j,0) - Mean_Dist, 2.0);
    }
    dist_std /= static_cast<double>(numInlier - 1);
    x.release();
    xp.release();
    Mid.release();
    Dist.release();
    invH.release();

    //cout<<"Inlier Count OK, inlier number is: "<<numInlier<<endl;
    return numInlier;
}

bool isColinear(const vector<int> & orderin, const vector<Point2f> & Key1)
{
    bool iscolinear;
    int num = orderin.size();
    double value;
    Mat p1(3,1,CV_64FC1);
    Mat p2(3,1,CV_64FC1);
    Mat p3(3,1,CV_64FC1);
    Mat lin(3,1,CV_64FC1);
    iscolinear = false;

    for( int i = 0; i < num-2; i++){
        p1.at<double>(0,0) = Key1[orderin[i]].x;
        p1.at<double>(1,0) = Key1[orderin[i]].y;
        p1.at<double>(2,0) = 1;
        for( int j = i+1; j < num -1; j++){
           p2.at<double>(0,0) = Key1[orderin[j]].x;
           p2.at<double>(1,0) = Key1[orderin[j]].y;
           p2.at<double>(2,0) = 1;
           lin = p1.cross(p2);
           for(int k = j+1; k < num; k++){
               p3.at<double>(0,0) = Key1[orderin[k]].x;
               p3.at<double>(1,0) = Key1[orderin[k]].y;
               p3.at<double>(2,0) = 1;
               value = p3.dot(lin);
               if(std::abs(value) < 10e-2) {
                   iscolinear = true;
                   break;
               }
           }
           if(iscolinear == true){
        	   break;
           }
        }
    }
    p1.release();
    p2.release();
    p3.release();
    lin.release();
    return iscolinear;
}

void customRANSAC(const vector<Point2f> & Key1, const vector<Point2f> & Key2, Mat & H_final)
{
    int numAll = 0;
    if(Key1.size() == Key2.size()){
    	numAll = Key1.size();
    }
    //cout<<"The number of keypoints: "<<numAll<<endl;
    int N = 500, nu = 4;// The whole sample points and the 4 points to estimate
    int Max_num = 4; //Max number of inliers
    int sample = 0;
    int numInlier;
    double e, p = 0.99;
    double current_dist_std,dist_std;
    vector<int> orderin(nu,0);
    vector<int> inliers;
    vector<int> max_inliers;
    Mat T1(3,3,CV_64FC1);
    Mat T2(3,3,CV_64FC1);
    Mat InvT2(3,3,CV_64FC1);
    Mat H(3,3,CV_64FC1);
    srand(unsigned(time(0)));
    bool iscolinear = true;
    vector<Point2f> KeyNorm1 =  Normalize(Key1, T1);
    vector<Point2f> KeyNorm2 =  Normalize(Key2, T2);

    while(N > sample){
        iscolinear = true;
        while(iscolinear == true){
          iscolinear = false;
          for(int i = 0; i < nu; i++){
            orderin[i] = rand() % numAll;
          }
          if(iscolinear == false){
        	  iscolinear = isColinear(orderin, Key1);
          }
        }

        DLT(H,KeyNorm1,KeyNorm2, orderin);

        numInlier = InliersCount(H,KeyNorm1,KeyNorm2, inliers, dist_std);

        if(numInlier > Max_num || (numInlier == Max_num &&  current_dist_std > dist_std)) {
            Max_num = numInlier;
            max_inliers.clear();
            max_inliers.resize(0);
            max_inliers.swap(inliers);
            H_final = H;
            current_dist_std = dist_std;

        }
        //update the parameters for N
        e = 1 - (double)numInlier / (double) numAll;
        N = static_cast<int>( std::log(1-p) / std::log(1-std::pow(1-e,nu)));
		inliers.clear();
		inliers.resize(0);

        sample++;
    }
    cv::invert(T2,InvT2);

    DLT(H_final,KeyNorm1,  KeyNorm2, max_inliers);

    H_final = InvT2 * H_final;
    H_final = H_final * T1;
    //cout << "Ransac OK" << endl;
}
