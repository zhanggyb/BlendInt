#ifndef _HLAYOUTTEST1_HPP_
#define _HLAYOUTTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>

class HLayoutTest1: public testing::Test
{
    protected:
       
        virtual void SetUp ()
        {
			ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
        }

        virtual void TearDown ()
        {
			ASSERT_TRUE(BlendInt::CheckAllocatedObjects());
        }
};

#endif  // _HLAYOUTTEST1_HPP_

