/*
 * Stich.hpp
 *
 *  Created on: 2018年10月9日
 *      Author: wj
 */

#ifndef STICH_HPP_
#define STICH_HPP_

#include <stdio.h>
#include <time.h>       /* For our FPS */
#include <string.h>    /* for strcpy and relatives */
#include <unistd.h>     /* needed to sleep*/
#include <math.h>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void cylinder(Mat& src,Mat & dst,double R,int shift);
void FusionSeam(Mat& src,Mat & dst,int seampostion);
void setpanoparam(unsigned int w,unsigned int h);
int getpanooffet(double angle);
void setfusionenalge(bool enable);
bool getfusionenable();
int  getPanoOffset(cv::Mat & src,cv::Mat & dst,int *xoffset ,int* yoffset);
double offet2angle(int  offsetx);
bool zerocalibration(Mat src,Mat dst,double *angleoffset);
void Matcpy(Mat src,Mat dst,int offset);
double offet2anglerelative(int  offsetx);

#endif /* STICH_HPP_ */
