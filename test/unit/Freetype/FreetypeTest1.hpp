#ifndef _FREETYPETEST1_HPP_
#define _FREETYPETEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class FreetypeTest1: public testing::Test
{
public:
	FreetypeTest1 ();
	virtual ~FreetypeTest1 ();

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

#endif  // _FREETYPETEST1_HPP_
