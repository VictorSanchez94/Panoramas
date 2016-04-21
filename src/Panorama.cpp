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

	Mat img_transformed, img2_color;
	//cvtColor(img2, img2_color, CV_GRAY2BGR);

	Size s(img1.rows, img1.cols);
	//warpPerspective(img2, img_transformed, H, s);

	perspectiveTransform(img2, img_transformed, H);
	//imshow("IMG1", scene);
	imshow("IMG2", img2);

	//imshow("Transformada", img_transformed);

	waitKey(0);

}
