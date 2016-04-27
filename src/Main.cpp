#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "BruteForceMatch.h"
#include "CaptureImages.h"
#include "Panorama.h"
#include <sstream>

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
	cout << argc << ": "<< argv[1] << ", " << argv[2] << endl;
	if (argc == 3){
		if(strcmp(argv[1], "-c") == 0){
			int numShots = captureImages(0);
			Mat img_1 = imread( "./Data/Captures/1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
			resize(img_1, img_1, Size(0,0), 0.8, 0.8);

			for (int i=1; i<numShots; i++){
				stringstream rutaImagen1, rutaImagen2;
				rutaImagen1 << "./Data/Captures/" << i << ".jpg";
				rutaImagen2 << "./Data/Captures/" << i+1 << ".jpg";
				Mat img_2 = imread( rutaImagen2.str(), CV_LOAD_IMAGE_GRAYSCALE );
				resize(img_2, img_2, Size(0,0), 0.8, 0.8);
				img_1 = bruteForceMatch(img_1, img_2);
			}
		}else if(strcmp(argv[1], "-p") == 0) {
			if(strcmp(argv[2], "-l")) {
				Mat img_1 = imread( "./Data/Paisaje/1.jpg", CV_LOAD_IMAGE_GRAYSCALE );
				resize(img_1, img_1, Size(0,0), 0.8, 0.8);

				for (int i=1; i<9; i++){
					stringstream rutaImagen1, rutaImagen2;
					rutaImagen1 << "./Data/Paisaje/" << i << ".jpg";
					rutaImagen2 << "./Data/Paisaje/" << i+1 << ".jpg";
					Mat img_2 = imread( rutaImagen2.str(), CV_LOAD_IMAGE_GRAYSCALE );
					resize(img_2, img_2, Size(0,0), 0.8, 0.8);
					img_1 = bruteForceMatch(img_1, img_2);
				}
			}else if(strcmp(argv[2], "-r")){
				Mat img_1 = imread( "./Data/Paisaje/9.jpg", CV_LOAD_IMAGE_GRAYSCALE );
				resize(img_1, img_1, Size(0,0), 0.8, 0.8);

				for (int i=9; i>1; i--){
					stringstream rutaImagen1, rutaImagen2;
					rutaImagen1 << "./Data/Paisaje/" << i << ".jpg";
					rutaImagen2 << "./Data/Paisaje/" << i-1 << ".jpg";

					Mat img_2 = imread( rutaImagen2.str(), CV_LOAD_IMAGE_GRAYSCALE );
					resize(img_2, img_2, Size(0,0), 0.8, 0.8);
					img_1 = bruteForceMatch(img_1, img_2);
				}
			}else{
				cout << "Argumentos inválidos: -p|c -l|r" << endl;
			}
		}else{
			cout << "Argumentos inválidos: -p|c -l|r" << endl;
		}
	}else{
		cout << "Número de argumentos inválido: -p|c -l|r" << endl;
	}
}
