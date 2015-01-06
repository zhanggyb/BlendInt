/*
 * MBARViewport.cpp
 *
 *  Created on: 2014年12月30日
 *      Author: zhanggyb
 */

#include "MBARViewport.hpp"

#include <opencv2/imgproc/imgproc.hpp>

MBARViewport::MBARViewport()
: BlendInt::CVVideoViewport()
{

}

MBARViewport::~MBARViewport()
{

}

void MBARViewport::ProcessImage(cv::Mat& image)
{
//	cv::Mat grey;
//	cv::cvtColor(image, grey, CV_BGR2GRAY);
//	image = grey;
	cv::Mat gray;

	cv::cvtColor(image, gray, CV_RGB2GRAY);

	image = gray;
}
