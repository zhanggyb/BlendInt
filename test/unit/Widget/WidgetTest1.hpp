#ifndef _WIDGETTEST1_HPP_
#define _WIDGETTEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Core/Object.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>

class WidgetTest1: public testing::Test
{
public:
	WidgetTest1 ();
	virtual ~WidgetTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _WIDGETTEST1_HPP_
