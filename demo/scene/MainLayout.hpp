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
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Gui/Label.hpp>

#include <BlendInt/Gui/ImagePlane2D.hpp>

namespace BI = BlendInt;

class MainLayout: public BI::VLayout
{
public:

	MainLayout ();

	virtual ~MainLayout ();

protected:

	virtual void PerformSizeUpdate (const BI::SizeUpdateRequest& request);

	virtual BI::ResponseType Draw (BI::Profile& profile);

private:

	void RenderToBuffer (BI::Profile& profile);

	void InitOnce ();

	void OnOpenClick ();

	void OnResize (AbstractWidget* context, int type);

	BI::ToolBar* CreateToolBar ();

	BI::MenuBar* CreateMenuBar ();

	//BI::ToolBox* CreateToolBox ();

	BI::Expander* CreateTransformExpander ();

	BI::Expander* CreateLightExpander ();

	BI::Expander* CreateColorExpander ();

	BI::ToolBar* CreateBottomBar ();

	BI::Tab* CreateTab ();

	void OnFileSelected ();

	BI::MenuBar* m_menubar;
	BI::ToolBar* m_toolbar;

	BI::ToolButton* m_tool_open;

	BI::Tab* m_tab;

	BI::Viewport3D* m_scene;
	BI::TextureView* m_image_view;

	BI::TextEntry* m_file_input;
	BI::FileButton* m_file_button;

	BI::Button* m_btn_open;

	BI::Label* m_msg_label;

	BI::ImagePlane2D buffer_;
};

#endif
