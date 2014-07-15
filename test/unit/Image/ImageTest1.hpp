#ifndef _IMAGETEST1_HPP_
#define _IMAGETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ImageTest1: public testing::Test
{
public:
	ImageTest1 ();
	virtual ~ImageTest1 ();

protected:

	virtual void SetUp ()
	{

	}

	virtual void TearDown ()
	{

	}
};

#endif  // _IMAGETEST1_HPP_
