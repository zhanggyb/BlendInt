#ifndef _TEXTENTRYTEST1_HPP_
#define _TEXTENTRYTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class TextEntryTest1: public testing::Test
{
public:
	TextEntryTest1 ();
	virtual ~TextEntryTest1 ();

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

#endif  // _TEXTENTRYTEST1_HPP_
