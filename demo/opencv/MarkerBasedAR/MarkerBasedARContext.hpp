/*
 * A context works in GLFW
 */

#ifndef _MARKERBASEDARCONTEXT_HPP_
#define _MARKERBASEDARCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVVideoViewport.hpp>

namespace BI=BlendInt;

class MarkerBasedARContext: public BI::Context
{
public:
	MarkerBasedARContext ();

	virtual ~MarkerBasedARContext ();

	virtual void SynchronizeWindow ();

protected:

private:

	BI::ToolBox* CreateToolBoxOnce ();

	BI::ToolBox* CreateToolBarOnce ();

	void OnPlay ();

	void OnPause ();

	void OnStop();

	BI::CVVideoViewport* video_;
};

#endif /* _MARKERBASEDARCONTEXT_HPP_ */
