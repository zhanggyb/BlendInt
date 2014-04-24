/*
 * A context in Qt5
 */

#ifndef QT5CONTEXT_HPP_
#define QT5CONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <QGuiApplication>
#include <QtGui/QWindow>

#include "MainLayout.hpp"

namespace BI=BlendInt;

class Qt5Context: public BI::Context
{
public:
	Qt5Context ();

	virtual ~Qt5Context ();

	// overload cursor functions

private:

	void OnResizeLayout (const BI::Size& size);

	Qt::CursorShape m_cursor;

	MainLayout* m_layout;
};

#endif /* QT5CONTEXT_HPP_ */
