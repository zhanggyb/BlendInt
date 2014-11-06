/*
 * A context works in GLFW
 */

#ifndef _CARTOONIFIERCONTEXT_HPP_
#define _CARTOONIFIERCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVVideoViewport.hpp>

namespace BI=BlendInt;

class CartoonifierContext: public BI::Context
{
public:
	CartoonifierContext ();

	virtual ~CartoonifierContext ();

	virtual void SynchronizeWindow ();

protected:

private:

	BI::ToolBox* CreateToolBoxOnce ();

	void OnPlay ();

	void OnPause ();

	void OnStop();

	BI::CVVideoViewport* video_;
};

#endif /* _CARTOONIFIERCONTEXT_HPP_ */
