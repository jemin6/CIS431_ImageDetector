/*
	Opening video file

	To compile:
	g++ -o read_video read_video.cpp `pkg-config --cflags --libs opencv` 
*/

#include <iostream>
#include "opencv2/opencv.hpp"

#include <stdlib.h>
#include <stdio.h>

using namespace cv; 
using namespace std; 

int main(int argc, char *argv[]) {

	// Input video file 
	VideoCapture video1("Videos/test_video.mp4");

	if(!video1.isOpened()) {
		cerr << "Cannot open video file!" << endl;
	} // End if statement

	// Face detection configuration
	CascadeClassifier face;

	face.load("opencv/data/haarcascades/haarcascade_frontalface_default.xml");
	
	
	Mat frame1; 
	namedWindow("video", 1);

	for(;;) {

		bool frame_valid = true; 

		
		try {
			//Read one frame from webcam 
			video1 >> frame1;
		} catch(Exception& e) {
			cerr << "Exception occurred. Ignoring frame ... " << e.err << endl;
			frame_valid = false;
		} // End try catch

		if(frame_valid) {
		
			transpose(frame1, frame1);
			flip(frame1, frame1, 1);


		   try {
			// convert captured frame to gray scale & equalize
			Mat grayframe; 
			cvtColor(frame1, grayframe, CV_BGR2GRAY);
			equalizeHist(grayframe, grayframe);

			// face detection routine 
			vector<Rect> faces; 

			face.detectMultiScale(grayframe, faces, 1.1, 3, 
						CV_HAAR_FIND_BIGGEST_OBJECT | 
						CV_HAAR_SCALE_IMAGE, Size(30,30) ); 

			// draw the results 
			for(int i = 0; i < faces.size(); i++) {
				Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
				Point tr(faces[i].x, faces[i].y);
				rectangle(frame1, lb, tr, Scalar(0,255,0), 3, 4, 0);

			} // End for loop

			imshow("video", frame1);
		   } catch(Exception& e) {
			cerr << "Exception occurred. Ignoring frame ... " << e.err << endl;
		   } // End try catch
			
		} // End if statement

		// Wait time so the vidoe doesn't run too fast 
		if(waitKey(20) == 27) {
			break; 
		} // End if statement

	} // End for loop

	return 0; 
} // End main
