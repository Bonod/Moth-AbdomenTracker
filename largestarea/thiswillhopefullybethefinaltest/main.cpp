////////////////////////////
//thanks to Shermal Fernado (http://opencv-srf.blogspot.com/) for very helpful open cv tutorials and image reading samples
////////////////////////////
//note: we may be limited by speed, if this is the case, video file may be better
//#include "C:\opencv\build\include\opencv2/highgui/highgui.hpp" //Could not get path stuff working, this will have to do for now
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <time.h>
#include "C:\opencv\build\include\opencv\cv.h"
#include "C:\opencv\build\include\opencv\highgui.h"
using namespace cv;
using namespace std;

ofstream SaveFile("dotstats.txt");

Mat mothframe; // image frame of video
Mat mothframe1; //blurred image
Mat mothframe2; //converted frame by threshold
Mat mothframe3; // contoured frame
vector<vector<Point> > contourvector; // vector of contour vectors
vector<Vec4i> hierarchy;
RotatedRect rectangle1; //rectangle used to find center 
int largestcontour; //variable for storing largest contour
double largestcontourarea; //variable for storing largest contour area

time_t starttime;
time_t endtime;
double difference;

double findbiggest(vector<vector<Point> > vector) //returns contour number with greatest area
{
	double max = 0;
	double maxarea = 0;
	int thisarea;
	for(int i = 0; i < vector.size(); i++)
	{
		thisarea = contourArea(vector[i]);

		if(thisarea > maxarea)
		{
			max = i;
			maxarea = thisarea;
		}	
	}
	return  max;
}
void savecoordinates() //saves coordinates to txt file
{
	SaveFile<<"Contour, Contour Area, Contour Center, Rectangle Size"<<endl;
		SaveFile<<largestcontour<<" , "<<largestcontourarea<<" , "<<rectangle1.center<<"  , "<<rectangle1.size<<endl; 
}
int main(int argc, char* argv[])  
{
	VideoCapture cap(0);   // camera 
	
	while(true)
	{
	
	 mothframe = imread("whiteout-0000000373.ppm");
	
	//bool readframe = cap.read(mothframe);  // camera reads to mat mothframe  
	//if(readframe != true) // checks to see if read
	//{
	//		cout<<"not reading camera"<<endl;
	//		break; 
	//}
	//time(&starttime);
	/*for(int j = 0; j < 1000; j++)
	{*/

	GaussianBlur(mothframe,mothframe1, Size(31,31),0,0); //blurs image to aid in contour finding
	threshold(mothframe1, mothframe2, 140, 255, THRESH_BINARY); // thresholds image to create black and white
	Canny(mothframe2,mothframe3, 1,255, 3); //marks edjes of blurred image
	findContours(mothframe3, contourvector, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0)); //finds and stores contours

	if(contourvector.size() > 0)
	{
	largestcontour = findbiggest(contourvector);
	largestcontourarea = contourArea(contourvector[largestcontour]);
	rectangle1 = minAreaRect(contourvector[largestcontour]); //finds rectangle to find center
	circle(mothframe3, rectangle1.center, 10 ,Scalar(150,150,150),3,8,0); //draws circle
	}

	//}
	//time(&endtime);
	/*difference = difftime(endtime, starttime);
	cout<<difference;
	*/

	namedWindow("Window1", CV_WINDOW_AUTOSIZE);
	namedWindow("Window2", CV_WINDOW_AUTOSIZE);
	namedWindow("Window3", CV_WINDOW_AUTOSIZE); 
	namedWindow("Window4", CV_WINDOW_AUTOSIZE); 
	imshow("Window1", mothframe); //displays windows
	imshow("Window2", mothframe1);
	imshow("Window3", mothframe2);
	imshow("Window4", mothframe3);
    
	if(waitKey(20) == 27) //required; escape to quit
	{
			break;
	}
	/*char wait;
	cin>>wait;*/
	savecoordinates();
	contourvector.clear(); //clears stuff
	hierarchy.clear();
	}
	SaveFile.close();
	return 0;
}