#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

VideoCapture TheVideoCapturer;
Mat bgrMap;

int captureImages (int camera) {
  char key = 0;

  int numSnapshot = 0;
  std::string snapshotFilename = "0";

  cout << "Press 's' to take snapshots" << std::endl;
  cout << "Press 'Esc' to finish" << std::endl;

  TheVideoCapturer.open(camera);	//0 -> Camara portatil --- 1 -> WebCam externa

  /**
   * CON LAS CAMARAS PROBADAS NO DEJA CAMBIAR NINGUN PARAMETRO
   */
  /*TheVideoCapturer.set(CV_CAP_PROP_FPS, 25);
  std::cout <<  TheVideoCapturer.get(CV_CAP_PROP_EXPOSURE) << std::endl;
  bool b = TheVideoCapturer.set(CV_CAP_PROP_EXPOSURE, 10);
  std::cout <<  TheVideoCapturer.get(CV_CAP_PROP_EXPOSURE) << " bool: " << b << " " << std::endl;*/


  if (!TheVideoCapturer.isOpened()) {
    cerr << "Could not open video" << std::endl;
    return -1;
  }

  while (key!=27 && TheVideoCapturer.grab()) {
    TheVideoCapturer.retrieve(bgrMap);
    cvtColor(bgrMap, bgrMap, CV_BGR2GRAY);

    imshow("BGR image", bgrMap);

    if (key == 115) {
      imwrite("Data/" + snapshotFilename + ".pgm", bgrMap);
      numSnapshot++;
      snapshotFilename = static_cast<std::ostringstream*>(&(std::ostringstream() << numSnapshot))->str();
      cout << "Captura guardada.";
        }

    key = waitKey(20);
  }
  cvDestroyWindow("BGR image"); //Destroy Window

  return 0;

  /*for(int photo=0; photo < numSnapshot; photo=photo+1){
	Mat image;
	stringstream s;
	s << photo << ".png";
	//std::cout << s;
	image = imread(s.str(), 1 );
	namedWindow( "Display Image", WINDOW_AUTOSIZE );
	imshow( "Display Image", image );
	waitKey(0);
  }*/
}
