#ifndef __PANORAMA_H__
#define __PANORAMA_H__

/*#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"*/

using namespace cv;
using namespace std;

void panorama (Mat img1, Mat img2, Mat H, vector<Point2f> obj, vector<Point2f> scene);

#endif
