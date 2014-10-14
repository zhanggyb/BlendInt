/*
 * GLFWContext.cpp
 */

#include "GLFWContext.hpp"
#include <BlendInt/Gui/SingleFrame.hpp>

GLFWContext::GLFWContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainLayout);
	m_layout->Resize(size());

	BI::SingleFrame* frame = BI::Manage(new BI::SingleFrame);
	frame->Setup(m_layout);

	AddFrame(frame);
	//Append(m_layout);

	events()->connect(resized(), frame, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::SingleFrame::Resize));
	events()->connect(resized(), this , &GLFWContext::OnResizeLayout);
}

GLFWContext::~GLFWContext ()
{

}

void GLFWContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
