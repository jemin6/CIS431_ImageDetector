/*
        Opening video file

        To compile:
        g++ -o car_plate car_plate.cpp `pkg-config --cflags --libs opencv`
*/

#include <iostream>
#include "opencv2/opencv.hpp"

#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
	
	Mat image, image2, image3, drawing; // Make Images 
	Rect rect, temp_rect; // Make temp rectangles
	vector<vector<Point> > contours; // Vectors for 'findContours' function 
	vector<Vec4i> hierarchy; 

	double ratio, delta_x, delta_y, grandient; // Variables for 'Snake' algorithm 
	int select, plate_width, count, friend_count = 0, refinery_count = 0; 

	image = imread("Images/car_test.jpg");
	image2 = imread("Images/car_test.jpg");

	imshow("Test", image);

	cvtColor(image2, image2, CV_BGR2GRAY); // Covert to gray Image 

	Canny(image2, image2, 100, 300, 3);
	
	imshow("Original->Gray->Canny", image2);

	waitKey(10000);
		
	// Finding contours 
	findContours(image2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Rect> boundRect2(contours.size());

	// Bind rectangle to every rectangle 
	for(int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}
	
	
	for(int i = 0; i < contours.size(); i++) {
		ratio = (double) boundRect[i].height / boundRect[i].width;
		
		if((ratio <= 2.5) && (ratio >= 0.5) && (boundRect[i].area() <= 700) && (boundRect[i].area() >= 100)) {
			drawContours(drawing, contours, i, Scalar(0,255,255), 1, 8, hierarchy, 0, Point());
			rectangle(drawing, boundRect[i].tl(), boundRect[i].br() ,Scalar(255, 0, 0), 1, 8, 0); 

			refinery_count += 1; 
			boundRect2[refinery_count] = boundRect[i];

		}	

	}

	boundRect2.resize(refinery_count);
	imshow("Original->Gray->Canny->Contours&Rectangles", drawing);

	waitKey(10000);


	return 0; 
} // End main 
