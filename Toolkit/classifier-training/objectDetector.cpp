/*
 *
 *  g++ objectDetector.cpp -o test `pkg-config --cflags --libs opencv`
 *
 *  credit to the lesson on https://abhishek4273.com/2014/03/16/traincascade-and-car-detection-using-opencv/
 *
 *  Created on: Mar 6, 2018
 *  Author: yanhualuo
 */

#include <highgui.h>

#include <iostream>
#include <stdio.h>
#include <cv.hpp>

using namespace std;
using namespace cv;
using namespace std;

int main() {

	cvNamedWindow("Object detecting camera", 1);
	
	// Load the trained model
    // Here the default tranined model is the 'car.xml', you may change to other model
	CascadeClassifier objectDetector;
	objectDetector.load("car.xml");

	if (objectDetector.empty()) {
		printf("Empty model.");
		return 0;
	}

	char key;
    
    //default video
    VideoCapture video("honda.mp4");

    if (!video.isOpened()){
        cout << "Could not read video file" << endl;
        return 1;
    }
    
    Mat frame;
    
	while (video.read(frame)) {

		vector<Rect> objects;

		// Detect object
		objectDetector.detectMultiScale(frame, objects, 1.1, 12,
				0 | CV_HAAR_SCALE_IMAGE, Size(200, 320));

		for (int i = 0; i < (int) objects.size(); i++) {
			Point pt1(objects[i].x, objects[i].y);
			Point pt2(objects[i].x + objects[i].width,
					objects[i].y + objects[i].width);

			// Draw a rectangle around the detected the object
			rectangle(frame, pt1, pt2, Scalar(0, 0, 255), 2);
			putText(frame, "Object", pt1, FONT_HERSHEY_PLAIN, 1.0,
					Scalar(255, 0, 0), 2.0);

		}
        
		// Show the transformed frame
		imshow("Car detecting video", frame);

		// Read keystrokes, exit after ESC pressed
		key = cvWaitKey(10);
		if (char(key) == 27) {
			break;
		}
	}

	return 0;
}

