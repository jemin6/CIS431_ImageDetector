/*
	Opening video file 
*/

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv; 
using namespace std; 

int main(int argc, char *argv[]) {

	// Input video file 
	VideoCapture video1("test_video.mp4");

	if(!video1.isOpened()) {
		printf("Cannot open video file!\n");
	} // End if statement

	// Video size 
	video1.set(CAP_PROP_FRAME_WIDTH, 400);
	video1.set(CAP_PROP_FRAME_HEIGHT, 720);
	
	Mat frame1; 
	namedWindow("video", 1);

	for(;;) {
		//Read one frame from webcam 
		video1 >> frame1;

		transpose(frame1, frame1);
		flip(frame1, frame1, 1);

		imshow("video", frame1);

		// Wait time so the vidoe doesn't run too fast 
		if(waitKey(20) == 27) {
			break; 
		} // End if statement

	} // End for loop

	return 0; 
} // End main
