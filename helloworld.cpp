#include "iostream"
#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv;

string videoname = "clip.mp4";

int main(){
	cout << " Hello Opencv" << endl;
	// Credited by http://blog.csdn.net/Grhdu/article/details/73997553

	VideoCapture capture(videoname);
    int frameH = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    int frameW = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int frameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
    Mat frame;
    char name[5];
    for (int i = 1; i <= frameNumber / 50; i++)
    {
        printf(name, "%d", i);
        string s = "Tang";
        string vname = s + name + ".avi";
        VideoWriter  writer(vname, CV_FOURCC('M', 'J', 'P', 'G'), 25, Size(frameW, frameH), 1);//每秒25帧
        for (int j = 1; j <= 50; j++)
        {
            capture >> frame;
            writer << frame;
            imshow("1", frame);
            waitKey(1);
        }
        writer.release();
    }
    capture.release();
	return 0;
}
