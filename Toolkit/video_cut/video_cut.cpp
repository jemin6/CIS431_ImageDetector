
<<<<<<< HEAD
// g++ -o video_cut video_cut.cpp  `pkg-config --cflags --libs opencv`
// ./video_cut
=======
// g++ -o video_cut video_cut.cpp  `pkg-config --cflags --libs opencv` -std=c++11
// ./video_cut <file_name>
>>>>>>> jim

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

<<<<<<< HEAD
=======
typedef std::chrono::high_resolution_clock Clock;

>>>>>>> jim
int main(int argc, char* argv[]){

  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name

<<<<<<< HEAD
=======
  auto t1 = Clock::now(); 

>>>>>>> jim
  if (argc <= 1){

    cout << "Usage: ./video_cut <file_name>" << endl;
    return -1;
    }

  string file_name = argv[1];

  VideoCapture cap(file_name); 
   
  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  char name [50];
  while(1){

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty())
      break;
    else
      sprintf(name, "frames/frame_%d.jpg", (int)cap.get(CV_CAP_PROP_POS_FRAMES));

    imwrite(name, frame);

  }
 
  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  destroyAllWindows();
<<<<<<< HEAD
=======

  auto t2 = Clock::now();

  double timer = chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
  cout << "Runtime: "
  << timer
    << " seconds" << std::endl;
>>>>>>> jim
	
  return 0;
}
