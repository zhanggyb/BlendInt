/*
 * GLFWContext.cpp
 */

#include "CVContext.hpp"

#include <BlendInt/Gui/SingleFrame.hpp>

using namespace BlendInt;

CVContext::CVContext()
: BI::Context(), m_layout(0)
{
	activate_events();

	m_layout = Manage (new MainSpace);
	m_layout->Resize(size());

	SingleFrame* frame = Manage(new SingleFrame);
	frame->Setup(m_layout);

	AddFrame(frame);

	events()->connect(resized(), frame, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::SingleFrame::Resize));

}

CVContext::~CVContext ()
{

}

void CVContext::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
