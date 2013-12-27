#ifndef _WIDGETTEST1_HPP_
#define _WIDGETTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class WidgetTest1: public testing::Test
{
public:
	WidgetTest1 ();
	virtual ~WidgetTest1 ();

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

#endif  // _WIDGETTEST1_HPP_
