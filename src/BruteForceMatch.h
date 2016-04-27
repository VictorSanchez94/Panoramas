#ifndef __BRUTEFORCEMATCH_H__
#define __BRUTEFORCEMATCH_H__

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat bruteForceMatch (Mat img_1, Mat img_2, char* tipo_r);

#endif
