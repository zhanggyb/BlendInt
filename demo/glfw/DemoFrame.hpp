/*
 * DemoFrame.hpp
 *
 *  Created on: 2013年11月16日
 *      Author: zhanggyb
 */

#ifndef DEMOFRAME_HPP_
#define DEMOFRAME_HPP_

#include <BlendInt/Frame.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/Button.hpp>

using namespace BlendInt;

class DemoFrame: public Frame
{
public:

	DemoFrame ();

	virtual ~DemoFrame();

private:

	HorizontalLayout* m_hlayout;

};

#endif /* DEMOFRAME_HPP_ */
