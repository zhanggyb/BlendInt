#ifndef _WIDGETTEST1_HPP_
#define _WIDGETTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Window.hpp>
#include <BlendInt/Object.hpp>

class WidgetTest1: public testing::Test
{
public:
	WidgetTest1 ();
	virtual ~WidgetTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}
};

#endif  // _WIDGETTEST1_HPP_
