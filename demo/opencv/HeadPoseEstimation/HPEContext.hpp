/*
 * A context works in GLFW
 */

#ifndef _HPECONTEXT_HPP_
#define _HPECONTEXT_HPP_

#include <GLFW/glfw3.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/ImageViewport.hpp>
#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/Panel.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>

#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/ButtonGroup.hpp>

namespace BI=BlendInt;

class HPEContext: public BI::Context
{
public:

	HPEContext ();

	virtual ~HPEContext ();

	virtual void SynchronizeWindow ();

protected:

private:

	enum Status {
		VideoPlay,
		VideoPause,
		VideoStop
	};

	BI::ToolBox* CreateToolBarOnce ();

	BI::Panel* CreateButtons ();

	BI::Workspace* CreateWorkspaceOnce ();

	BI::Workspace* CreateToolsOnce ();

	BI::ToolBox* CreateRadios ();

	bool OpenCamera (int n, const BI::Size& resolution = BI::Size(640, 480));

	BI::Viewport* viewport_3d_;

	BI::RefPtr<BI::ButtonGroup> radio_group_;
};

#endif /* _HPECONTEXT_HPP_ */
