/*
 * MBARViewport.hpp
 *
 *  Created on: 2014年12月30日
 *      Author: zhanggyb
 */

#ifndef _MARKERBASEDAR_MBARVIEWPORT_HPP_
#define _MARKERBASEDAR_MBARVIEWPORT_HPP_

#include <BlendInt/Gui/CVVideoViewport.hpp>

class MBARViewport: public BlendInt::CVVideoViewport
{
public:

	MBARViewport();

	virtual ~MBARViewport();

protected:

	virtual void ProcessImage (cv::Mat& image);

};

#endif /* _MARKERBASEDAR_MBARVIEWPORT_HPP_ */
