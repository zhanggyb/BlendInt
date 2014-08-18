#ifndef _HBLOCKTEST1_HPP_
#define _HBLOCKTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class HBlockTest1: public testing::Test
{
public:
	HBlockTest1 ();
	virtual ~HBlockTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _HBLOCKTEST1_HPP_
