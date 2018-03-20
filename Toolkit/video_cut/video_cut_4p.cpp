
// mpic++ -std=c++11 -o video_cut_4p video_cut_4p.cpp  `pkg-config --cflags --libs opencv` -std=c++11
// mpirun --hostfile hostfile -np 4 video_cut_4p <file_name>

#include "opencv2/opencv.hpp"
#include <iostream>
#include "mpi.h"
#define  MASTER   0

using namespace std;
using namespace cv;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[]){

  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name

  int numtasks, taskid;

  MPI_Init(&argc, &argv);

  // MPI stuff
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

  if (numtasks != 4) {
    printf("Quitting. Number of MPI tasks must be 4.\n");
    MPI_Abort(MPI_COMM_WORLD, 0);
    exit(0);
  }


  auto t1 = Clock::now(); 

  if (argc <= 1){

    cout << "Usage: ./video_cut <file_name>" << endl;
    return -1;
    }

  string file_name = argv[1];

  VideoCapture cap(file_name); 
  int chunk = cap.get(CV_CAP_PROP_FRAME_COUNT) / numtasks;
  cap.set(CV_CAP_PROP_POS_FRAMES, chunk*taskid);

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  char name [50];

  int count = 0;
   
  while(1){

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
 
    // If the frame is empty, break immediately
    if (frame.empty() || (count >= chunk))
      break;
    else
      sprintf(name, "frames/frame_%d.jpg", (int)cap.get(CV_CAP_PROP_POS_FRAMES));

    imwrite(name, frame);

    count++;

  }
 
  // When everything done, release the video capture object
  cap.release();

  // Closes all the frames
  //destroyAllWindows();

  auto t2 = Clock::now();

  if (taskid == MASTER){

      double timer = chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
      cout << ";  Runtime: "
        << timer
        << " seconds" << std::endl;

  }

  MPI_Finalize();
	
  return 0;
}
