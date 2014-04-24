#ifndef _WIDGETTEST1_HPP_
#define _WIDGETTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

class WidgetTest1: public testing::Test
{
public:
	WidgetTest1 ();
	virtual ~WidgetTest1 ();

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

#endif  // _WIDGETTEST1_HPP_
