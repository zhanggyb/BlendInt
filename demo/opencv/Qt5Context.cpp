/*
 * Qt5Context.cpp
 */

#include "Qt5Context.hpp"
#include <QtGui/QCursor>

#include <BlendInt/Gui/Screen.hpp>

using namespace BI;

Qt5Context::Qt5Context (QWindow* window)
: BI::Context (), window_(window), m_layout(0)
{
	m_layout = Manage(new MainLayout);
	m_layout->Resize(size());

	Screen* screen1 = Manage(new Screen);
	screen1->Resize(m_layout->size());
	screen1->Setup(m_layout);
	AddScreen(screen1);

	events()->connect(resized(), this , &Qt5Context::OnResizeLayout);
}

Qt5Context::~Qt5Context ()
{
}

void Qt5Context::SetCursor(int cursor_type)
{
	if(window_) {

		window_->setCursor(QCursor(static_cast<Qt::CursorShape>(cursor_type)));
	}
}

void Qt5Context::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
