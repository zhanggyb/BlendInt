#ifndef _LISTMODELTEST1_HPP_
#define _LISTMODELTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ListModelTest1: public testing::Test
{
public:
	ListModelTest1 ();
	virtual ~ListModelTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _LISTMODELTEST1_HPP_
