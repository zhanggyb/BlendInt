/**
 * Main Layout
 */

#ifndef _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_
#define _BLENDINT_DEMO_IMGPROC_MAIN_LAYOUT_HPP_

#include <BlendInt/Gui/VBox.hpp>

#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/CVImageView.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/ScrollArea.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/TextEntry.hpp>
#include <BlendInt/Gui/ToolButton.hpp>

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

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;
	BI::ImageView* m_imgview;
	BI::ScrollArea* m_area;

	BI::TextEntry* m_input;
	BI::ComboBox* m_combo;
	BI::ToolButton* m_open;

};

#endif
