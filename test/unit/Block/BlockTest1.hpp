#ifndef _BLOCKTEST1_HPP_
#define _BLOCKTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class BlockTest1: public testing::Test
{
public:
	BlockTest1 ();
	virtual ~BlockTest1 ();

protected:

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}
};

#endif  // _BLOCKTEST1_HPP_
