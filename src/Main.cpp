#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "BruteForceMatch.h"
#include "CaptureImages.h"
#include "Panorama.h"

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
	//int i = captureImages(0);

	Mat img_1 = imread( "./Data/001p.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_2 = imread( "./Data/002p.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_3 = imread( "./Data/003p.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_4 = imread( "./Data/004p.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_5 = imread( "./Data/005p.jpg", CV_LOAD_IMAGE_GRAYSCALE );

	Mat nueva = bruteForceMatch(img_1, img_2);


}
