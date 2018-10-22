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
//bool zerocalibration(Mat src,Mat dst,double *angleoffset);
bool zerocalibration(Mat src,Mat dst,double *angleoffset,int *xoffset);
void Matcpy(Mat src,Mat dst,int offset);
double offet2anglerelative(int  offsetx);
void setpanomap(Mat pano[]);
Mat getpanomap();
void getpanoparam(unsigned int * w,unsigned int * h);
void setnextnum();
Mat getpanolastmap();
void setmvdetect(std::vector<cv::Rect> &mv,int chid);
void  getmvdetect(std::vector<cv::Rect> &mv,int chid);
void Multipotionto360(std::vector<cv::Rect> &mv,int chid);
void mvdetectup(std::vector<cv::Rect> &mv);
void mvclassification(std::vector<cv::Rect> &mv,std::vector<cv::Rect> &mv180,std::vector<cv::Rect> &mv360);
void stichinit();
void Matblack(Mat src,int offset);
double offet2anglerelative2(int  offsetx);
void histequision(Mat& src);

#endif /* STICH_HPP_ */
