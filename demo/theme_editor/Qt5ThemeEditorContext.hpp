/*
 * A context in Qt5
 */

#ifndef QT5CONTEXT_HPP_
#define QT5CONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <QGuiApplication>
#include <QtGui/QWindow>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/Expander.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Workspace.hpp>

#include <BlendInt/Gui/Panel.hpp>
#include <BlendInt/Gui/StaticPanel.hpp>

namespace BI=BlendInt;

class Qt5ThemeEditorContext: public BI::Context
{
public:
	Qt5ThemeEditorContext (QWindow* window);

	virtual ~Qt5ThemeEditorContext ();

	// overload cursor functions
	virtual void SetCursor (int cursor_type);

private:

	void Initialize ();

	BI::StaticPanel* CreateButtonPanel ();

	BI::StaticPanel* CreateColorPanel ();

	QWindow* window_;
};

#endif /* QT5CONTEXT_HPP_ */
