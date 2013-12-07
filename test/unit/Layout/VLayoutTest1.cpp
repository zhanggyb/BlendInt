/*
 * VLayoutTest1.cpp
 *
 *  Created on: 2013Äê12ÔÂ7ÈÕ
 *      Author: zhanggy
 */

#include "VLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/VerticalLayout.hpp>

using namespace BlendInt;

VLayoutTest1::VLayoutTest1()
: testing::Test()
{
	// TODO Auto-generated constructor stub

}

VLayoutTest1::~VLayoutTest1() {
	// TODO Auto-generated destructor stub
}

// test default Add() method
TEST_F(VLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Vertical Layout Test");

	// add test code here

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
