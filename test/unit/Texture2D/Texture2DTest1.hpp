#ifndef _TEXTURE2DTEST1_HPP_
#define _TEXTURE2DTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class Texture2DTest1: public testing::Test
{
public:
	Texture2DTest1 ();
	virtual ~Texture2DTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
	}
};

#endif  // _TEXTURE2DTEST1_HPP_
