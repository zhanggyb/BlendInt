/*
 * A context in Qt5
 */

#ifndef QT5CONTEXT_HPP_
#define QT5CONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <QGuiApplication>
#include <QtGui/QWindow>

#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/Button.hpp>

#include "MainLayout.hpp"

namespace BI=BlendInt;

class Qt5Context: public BI::Context
{
public:
	Qt5Context (QWindow* window);

	virtual ~Qt5Context ();

	// overload cursor functions
	virtual void SetCursor (int cursor_type);

private:

	void OnResizeLayout (const BI::Size& size);

	QWindow* window_;

	MainLayout* m_layout;
};

#endif /* QT5CONTEXT_HPP_ */
