#ifndef _IMAGEVIEWTEST1_HPP_
#define _IMAGEVIEWTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ImageViewTest1: public testing::Test
{
public:
	ImageViewTest1 ();
	virtual ~ImageViewTest1 ();

protected:

	virtual void SetUp ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}

	virtual void TearDown ()
	{
#ifdef DEBUG
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
#endif
	}
};

#endif  // _IMAGEVIEWTEST1_HPP_
