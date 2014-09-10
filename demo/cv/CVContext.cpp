/*
 * GLFWContext.cpp
 */

#include "CVContext.hpp"

CVContext::CVContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainSpace);
	m_layout->Resize(size());

	Append(m_layout);

	events()->connect(resized(), this , &CVContext::OnResizeLayout);
}

CVContext::~CVContext ()
{

}

void CVContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
