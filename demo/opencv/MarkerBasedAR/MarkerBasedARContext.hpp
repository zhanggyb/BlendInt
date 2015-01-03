/*
 * A context works in GLFW
 */

#ifndef _MARKERBASEDARCONTEXT_HPP_
#define _MARKERBASEDARCONTEXT_HPP_

#include <GLFW/glfw3.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVVideoViewport.hpp>
#include <BlendInt/Core/Timer.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>

#include "MBARViewport.hpp"

namespace BI=BlendInt;

class MarkerBasedARContext: public BI::Context
{
public:

	explicit MarkerBasedARContext (GLFWwindow* window);

	virtual ~MarkerBasedARContext ();

	virtual void SynchronizeWindow ();

	virtual void MakeGLContextCurrent ();

protected:

private:

	BI::ToolBox* CreateToolBoxOnce ();

	BI::ToolBox* CreateToolBarOnce ();

	void OnToggleCamera (BI::AbstractButton* sender, bool toggled);

	void OnPlay (BI::AbstractButton* sender);

	void OnPause (BI::AbstractButton* sender);

	void OnStop(BI::AbstractButton* sender);

	MBARViewport* viewport_;

	GLFWwindow* window_;

};

#endif /* _MARKERBASEDARCONTEXT_HPP_ */
