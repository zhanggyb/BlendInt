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

void UnitTestContext::LinkResizeEvent(BI::AbstractFrame* frame)
{
	if(frame)
		events()->connect(resized(), frame, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::AbstractFrame::Resize));
}

void UnitTestContext::UnlinkResizeEvent(BI::AbstractFrame* frame)
{
	if(frame)
		resized().disconnectOne(frame, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::AbstractFrame::Resize));
}
