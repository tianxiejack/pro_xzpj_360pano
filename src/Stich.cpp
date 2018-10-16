#include"Stich.hpp"
#include"config.h"
#include"Homography.hpp"
#include"osa.h"
unsigned int panowidth=0;
unsigned int panoheight=0;
double angle=0.0;


static unsigned int zeroimagpenum=0;
bool imgprocessenable =false;

void setfusionenalge(bool enable)
{
	imgprocessenable=enable;
}
bool getfusionenable()
{
	return imgprocessenable;
}
void setpanoparam(unsigned int w,unsigned int h)
{
	panowidth=w;
	panoheight=h;

}

int getpanooffet(double angle)
{
	int xoffset=PANOSCALE*angle*panowidth/360;
	return xoffset;

}
double offet2angle(int  offsetx)
{
	//int xoffset=PANOSCALE*angle*panowidth/360;
	angle+=offsetx*360.0/(panowidth*PANOSCALE);
	return angle;

}

double offet2anglerelative(int  offsetx)
{
	//int xoffset=PANOSCALE*angle*panowidth/360;
	double angle;
	angle=offsetx*360.0/(panowidth*PANOSCALE);
	return angle;

}

void cylinder(Mat& src,Mat & dst,double R,int shift)
{
	int width = src.cols, height = src.rows;
	double x, y;
	//double R = width / 2;
	int drcpoint;
	double fovAngle=2*atan(width/(2.0*R));
	for (int hnum = 0; hnum < height;hnum++)
	{
		for (int wnum = shift; wnum < width;wnum++)
		{
			double k = R / sqrt(R*R + (wnum - width / 2)*(wnum - width / 2));
			//x = (wnum - width / 2) / k + width / 2;
			#if 0
			x= width/2.0 + R * tan((wnum-R * fovAngle/2)/R) ;
			y=hnum;
			#else
			//x==  width/2.0 - R  * tan((R  * fovAngle/2 - wnum)/R ) + 0.5;
			x=width/2+R*tan((wnum-width/2)/R);
			y = (hnum - height / 2) / k + height / 2;
			#endif
			
			if (0 < x && x < width && 0 < y &&y < height)
			{
				dst.at<Vec3b>(hnum, wnum-shift) = src.at<Vec3b>(int(y), int(x));
			}
		}
	}


}


void FusionSeam(Mat& src,Mat & dst,int seampostion)
{
	double processWidth = src.cols - seampostion;
	processWidth=seampostion;
	int rows = src.rows;
	int cols = src.cols; 
	double alpha = 1;


	for (int i = 0; i < rows; i++)
	{
		uchar* p  = src.ptr<uchar>(i);  
		uchar* t  = dst.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = 0; j < processWidth; j++)
		{
			if (p[(j+seampostion)* 3] == 0 && p[(j+seampostion) * 3 + 1] == 0 && p[(j+seampostion) * 3 + 2] == 0)
			{
				alpha = 0;
			}
			else
			alpha =(processWidth- j)*1.0/ processWidth;
			
			d[j * 3] = p[(j+seampostion) * 3] * alpha + t[j * 3] * (1 - alpha);
			d[j * 3 + 1] = p[(j+seampostion) * 3 + 1] * alpha + t[j * 3 + 1] * (1 - alpha);
			d[j * 3 + 2] = p[(j+seampostion)* 3 + 2] * alpha + t[j * 3 + 2] * (1 - alpha);
		}
	}

	

}

int  getPanoOffset(cv::Mat & src,cv::Mat & dst,int *xoffset ,int* yoffset)
{
	//return getPano360Offset( src, dst,xoffset ,yoffset);
	return getPano360OffsetT( src, dst,xoffset ,yoffset);
}


bool zerocalibration(Mat src,Mat dst,double *angleoffset)
{
	bool ret=false;
	int status=0;
	int xoffsetfeat=0;
	int yoffsetfeat=0;
	char dstbuf[50];
	memset(dstbuf,0,sizeof(char)*50);
	sprintf(dstbuf,"zero_%d.jpg",zeroimagpenum);
	OSA_printf("zerocalibration begin\n");
	imwrite("src.jpg",src);
	imwrite(dstbuf,dst);
	
	status=getPanoOffset(src,dst,&xoffsetfeat,&yoffsetfeat);
	if(status==-1)
		return ret;
	*angleoffset=offet2anglerelative(xoffsetfeat);
	
	OSA_printf("the zeroimagpenum=%d oofset=%d  angle=%f\n",zeroimagpenum,xoffsetfeat,*angleoffset);
	zeroimagpenum++;
	return true;
}

void Matcpy(Mat src,Mat dst,int offset)
{
	int width = src.cols, height = src.rows;
	double x, y;
	//double R = width / 2;
	int drcpoint;

	for (int hnum = 0; hnum < height;hnum++)
	{
		for (int wnum = 0; wnum < width;wnum++)
		{	
			if(wnum+offset<width)
				dst.at<Vec3b>(hnum, wnum) = src.at<Vec3b>(int(hnum), int(wnum+offset));
			
		}
	}


}
