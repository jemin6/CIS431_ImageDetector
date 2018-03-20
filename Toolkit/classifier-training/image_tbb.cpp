//
// image_tbb.cpp
//
// g++ -I/opt/X11/include -o image_tbb image_tbb.cpp -L/opt/X11/lib -lX11 -ljpeg -ltbb `pkg-config --cflags --libs opencv` -std=c++11
//
//  Created by Yanhua Luo on 3/12/18.

/*
 * This file is for creating and modifying both of the positive and negative dataset.
 *  - Positive image: all images are mandatory resize to 100x100, it creates all images in grayscale,
 *    one origin, one mirror, and four chopped section images.
 *  - Negative image: all images are mandatory resize to 100x100, it creates all images in grayscale,
      only one origin.
    Each provided positive image should result in 6 different images.
    Each provided negative image should result in 1 different iamge.
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
#include <fstream>
#include <chrono>
#include <cv.hpp>

#include <tbb/tbb.h>


using namespace std;
using namespace cv;
using namespace tbb;

typedef chrono::high_resolution_clock Clock;

const int newWidth = 100;
const int newHeight = 100;

concurrent_vector<Mat> posImages;
concurrent_vector<Mat> negImages;
vector<String> posFn;
vector<String> negFn;


class Pos : public ParallelLoopBody{
public:
    void operator () (const Range& range) const {
        for (int i = range.start; i < range.end; ++ i){
            posImages.push_back(imread(posFn[i]));
        }
    }
};

class Neg : public ParallelLoopBody{
    public:
    void operator () (const Range& range) const {
        for (int i = range.start; i < range.end; ++ i){
            negImages.push_back(imread(negFn[i]));
        }
    }
};

void posLoopOver(concurrent_vector<Mat> lines, vector<String> fn){
    cout << "loopover "<< lines.size() << endl;
    
    Mat mat;        // input image
    Mat resize_mat; // resize image to 60x60
    Mat gray_mat;   // grayscale image
    Mat topLeft_mat;
    Mat topRight_mat;
    Mat bottomLeft_mat;
    Mat bottomRight_mat;
    Mat mirror_hor_mat;
    int idx;
    
    
    for( int i = 0; i < lines.size(); i++){
        mat = lines[i];
        resize(mat, resize_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        cvtColor(resize_mat, gray_mat, CV_BGR2GRAY);
        
        //creating a tbb::task_group to run tasks concurrently.
        task_group g;
        
        string file = fn[i];
        idx = file.rfind('/');
        
        g.run([&](){
            string file_name_gray = "pos-training/new_" + file.substr(idx+1);
            imwrite(file_name_gray.c_str(), gray_mat);
        });
        
        // mirror the garyscale image
        g.run([&](){
            flip(gray_mat,mirror_hor_mat, +1);
            string file_name_hor = "pos-training/hor_" + file.substr(idx+1);
            imwrite(file_name_hor.c_str(), mirror_hor_mat);
        });
        
        
        //-------------------chop the image into 4 smaller images---------------------------
        g.run([&]{
            resize(Mat(gray_mat, Rect(0, 0 , 50, 50)), topLeft_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
            string file_name_topLeft = "pos-training/topLeft_" + file.substr(idx+1);
            imwrite(file_name_topLeft.c_str(), topLeft_mat);
        });
        
        g.run([&]{
            resize(Mat(gray_mat, Rect(0, 50 , 50, 50)), bottomLeft_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
            string file_name_bottomLeft = "pos-training/bottomLeft_" + file.substr(idx+1);
            imwrite(file_name_bottomLeft.c_str(), bottomLeft_mat);
        });
        
        g.run([&]{
            resize(Mat(gray_mat, Rect(50, 0, 50, 50)), topRight_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
            string file_name_topRight = "pos-training/topRight_" + file.substr(idx+1);
            imwrite(file_name_topRight.c_str(), topRight_mat);
        });
        
        g.run([&]{
            resize(Mat(gray_mat, Rect(50, 50, 50, 50)), bottomRight_mat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
            string file_name_bottomRight = "pos-training/bottomRight_" + file.substr(idx+1);
            imwrite(file_name_bottomRight.c_str(), bottomRight_mat);
        });
        //-------------------------------------------------------------------------------------
        
        g.wait();
        
    }
    
}

void negLoopOver(concurrent_vector<Mat> lines, vector<String> fn) {
    
    cout << "loopover "<< lines.size() << endl;
    
    Mat negMat;        // input image
    Mat resize_negmat; // resize image to 60x60
    Mat gray_negmat;   // grayscale image
    
    for (int i = 0; i < lines.size(); i++){
        negMat = lines[i];
        resize(negMat, resize_negmat, Size(newWidth, newHeight), 0, 0, CV_INTER_LINEAR);
        cvtColor(resize_negmat, gray_negmat, CV_BGR2GRAY);
        
        string file = fn[i];
        int idx = file.rfind('/');
        
        string file_name_gray = "neg-training/new_" + file.substr(idx+1);
        imwrite(file_name_gray.c_str(), gray_negmat);
    }
}



int main(int argc, char* argv[]){
    if (argc > 2){
        cout << "positive image folder: " << argv[1] << endl;
        cout << "negitive image folder: " << argv[2] << endl;
    }else {
        cout << "Missing one, or No folder name enter. Exiting.." << endl;
        return -1;
    }
    
    
    string posFolderName = argv[1];
    string posFolderImages = posFolderName + "/*.jpg";
    
    string negFolderName = argv[2];
    string negFolderImages = negFolderName + "/*.jpg";
    // cout << "folderimage " << folderImages << endl;
    
    //check if the user input folder exist.
    ifstream infile(argv[1]);
    if (infile.is_open() && infile.good()){
        glob(posFolderImages, posFn, false);
        glob(negFolderImages, negFn, false);
    }else{
        cout << "No such folder exist. Exiting..." << endl;
        return -1;
    }
    
    auto t1 = Clock::now();
    
    size_t posCount = posFn.size();
    size_t negCount = negFn.size();
    cout << "pos size is " << posCount << endl;
    cout << "neg size is " << negCount << endl;
    
    //tbb::parallel_for_ pushing image into the vector
    Pos posimage;
    parallel_for_(Range(0, posCount), posimage);
    Neg negimage;
    parallel_for_(Range(0, negCount), negimage);
    
    auto t2 = Clock::now();
    double timer = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count();
    cout << "pushing images : " << timer / 1000000000 << " seconds" << endl;
    
    //-----------------------------------------------
    auto t3 = Clock::now();
    
    posLoopOver(posImages, posFn);
    
    negLoopOver(negImages, negFn);
    
    auto t4 = Clock::now();
    double timer2 = chrono::duration_cast<chrono::nanoseconds>(t4 - t3).count();
    cout << "loop over time : " << timer2 / 1000000000 << " seconds" << endl;
    
    return 0;
    
}

