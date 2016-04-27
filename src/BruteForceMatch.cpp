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

Mat bruteForceMatch(Mat img_1, Mat img_2, int H_type, char* tipo_r) {
	try{
		//Inicializaciones necesarias
		int minHessian = 400;
		std::vector<KeyPoint> keypoints_1, keypoints_2;
		Mat descriptors_1, descriptors_2;
		vector<DMatch> matches;
		Mat img_matches;
		vector<Point2f> obj, scene;

		//Se selecciona el tipo de reconocimiento que se desea
		if(strcmp(tipo_r, "sift")==0){
			SiftFeatureDetector detector( minHessian );
			SiftDescriptorExtractor extractor;
			detector.detect( img_1, keypoints_1 );
			detector.detect( img_2, keypoints_2 );
			extractor.compute( img_1, keypoints_1, descriptors_1 );
			extractor.compute( img_2, keypoints_2, descriptors_2 );
		}else if(strcmp(tipo_r, "surf")==0){
			SurfFeatureDetector detector( minHessian );
			SurfDescriptorExtractor extractor;
			detector.detect( img_1, keypoints_1 );
			detector.detect( img_2, keypoints_2 );
			extractor.compute( img_1, keypoints_1, descriptors_1 );
			extractor.compute( img_2, keypoints_2, descriptors_2 );
		}else if(strcmp(tipo_r, "orb")==0){
			OrbFeatureDetector detector( minHessian );
			OrbDescriptorExtractor extractor;
			detector.detect( img_1, keypoints_1 );
			detector.detect( img_2, keypoints_2 );
			extractor.compute( img_1, keypoints_1, descriptors_1 );
			extractor.compute( img_2, keypoints_2, descriptors_2 );
		}

		//Una vez que se han calculado los puntos de interes se emparejan con fuerza bruta
		BFMatcher matcher(NORM_L2, true);		//Mejor resultado con validacion cruzada que con ratio al segundo vecino
		matcher.match( descriptors_1, descriptors_2, matches );

		drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches );

		for (int i=0; i < matches.size(); i++) {
			obj.push_back( keypoints_1[ matches[i].queryIdx ].pt );
			scene.push_back( keypoints_2[ matches[i].trainIdx ].pt );
		}

		//Se cacula la homografia resultante de los puntos calculados
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

	}catch(...){
		cout << "Error al calcular puntos de interes/homografia." << endl;
		exit(0);
	}


}
