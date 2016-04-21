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

void panorama(Mat img1, Mat img2, Mat H, vector<Point2f> obj, vector<Point2f> scene) {

	//Mat img_transformed, img2_color;
	Mat img_transformed = Mat::zeros(img2.rows, img2.cols, CV_32F);
	//cvtColor(img2, img2_color, CV_GRAY2BGR);
	//Size s(img1.rows, img1.cols);
	//warpPerspective(img2, img_transformed, H, s);
	//perspectiveTransform(img2, img_transformed, H);
	//imshow("IMG1", scene);
	//imshow("IMG2", img2);
	//imshow("Transformada", img_transformed);
	cout << H.type();
	for (int i=0; i<img2.rows; i++){
		for (int j=0; j<img2.cols; j++){
			uchar p = img2.at<uchar>(i,j);
			//Calcula nuevas coordenadas del pixel
			float d[3] = { i*1.0,j*1.0,1 };
			Mat pos = cv::Mat(3,1, CV_64F, d);
			Mat posTransform = H*pos;
			int iTransform = posTransform.at<float>(0,0);
			int jTransform = posTransform.at<float>(1,0);
			//int iTransform = posTransform[0];
			//int jTransform = posTransform[1];
			img_transformed.at<float>(iTransform,jTransform) = p;
		}
	}
	imshow("Transformada", img_transformed);


	waitKey(0);

}
