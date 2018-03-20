# Video object detecting

## Description: 

Detecting object in a video

## Usage:

Compaile and run the following files on command line: (Make sure OpenCV and OpenMPI have been installed)

for object_detector.cpp 

~~~
   g++ object_detector.cpp -o object_detector `pkg-config --cflags --libs opencv` -std=c++11
   ./object_detector <model_name> <video_name>
~~~

for object_detector_no_play.cpp 

~~~
   g++ object_detector_no_play.cpp -o object_detector_no_play `pkg-config --cflags --libs opencv` -std=c++11
   ./object_detector_no_play <model_name> <video_name>
~~~

for object_detector_no_play_4p.cpp 

~~~
   mpic++ -std=c++11 -o object_detector_no_play_4p object_detector_no_play_4p.cpp  `pkg-config --cflags --libs opencv`
   mpirun --hostfile hostfile -np 4 object_detector_no_play_4p <model_name> <video_name>
~~~

for object_detector_no_play_9p.cpp 

~~~
   mpic++ -std=c++11 -o object_detector_no_play_9p object_detector_no_play_9p.cpp  `pkg-config --cflags --libs opencv`
   mpirun --hostfile hostfile -np 9 object_detector_no_play_9p <model_name> <video_name>
~~~

