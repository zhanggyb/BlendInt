#ifndef _LISTVIEWTEST1_HPP_
#define _LISTVIEWTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ListViewTest1: public testing::Test
{
public:
	ListViewTest1 ();
	virtual ~ListViewTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _LISTVIEWTEST1_HPP_
