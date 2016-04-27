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

/*
 * Calcula las coordenadas resultantes de un punto
 * al aplicarle una Homografia
 */
pair<double, double> calculateCoor(Mat H, int x, int y) {
	pair<double, double> p;
	double xC = H.at<double>(0, 0) * x + H.at<double>(0, 1) * y + H.at<double>(0, 2);
	double yC = H.at<double>(1, 0) * x + H.at<double>(1, 1) * y + H.at<double>(1, 2);
	double zC = H.at<double>(2, 0) * x + H.at<double>(2, 1) * y + H.at<double>(2, 2);
	p.first = round(xC / zC);
	p.second = round(yC / zC);
	return p;
}

/*
 * Calcula el offset necesario para que la imagen no tenga pixeles
 * en coordenadas negativas y si alguno de los valores sea atipico
 * devuelve este
 */
pair<int, int> calculateOffset(Mat H, Mat img) {

	pair<double, double> A = calculateCoor(H, 0, 0);
	pair<double, double> B = calculateCoor(H, 0, img.rows);
	pair<double, double> C = calculateCoor(H, img.cols, 0);
	pair<double, double> D = calculateCoor(H, img.cols, img.rows);

	double minX = min(min(A.first, B.first), min(C.first, D.first));
	double minY = min(min(A.second, B.second), min(C.second, D.second));

	pair<double, double> p;
	p.first = minX < 0 ? abs(minX) : 0;
	p.second = minY < 0 ? abs(minY) : 0;

	return p;
}

/*
 * Aplica un offset a una homografia
 */
void applyOffset(Mat m, double x, double y) {
	Mat f = Mat::eye(3, 3, m.type());
	f.at<double>(0, 2) = x;
	f.at<double>(1, 2) = y;
	Mat R = f*m;
	R.copyTo(m);
	R.release();
	f.release();
}

void crop(Mat &in, Mat img2, pair<double, double> off) {
	Scalar sumaX = 0;
	int ancho = 0, inicioAncho = -1;
	for (int j = 5; j < in.cols; j++) {
		sumaX = sum(in.col(j));
		if(sumaX.val[0] > 0){
			if(inicioAncho==-1){
				inicioAncho=j;
			}
		}else{
			ancho = j;
			if(inicioAncho!=-1){ //Cochinada
				break;
			}

		}
	}

	Scalar sumaY = 0;
	int alto = 0, inicioAlto = -1;
	for (int j = 5; j < in.rows; j++) {
		sumaY = sum(in.row(j));
		if(sumaY.val[0] > 0){
			if(inicioAlto==-1){
				inicioAlto=j;
			}
		}else{
			alto = j;
			if(inicioAncho!=-1){ //Cochinada
				break;
			}
		}
	}
	in = in.operator ()(Range(inicioAlto, alto), Range(inicioAncho, ancho));
}

Mat panorama(Mat img1, Mat img2, Mat H) {
	//En primer lugar se calcula y se aplica el offset a la homografia para no perder informacion
	pair<double, double> off = calculateOffset(H, img1);
	applyOffset(H, off.first, off.second);
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0);
	obj_corners[1] = cvPoint(img1.cols, 0);
	obj_corners[2] = cvPoint(img1.cols, img1.rows);
	obj_corners[3] = cvPoint(0, img1.rows);
	std::vector<Point2f> scene_corners(4);
	pair<int, int> ancho_img1_t = calculateCoor(H,0, img1.cols);
	pair<int, int> alto_img1_t = calculateCoor(H,img1.rows, img1.rows);

	Mat nueva1(img1.rows+img2.rows, img1.cols+img2.cols, img1.type(), Scalar(0, 0, 0));
	warpPerspective(img1, nueva1, H, nueva1.size(), INTER_LINEAR, BORDER_TRANSPARENT);

	for (int i=0; i<img2.rows; i++){
		for (int j=0; j<img2.cols; j++){
			int p = img2.at<uchar>(i,j);
			nueva1.at<uchar>(i+off.second,j+off.first) = p;
		}
	}

	crop(nueva1, img2, off);
	imshow("Panorama", nueva1);

	waitKey(0);
	return nueva1;

}
