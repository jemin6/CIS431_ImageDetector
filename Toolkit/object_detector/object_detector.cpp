/*
 *
 * g++ object_detector.cpp -o object_detector `pkg-config --cflags --libs opencv` -std=c++11
 * ./object_detector <model_name> <video_name>
 *
 * credit to the lesson on https://abhishek4273.com/2014/03/16/traincascade-and-car-detection-using-opencv/
 
 *  Created on: Mar 6, 2018
 *      Author: yanhualuo
 */

#include <highgui.h>

#include <iostream>
#include <stdio.h>
#include <cv.hpp>

using namespace std;
using namespace cv;
using namespace std;

typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char* argv[]) {

	auto t1 = Clock::now(); 

	cvNamedWindow("Dog detecting camera", 1);
	// Capture images from any camera connected to the system
//	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);

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
    /*
    length = int(video.get())
    cout << "number of frame: " <<
     */
    
    if (!video.isOpened()){
        cout << "Could not read video file" << endl;
        return 1;
    }
   // Mat frame = cvarrToMat(cvQueryFrame(capture));
    
    int frame_count = 0;

    Mat frame;
    
	while (video.read(frame)) {
        

		std::vector<Rect> detected_frame;

		// Detect detected_frame
		Detector.detectMultiScale(frame, detected_frame, 1.1, 15,
				0 | CASCADE_SCALE_IMAGE, Size(200, 320));

		if (detected_frame.size() != 0){
			frame_count ++;
		}

		//cout << detected_frame.size() << endl;

		for (int i = 0; i < (int) detected_frame.size(); i++) {
			Point pt1(detected_frame[i].x, detected_frame[i].y);
			Point pt2(detected_frame[i].x + detected_frame[i].width,
					detected_frame[i].y + detected_frame[i].width);

			// Draw a rectangle around the detected brezel
			rectangle(frame, pt1, pt2, Scalar(0, 0, 255), 2);
			putText(frame, "", pt1, FONT_HERSHEY_PLAIN, 1.0,
					Scalar(255, 0, 0), 2.0);

		}
        
		// Show the transformed frame
		imshow("Detecting camera", frame);

		// Read keystrokes, exit after ESC pressed
		key = cvWaitKey(10);
		if (char(key) == 27) {
			break;
		}
	}

	auto t2 = Clock::now(); 

	double timer = chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1000000000.0;
  cout << "Runtime: "
  << timer
    << " seconds" << std::endl;

	double total_time = frame_count / (double) video.get(CV_CAP_PROP_FPS);

	cout << "Total apprearance time = " << total_time << " seconds" << endl;

	cout << " Frames per second: " << video.get(CV_CAP_PROP_FPS)<< "; Detected frames: " << frame_count << "; Total frames: " << video.get(CV_CAP_PROP_FRAME_COUNT) << endl;

	return 0;
}


