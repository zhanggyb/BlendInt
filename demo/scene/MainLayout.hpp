/**
 * Main Layout
 */

#ifndef _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_
#define _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_

#include <BlendInt/Gui/VBox.hpp>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Tab.hpp>

namespace BI = BlendInt;

class MainLayout: public BI::VBox
{
public:

	MainLayout ();

	virtual ~MainLayout ();

private:

	void InitOnce ();

	void OnOpenClick ();

	void OnResize (AbstractWidget* context, int type);

	BI::ToolBar* CreateToolBar ();

	BI::MenuBar* CreateMenuBar ();

	BI::ToolBox* CreateToolBox ();

	BI::Expander* CreateExpander ();

	BI::Tab* CreateTab ();

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;

	BI::TextEntry* m_input;
	BI::ToolButton* m_open;

	BI::Viewport3D* m_scene;
};

#endif
