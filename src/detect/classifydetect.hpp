/*
 * classifydetect.hpp
 *
 *  Created on: 2018年11月28日
 *      Author: wj
 */

#ifndef CLASSIFYDETECT_HPP_
#define CLASSIFYDETECT_HPP_
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
typedef void ( *LPNOTIFYFUNCCD)(void *context, int chId);
class ClassifyDetect
{

public:
	ClassifyDetect();
	~ClassifyDetect();
	void movdetectcreate(LPNOTIFYFUNCCD fun, void *context);
	LPNOTIFYFUNCCD callback;
	void * cdcontext;
public:
	int create();
	void destory();
	void detect(cv::Mat frame,int chid);
	void getMoveTarget(std::vector<cv::Rect> &objects,int chId);
public:
	cv::CascadeClassifier cascade;
	std::vector<cv::Rect> objectsrect;
};


#endif /* CLASSIFYDETECT_HPP_ */
