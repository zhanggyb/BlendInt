/*
 * GLFWContext.cpp
 */

#include "GLFWContext.hpp"
#include <BlendInt/Gui/Frame.hpp>

GLFWContext::GLFWContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainLayout);
	m_layout->Resize(size());

	BI::Frame* frame = BI::Manage(new BI::Frame);
	frame->AddWidget(m_layout);

	AddFrame(frame);
	//Append(m_layout);

	events()->connect(resized(), frame, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::Frame::Resize));
	events()->connect(resized(), this , &GLFWContext::OnResizeLayout);
}

GLFWContext::~GLFWContext ()
{

}

void GLFWContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
