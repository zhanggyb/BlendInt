#ifndef _PERFORMANCETEST1_HPP_
#define _PERFORMANCETEST1_HPP_

#include <gtest/gtest.h>
#include <BlendInt/Object.hpp>

class PerformanceTest1: public testing::Test
{
public:
	PerformanceTest1 ();
	virtual ~PerformanceTest1 ();

protected:

	virtual void SetUp ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}

	virtual void TearDown ()
	{
		ASSERT_TRUE(BlendInt::Object::CheckAllocatedObjects());
	}
};

#endif  // _PERFORMANCETEST1_HPP_
