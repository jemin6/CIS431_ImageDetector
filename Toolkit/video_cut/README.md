# Video cutting tool

## Description: 

Split the video into frames.

## Usage:

Compaile and run the following files on command line: (Make sure OpenCV and OpenMPI have been installed)

for video_cut.cpp 

~~~
   g++ -o video_cut video_cut.cpp  `pkg-config --cflags --libs opencv` -std=c++11
   ./video_cut <file_name>
~~~

for video_cut_4p.cpp

~~~
    mpic++ -std=c++11 -o video_cut_4p video_cut_4p.cpp  `pkg-config --cflags --libs opencv` -std=c++11
    mpirun --hostfile hostfile -np 4 video_cut_4p <file_name>
~~~

