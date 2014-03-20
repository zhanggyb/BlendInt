#ifndef _TEXTUREFONTTEST1_HPP_
#define _TEXTUREFONTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class TextureFontTest1: public testing::Test
{
public:
	TextureFontTest1 ();
	virtual ~TextureFontTest1 ();

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

#endif  // _TEXTUREFONTTEST1_HPP_
