#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "Panorama.h"
#include "CustomRANSAC.h"

using namespace cv;
using namespace std;

Mat bruteForceMatch(Mat img_1, Mat img_2, int H_type) {

  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  SurfDescriptorExtractor extractor;

  Mat descriptors_1, descriptors_2;

  extractor.compute( img_1, keypoints_1, descriptors_1 );
  extractor.compute( img_2, keypoints_2, descriptors_2 );

  BFMatcher matcher(NORM_L2, true);		//Mejor resultado con validacion cruzada
  vector<DMatch> matches;
  matcher.match( descriptors_1, descriptors_2, matches );

  Mat img_matches;
  drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches );

  vector<Point2f> obj, scene;

  for (int i=0; i < matches.size(); i++) {
    obj.push_back( keypoints_1[ matches[i].queryIdx ].pt );
    scene.push_back( keypoints_2[ matches[i].trainIdx ].pt );
  }

  Mat nuevaImagen;
  if (H_type == 0){
	  Mat H = findHomography( obj, scene, CV_RANSAC );
	  imshow("Matches", img_matches );
	  nuevaImagen = panorama(img_1, img_2, H);
  }else if(H_type == 1) {
	  Mat H(3,3,CV_64FC1);
	  vector<Point2f> key1, key2;
	  FeatureExtraction(img_1, img_2, key1, key2);
	  RANSAC_DLT(key1, key2, H);
	  imshow("Matches", img_matches );
	  nuevaImagen = panorama(img_1, img_2, H);
  }else{
	  cout << "Aprende a meter los parametros, parguela" << endl;
  }

  return nuevaImagen;
  }
