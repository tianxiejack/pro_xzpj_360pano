#include"classifydetect.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include<string.h>
#include "stdio.h"
#include "osa.h"
#include "config.h"
#include <cctype>
#include <iterator>

using namespace std;
using namespace cv;
const string cascade_names = "cars.xml";

ClassifyDetect::ClassifyDetect()
{


}
ClassifyDetect::~ClassifyDetect()
{


}
void ClassifyDetect::movdetectcreate(LPNOTIFYFUNCCD fun, void *context)
{
		callback=fun;
		cdcontext=context;
}
int  ClassifyDetect::create()
{
	 if( !cascade.load( cascade_names ) ){ printf("--(!)Error loading\n"); return -1; };
	

};

void ClassifyDetect::getMoveTarget(vector<Rect> &objects,int chId)
{
	objects.clear();
	for(int i=0;i<objectsrect.size();i++)
		{
			;
			objects.push_back(objectsrect[i]);
		}
	
}
void ClassifyDetect::detect(Mat frame,int chid)
{
	//std::vector<Rect> faces;
	objectsrect.clear();
	//printf("*************%s*****************\n",__func__);
	//return ;
	cascade.detectMultiScale( frame, objectsrect, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
	if(callback!=NULL)
		callback(cdcontext,chid);

}


