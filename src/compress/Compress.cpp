#include"Compress.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
#include "screenCapture.h"

Compress::Compress()
{
	
}
Compress::~Compress()
{
	

}
void Compress::create()
{
	gst_videnc_create();
	gst_videnc_bitrate(2);

}

void Compress::encode(Mat src)
{
	

}
