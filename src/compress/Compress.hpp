/*
 * Compress.hpp
 *
 *  Created on: 2018年11月15日
 *      Author: wj
 */

#ifndef COMPRESS_HPP_
#define COMPRESS_HPP_

#include <opencv2/opencv.hpp>
using namespace cv;

class Compress
{
	public :
		Compress();
		~Compress();
		void create();
		
		void encode(Mat src);
};


#endif /* COMPRESS_HPP_ */
