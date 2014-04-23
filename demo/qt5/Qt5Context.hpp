/*
 * A context in Qt5
 */

#ifndef QT5CONTEXT_HPP_
#define QT5CONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <QGuiApplication>
#include <QtGui/QWindow>

namespace BI=BlendInt;

class Qt5Context: public BI::Context
{
public:
	Qt5Context ();

	virtual ~Qt5Context ();

	// overload cursor functions

private:

	Qt::CursorShape m_cursor;

};

#endif /* QT5CONTEXT_HPP_ */
