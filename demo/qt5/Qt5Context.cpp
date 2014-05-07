/*
 * Qt5Context.cpp
 */

#include "Qt5Context.hpp"
#include <QtGui/QCursor>

Qt5Context::Qt5Context (QWindow* window)
: BI::Context (), m_qt_window(window), m_current_cursor(BI::ArrowCursor),
  m_view3d(0), m_ok(0)
{
	m_layout.reset(new BI::VLayoutExt(this));

	if(m_layout) {
		DBG_PRINT_MSG("%s", "layout valid");
	}

	m_view3d = Manage(new BI::Viewport3D);
	m_view3d->set_name("Viewport3D");
	m_view3d->Resize(630, 445);
	m_view3d->SetPosition(5, 30);

	m_ok = Manage(new BI::Button("OK"));
	m_ok->set_name("OK");
	m_ok->SetExpandX(false);

	Add(m_view3d);
	Add(m_ok);

	m_layout->Add(m_view3d);
	m_layout->Add(m_ok);
	m_layout->Fill();

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
	//m_layout->Resize(size);
	m_layout->Fill();
}
