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

#include <BlendInt/OpenGL/TextureBuffer.hpp>

#include <BlendInt/Gui/Viewport2D.hpp>

namespace BI = BlendInt;

class MainSpace: public BI::VLayout
{
public:

	MainSpace ();

	virtual ~MainSpace ();

protected:

	virtual void PerformRefresh (const BI::RefreshRequest& request);

	virtual void PerformSizeUpdate (const BI::SizeUpdateRequest& request);

	virtual BI::ResponseType Draw (BI::Profile& profile);

private:

	void RenderToBuffer ();

	void InitOnce ();

	void OnResize (AbstractWidget* context, int type);

	BI::ToolBar* CreateToolBar ();

	BI::ToolBox* CreateSideBox ();

	BI::Expander* CreateTransformExpander ();

	BI::Expander* CreateLightExpander ();

	BI::Expander* CreateColorExpander ();

	BI::ToolBar* CreateBottomBar ();

	BI::ToolBar* m_toolbar;

	BI::Viewport2D* image_viewport_;

	BI::Label* message_;

	bool refresh_;

	BI::TextureBuffer buffer_;
};

#endif
