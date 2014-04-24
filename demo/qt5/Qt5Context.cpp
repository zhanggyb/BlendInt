/*
 * Qt5Context.cpp
 */

#include "Qt5Context.hpp"
#include <QtGui/QCursor>

Qt5Context::Qt5Context ()
: BI::Context (), m_cursor(Qt::ArrowCursor), m_layout(0)
{
	m_layout = Manage (new MainLayout);
	m_layout->Resize(size());
	Add(m_layout);

	events()->connect(resized(), this , &Qt5Context::OnResizeLayout);
}

Qt5Context::~Qt5Context ()
{
}

void Qt5Context::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
