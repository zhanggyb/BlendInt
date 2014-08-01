#ifndef _CVIMAGEVIEWTEST1_HPP_
#define _CVIMAGEVIEWTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class CVImageViewTest1: public testing::Test
{
public:
	CVImageViewTest1 ();
	virtual ~CVImageViewTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _CVIMAGEVIEWTEST1_HPP_
