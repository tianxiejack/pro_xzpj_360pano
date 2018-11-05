/*
 * Homography.hpp
 *
 *  Created on: 2018年9月20日
 *      Author: wj
 */

#ifndef HOMOGRAPHY_HPP_
#define HOMOGRAPHY_HPP_

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"


void FindHomographyfromPicture(cv::Mat& src,cv::Mat & dst,cv::Mat &H);
int  getPano360Rotation(cv::Mat & src,cv::Mat & dst,double *rotation );
int  getPano360Offset(cv::Mat & src,cv::Mat & dst,int *xoffset ,int* yoffset);
int  getPano360OffsetT(cv::Mat & src,cv::Mat & dst,int *xoffset ,int* yoffset) ;
cv::Mat  FindHomography(cv::Mat& src,cv::Mat & dst);

#endif /* HOMOGRAPHY_HPP_ */
