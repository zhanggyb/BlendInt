/**
 * Main Layout
 */

#ifndef _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_
#define _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_

#include <BlendInt/Gui/VLayout.hpp>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Tab.hpp>
#include <BlendInt/Gui/FileButton.hpp>

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

	BI::ToolBar* CreateBottomBar ();

	BI::Tab* CreateTab ();

	void OnFileSelected ();

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;

	BI::ToolButton* m_tool_open;

	BI::Viewport3D* m_scene;

	BI::TextEntry* m_file_input;
	BI::FileButton* m_file_button;

	BI::Button* m_btn_open;
};

#endif
