#ifndef __CAPTUREIMAGES_H__
#define __CAPTUREIMAGES_H__

using namespace cv;
using namespace std;

vector<Point2f> Normalize(const vector<Point2f> & Key, Mat &T);

void FeatureExtraction(const Mat& img1, const Mat& img2, vector<Point2f> & Key1, vector<Point2f> & Key2);

void DLT(Mat& H, const vector<Point2f> & KeyNorm1, const vector<Point2f> & KeyNorm2, const vector<int> &orderin);

int InliersCount(const Mat& H, const vector<Point2f> & KeyNorm1, const vector<Point2f> & KeyNorm2, vector<int> & inliers, double & dist_std);

bool isColinear(const vector<int> & orderin, const vector<Point2f> & Key1);

void  LinearBlendPrepare( Mat& img1,  Mat& img2);

void customRANSAC(const vector<Point2f> & Key1, const vector<Point2f> & Key2, Mat & H_final);

void FeatureExtraction(const Mat& img1, const Mat& img2, vector<Point2f> & Key1, vector<Point2f> & Key2);

#endif
