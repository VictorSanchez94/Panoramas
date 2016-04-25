#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "Panorama.h"

using namespace cv;
using namespace std;

void bruteForceMatch() {

  Mat img_1 = imread( "./Data/001p.jpg", CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( "./Data/002p.jpg", CV_LOAD_IMAGE_GRAYSCALE );

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Step 2: Calculate descriptors (feature vectors)
  SurfDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  //-- Step 3: Matching descriptor vectors with a brute force matcher
  BFMatcher matcher(NORM_L2, true);		//Mejor resultado con validacion cruzada
  //std::vector<vector<DMatch> > matches;
  vector<DMatch> matches;
  //matcher.knnMatch( descriptors_1, descriptors_2, matches, 2);
  matcher.match( descriptors_1, descriptors_2, matches );

  //-- Draw matches
  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches );

  vector<Point2f> obj, scene;

  for (int i=0; i < matches.size(); i++) {
    //-- Get the keypoints from the good matches
    obj.push_back( keypoints_1[ matches[i].queryIdx ].pt );
    scene.push_back( keypoints_2[ matches[i].trainIdx ].pt );
  }
  Mat H = findHomography( obj, scene, CV_RANSAC );



  //-- Show detected matches
  imshow("Matches", img_matches );

  panorama(img_1, img_2, H, obj, scene, img_matches);

  waitKey(0);

  }
