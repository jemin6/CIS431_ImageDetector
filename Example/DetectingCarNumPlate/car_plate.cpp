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
		


	return 0; 
} // End main 
