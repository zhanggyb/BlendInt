#ifndef _PANELTEST1_HPP_
#define _PANELTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class PanelTest1: public testing::Test
{
public:
	PanelTest1 ();
	virtual ~PanelTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _PANELTEST1_HPP_
