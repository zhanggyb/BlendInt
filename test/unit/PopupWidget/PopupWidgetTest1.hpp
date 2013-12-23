#ifndef _POPUPWIDGETTEST1_HPP_
#define _POPUPWIDGETTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class PopupWidgetTest1: public testing::Test
{
public:
	PopupWidgetTest1 ();
	virtual ~PopupWidgetTest1 ();

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

#endif  // _POPUPWIDGETTEST1_HPP_
