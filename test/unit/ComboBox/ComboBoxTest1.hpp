#ifndef _COMBOBOXTEST1_HPP_
#define _COMBOBOXTEST1_HPP_

#include <gtest/gtest.h>
#include <Common/Window.hpp>
#include <BlendInt/Core/Object.hpp>

class ComboBoxTest1: public testing::Test
{
public:
	ComboBoxTest1 ();
	virtual ~ComboBoxTest1 ();

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

#endif  // _COMBOBOXTEST1_HPP_
