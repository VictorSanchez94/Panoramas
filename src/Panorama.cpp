#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;

void panorama(Mat img1, Mat img2, Mat H, vector<Point2f> obj, vector<Point2f> scene, Mat img_matches) {
	/*cout << H;
	Mat img2_transformed;
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(img1.cols, 0);
	obj_corners[2] = cvPoint(img1.cols, img1.rows);
	obj_corners[3] = cvPoint(0, img1.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);
	Mat img_matchesc;
	line(img_matches, scene_corners[0] + Point2f(img1.cols, 0), scene_corners[1] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img1.cols, 0), scene_corners[2] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches,scene_corners[2] + Point2f(img1.cols, 0), scene_corners[3] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img1.cols, 0), scene_corners[0] + Point2f(img1.cols, 0), Scalar(0, 255, 0), 4);


	imshow("C", img_matches);*/


	Mat nueva1(img1.rows+img2.rows, img1.cols+img2.cols, img1.type(), Scalar(0, 0, 0));
	//Mat nueva2(img2.rows, img2.cols, img2.type(), Scalar(0, 0, 0));
	  //warpPerspective(original, nueva, H, nueva.size(), INTER_CUBIC);
	  //warpPerspective(img1, nueva1, H, nueva1.size());
	  warpPerspective(img1, nueva1, H, nueva1.size());

	  for (int i=0; i<img2.rows; i++){
	  		for (int j=0; j<img2.cols; j++){
	  			int p = img2.at<uchar>(i,j);
	  			if(nueva1.at<uchar>(i,j) == 0){
	  				//cout << "Aqui" << endl;
	  				nueva1.at<uchar>(i,j) = p;
	  			}
	  			//

	  		}
	  }

	  imshow("img2", img2);
	  imshow("nueva1", nueva1);

	//Mat img_transformed, img2_color;
	//Mat img_transformed2, img_transformed1 /* = Mat::zeros(img2.rows, img2.cols, CV_8UC1)*/;
	//warpPerspective(img2, img_transformed2, H, img_transformed2.size());
	//warpPerspective(img1, img_transformed1, H, img_transformed1.size());
	//cout << img_transformed.type();
	/*for (int i=0; i<img2.rows; i++){
		for (int j=0; j<img2.cols; j++){
			int p = img2.at<uchar>(i,j);
			//Calcula nuevas coordenadas del pixel
			double d[3] = { i,j,1 };
			Mat pos = Mat(3,1, CV_64F, &d);
			Mat posTransform = H*pos;
			//multiply(H, pos, posTransform);
			//cout << posTransform << endl;

			int iTransform, jTransform;
			iTransform = (int)posTransform.at<double>(0,0);
			jTransform = (int)posTransform.at<double>(0,1);

			//cout << iTransform << " " << jTransform << "      " << p << endl << endl;
			if(iTransform>0 && iTransform<img_transformed.rows && jTransform>0 && jTransform<img_transformed.cols){
				//img_transformed.at<uchar>(iTransform,jTransform) = 200;
			}
			img_transformed.at<uchar>(i,j) = 200;
		}
	}*/


	waitKey(0);

}
