/**
 * Main Layout
 */

#ifndef _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_
#define _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_

#include <BlendInt/Gui/VLayout.hpp>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/CVImageView.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/Expander.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/Button.hpp>

namespace BI = BlendInt;

class MainLayout: public BI::VLayout
{
public:

	MainLayout ();
	
	virtual ~MainLayout ();

private:

	void InitOnce ();

	void OnOpenClick ();

	void OnResize (AbstractWidget* context, int type);

	BI::MenuBar* CreateMenuBar ();

	BI::Expander* CreateExpander ();

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;
	BI::CVImageView* m_imgview;

	BI::TextEntry* m_input;
	BI::ComboBox* m_combo;
	BI::Button* m_tool_open;
};

#endif
