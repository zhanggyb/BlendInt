/*
 * GLFWContext.cpp
 */

#include "GLFWContext.hpp"

#include <BlendInt/Gui/SingleFrame.hpp>

using namespace BI;

GLFWContext::GLFWContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainLayout);
	m_layout->Resize(size());

	SingleFrame* screen = Manage(new SingleFrame);
	screen->Resize(m_layout->size());
	screen->Setup(m_layout);

	AddFrame(screen);

	events()->connect(resized(), this , &GLFWContext::OnResizeLayout);
}

GLFWContext::~GLFWContext ()
{

}

void GLFWContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
