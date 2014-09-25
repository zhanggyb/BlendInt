/*
 * GLFWContext.cpp
 */

#include "GLFWContext.hpp"
#include <BlendInt/Gui/Screen.hpp>

GLFWContext::GLFWContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainLayout);
	m_layout->Resize(size());

	BI::Screen* screen = BI::Manage(new BI::Screen);
	screen->Setup(m_layout);

	AddScreen(screen);
	//Append(m_layout);

	events()->connect(resized(), screen, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::Screen::Resize));
	events()->connect(resized(), this , &GLFWContext::OnResizeLayout);
}

GLFWContext::~GLFWContext ()
{

}

void GLFWContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
