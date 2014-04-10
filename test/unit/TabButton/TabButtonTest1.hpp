#ifndef _TABBUTTONTEST1_HPP_
#define _TABBUTTONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class TabButtonTest1: public testing::Test
{
public:
	TabButtonTest1 ();
	virtual ~TabButtonTest1 ();

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

#endif  // _TABBUTTONTEST1_HPP_
