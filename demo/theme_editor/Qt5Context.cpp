/*
 * Qt5Context.cpp
 */

#include "Qt5Context.hpp"
#include <QtGui/QCursor>

Qt5Context::Qt5Context (QWindow* window)
: BI::Context (), m_qt_window(window), m_current_cursor(BI::ArrowCursor), m_layout(0)
{
	m_layout = Manage(new MainLayout);
	m_layout->Resize(size());

	PushBack(m_layout);

	events()->connect(resized(), this , &Qt5Context::OnResizeLayout);
}

Qt5Context::~Qt5Context ()
{
}

void Qt5Context::SetCursor(int cursor_type)
{
	if(m_qt_window) {

		Qt::CursorShape qt_shape = Qt::ArrowCursor;

		switch(cursor_type) {

			case BI::ArrowCursor:
				qt_shape = Qt::ArrowCursor;
				break;

			case BI::UpArrowCursor:
				qt_shape = Qt::UpArrowCursor;
				break;

			case BI::CrossCursor:
				qt_shape = Qt::CrossCursor;
				break;

			case BI::WaitCursor:
				qt_shape = Qt::WaitCursor;
				break;

			case BI::IBeamCursor:
				qt_shape = Qt::IBeamCursor;
				break;

			case BI::SizeVerCursor:
				qt_shape = Qt::SizeVerCursor;
				break;

			case BI::SplitVCursor:
				qt_shape = Qt::SplitHCursor;
				break;

			case BI::SplitHCursor:
				qt_shape = Qt::SplitVCursor;
				break;

			default:
				break;
		}

		m_qt_window->setCursor(QCursor(qt_shape));
	}
}

int Qt5Context::GetCursor () const
{
	return m_current_cursor;
}

void Qt5Context::PushCursor (int cursor_type)
{
	m_cursor_stack.push(cursor_type);
}

int Qt5Context::PopCursor ()
{
	if(m_cursor_stack.empty()) {
		return BI::ArrowCursor;
	} else {
		int ret = m_cursor_stack.top();

		m_cursor_stack.pop();

		return ret;
	}
}

void Qt5Context::OnResizeLayout(const BI::Size& size)
{
	m_layout->Resize(size);
}
