#ifndef _SECTIONTEST1_HPP_
#define _SECTIONTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class SectionTest1: public testing::Test
{
public:
	SectionTest1 ();
	virtual ~SectionTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _SECTIONTEST1_HPP_
