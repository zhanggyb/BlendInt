#ifndef _FILESELECTORTEST1_HPP_
#define _FILESELECTORTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class FileSelectorTest1: public testing::Test
{
public:
	FileSelectorTest1 ();
	virtual ~FileSelectorTest1 ();

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

#endif  // _FILESELECTORTEST1_HPP_
