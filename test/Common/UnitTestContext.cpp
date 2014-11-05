/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "UnitTestContext.hpp"

UnitTestContext::UnitTestContext()
: BI::Context()
{

}

UnitTestContext::~UnitTestContext ()
{

}

void UnitTestContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}
