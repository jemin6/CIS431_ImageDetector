/*
 *
 * mpic++ -std=c++11 -o object_detector_no_play_4p object_detector_no_play_4p.cpp  `pkg-config --cflags --libs opencv`
 * mpirun --hostfile hostfile -np 4 object_detector_no_play_4p <model_name> <video_name>
 *
 * credit to the lesson on https://abhishek4273.com/2014/03/16/traincascade-and-car-detection-using-opencv/
 
 *  Created on: Mar 6, 2018
 *      Author: yanhualuo
 */

#include <highgui.h>

#include <iostream>
#include <stdio.h>
#include <cv.hpp>
#include "mpi.h"
#define  MASTER   0

using namespace std;
using namespace cv;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[]) {

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

	cvNamedWindow("Dog detecting camera", 1);

	// Capture images from any camera connected to the system
	// CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

	 if (argc <= 2){

    cout << "Usage: ./object_detector <model_name> <video_name>" << endl;
    return -1;
    }

    string model_name = argv[1];
    string video_name = argv[2];

	// Load the trained model
	CascadeClassifier Detector;
	Detector.load(model_name);

	if (Detector.empty()) {
		printf("Empty model.");
		return 0;
	}

	char key;
    
    
    VideoCapture video(video_name);
  	int chunk = video.get(CV_CAP_PROP_FRAME_COUNT) / numtasks;
  	video.set(CV_CAP_PROP_POS_FRAMES, chunk*taskid);
    
    if (!video.isOpened()){
        cout << "Could not read video file" << endl;
        return 1;
    }
   // Mat frame = cvarrToMat(cvQueryFrame(capture));
    
    int frame_count = 0;

    Mat frame;

    int count = 0;
    
	while (video.read(frame)) {
        

		std::vector<Rect> detected_frame;

		// Detect detected_frame
		Detector.detectMultiScale(frame, detected_frame, 1.1, 15,
				0 | CASCADE_SCALE_IMAGE, Size(200, 320));

		if (detected_frame.size() != 0){
			frame_count ++;
		}

		// If the frame is empty, break immediately
    	if (frame.empty() || (count >= chunk))
        	break;

		//cout << detected_frame.size() << endl;

		for (int i = 0; i < (int) detected_frame.size(); i++) {
			Point pt1(detected_frame[i].x, detected_frame[i].y);
			Point pt2(detected_frame[i].x + detected_frame[i].width,
					detected_frame[i].y + detected_frame[i].width);

			// Draw a rectangle around the detected brezel
			rectangle(frame, pt1, pt2, Scalar(0, 0, 255), 2);
			putText(frame, "Dog", pt1, FONT_HERSHEY_PLAIN, 1.0,
					Scalar(255, 0, 0), 2.0);

		}

		count++;
	}

	auto t2 = Clock::now(); 

	int global_frame_count;

	MPI_Reduce(&frame_count, &global_frame_count, 1, MPI_INT, MPI_SUM, 0,
           MPI_COMM_WORLD);

	if (taskid == MASTER){

		double timer = chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
  	
  		cout << "Runtime: " << timer << " seconds" << std::endl;

		double total_time = global_frame_count / (double) video.get(CV_CAP_PROP_FPS);

		cout << "Total apprearance time = " << total_time << " seconds" << endl;

		cout << " Frames per second: " << video.get(CV_CAP_PROP_FPS)<< "; Detected frames: " << global_frame_count << "; Total frames: " << video.get(CV_CAP_PROP_FRAME_COUNT) << endl;


	}


	MPI_Finalize();


	return 0;
}


