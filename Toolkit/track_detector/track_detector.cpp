// g++ -o track_detector track_detector.cpp  `pkg-config --cflags --libs opencv`
// ./track_detector


#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

// Convert to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
( std::ostringstream() << std::dec << x ) ).str()

int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.2
    // NOTE : GOTURN implementation is buggy and does not work.
    string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN"};
    // vector <string> trackerTypes(types, std::end(types));
    
    // Create a tracker
    string trackerType = trackerTypes[2];
    
    Ptr<Tracker> tracker;
    
#if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
#else //s
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
    }
#endif

    // Read video
    if (argc <= 1){
        cout << "Usage: ./track_detector <file_name> <frame_number(default=0)>" << endl;
        return -1;
    }

    long int frame_number = 0;
    string file_name = argv[1];

    if (argc == 3){
        string frame_number_char = argv[2];
        frame_number = stoi(frame_number_char, nullptr, 10);

    }

    VideoCapture video(file_name); 
    
    // Exit if video is not opened
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
        
    }

    video.set(CV_CAP_PROP_POS_FRAMES, frame_number);

    // For frame name
    int num = 0;
    char name [50];
    
    // Read first frame
    Mat frame;
    bool ok = video.read(frame);
    
    // Define initial boundibg box
    Rect2d bbox(287, 23, 86, 320);
    
    // Uncomment the line below to select a different bounding box
    bbox = selectROI(frame, false);
    
    // Display bounding box.
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );

    imshow("Tracking", frame);
    
    tracker->init(frame, bbox);

    video.set(CV_CAP_PROP_POS_MSEC, 0);
    
    while(video.read(frame))
    {
        // Start timer
        double timer = (double)getTickCount();
        
        // Update the tracking result
        bool ok = tracker->update(frame, bbox);
        
        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
        
        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
        }
        
        // Display frame.
        imshow("Tracking", frame);
        
        // Exit if ESC pressed.
        int k = waitKey(1);
        if(k == 27)
        {
            break;
        }
        
    }
}

