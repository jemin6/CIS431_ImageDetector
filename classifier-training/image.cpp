//
//  pos_image.cpp
//  
// g++ -o images image.cpp `pkg-config --cflags --libs opencv`
//
//  Created by Yanhua Luo on 3/12/18.
//
/*
 * This file is for creating and modifying both of the positive and negative dataset.
 *  - Positive image: all images are mandatory resize to 100x100, it creates all images in grayscale,
 *    one origin, one mirror, and four chopped section images.
 *  - Negative image: all images are mandatory resize to 100x100, it creates all images in grayscale,
 *    only one original.
 *  - Each provided positive image should result in 6 different images.
 *  - Each provided negative image should result in 1 different iamge.
 */

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <math.h>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#define cimg_use_jpeg
#include "CImg.h"

#include <cv.hpp>

using namespace std;
using namespace cv;

typedef chrono::high_resolution_clock Clock;

const int newWidth = 100;
const int newHeight = 100;

void posLoopOver(vector<Mat> lines, vector<String> fn){
    cout << "loopover "<< lines.size() << endl;
    
    Mat mat;        // input image
    Mat resize_mat; // resize image
    Mat gray_mat;   // grayscale image
    Mat topLeft_mat;
    Mat topRight_mat;
    Mat bottomLeft_mat;
    Mat bottomRight_mat;
    Mat mirror_hor_mat;
    for (int i = 0; i < lines.size(); i++){
        mat = lines[i];
        resize(mat, resize_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        
        //graysacle
        cvtColor(resize_mat, gray_mat, CV_BGR2GRAY);
        
        flip(gray_mat, mirror_hor_mat, +1);
        
        //chop the original image into 4 different images
        resize(Mat(gray_mat, Rect(0, 0 , 50, 50)), topLeft_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        resize(Mat(gray_mat, Rect(0, 50 , 50, 50)), bottomLeft_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        resize(Mat(gray_mat, Rect(50, 0, 50, 50)), topRight_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        resize(Mat(gray_mat, Rect(50, 50, 50, 50)), bottomRight_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        
        string file = fn[i];
        int idx = file.rfind('/');
        string file_name_gray = "pos-training/new_" + file.substr(idx+1);
        string file_name_hor = "pos-training/hor_" + file.substr(idx+1);
        
        string file_name_topLeft = "pos-training/topLeft_" + file.substr(idx+1);
        string file_name_bottomLeft = "pos-training/bottomLeft_" + file.substr(idx+1);
        string file_name_topRight = "pos-training/topRight_" + file.substr(idx+1);
        string file_name_bottomRight = "pos-training/bottomRight_" + file.substr(idx+1);
        
        imwrite(file_name_gray.c_str(), gray_mat);
        imwrite(file_name_hor.c_str(), mirror_hor_mat);
        
        imwrite(file_name_topLeft.c_str(), topLeft_mat);
        imwrite(file_name_bottomLeft.c_str(), bottomLeft_mat);
        imwrite(file_name_topRight.c_str(), topRight_mat);
        imwrite(file_name_bottomRight.c_str(), bottomRight_mat);

    }
    
}

void negLoopOver(vector<Mat> lines, vector<String> fn){
    cout << "loopover "<< lines.size() << endl;
    
    Mat mat;        // input image
    Mat resize_mat; // resize image to 60x60
    Mat gray_mat;   // grayscale image
    
    for (int i = 0; i < lines.size(); i++){
        mat = lines[i];
        resize(mat, resize_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        
        cvtColor(resize_mat, gray_mat, CV_BGR2GRAY);
        
        string file = fn[i];
        int idx = file.rfind('/');
        string file_name_gray = "neg-training/new_" + file.substr(idx+1);
        
        imwrite(file_name_gray.c_str(), gray_mat);
        
    }
}


int main(int argc, char* argv[]){
    if (argc > 2){
        cout << "positive images folder: " << argv[1] << endl;
        cout << "negative images folder: " << argv[2] << endl;
    }else {
        cout << "No folder name enter. Exiting.." << endl;
        return -1;
    }
    
    vector<String> posFn;
    string posFolderName = argv[1];
    string posFolderImages = posFolderName + "/*.jpg";
    
    vector<String> negFn;
    string negFolderName = argv[2];
    string negFolderImages = negFolderName + "/*.jpg";
 
    //read a sequence of files within a folder
    glob(posFolderImages, posFn, false);
    glob(negFolderImages, negFn, false);
    
    auto t1 = Clock::now();
    
    vector<Mat> posImages;
    vector<Mat> negImages;
    
    size_t posCount = posFn.size();
    size_t negCount = negFn.size();
    cout << "size is " << posCount << endl;
    cout << "size is " << negCount << endl;
    
    for (int i =0; i< posCount; i++){
        posImages.push_back(imread(posFn[i]));
        
    }
    
    for (int i =0; i< negCount; i++){
        negImages.push_back(imread(negFn[i]));
        
    }
    
    auto t2 = Clock::now();
    double timer = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
    cout << "image push back : " << timer / 1000000000 << " seconds" << endl;
    
    
    //----------------------------------
    auto t3 = Clock::now();
    
    posLoopOver(posImages, posFn);
    negLoopOver(negImages, negFn);
    
    auto t4 = Clock::now();
    double timer2 = chrono::duration_cast<chrono::nanoseconds>(t4 - t3).count();
    cout << "loop over time : " << timer2 / 1000000000 << " seconds" << endl;
    
    return 0;
    
}
