/*
 * A context works in GLFW
 */

#ifndef _CARTOONIFIERCONTEXT_HPP_
#define _CARTOONIFIERCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/CVVideoViewport.hpp>
#include <BlendInt/Gui/Button.hpp>

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

	void OnPlay (BI::AbstractButton* sender);

	void OnPause (BI::AbstractButton* sender);

	void OnStop(BI::AbstractButton* sender);

	BI::CVVideoViewport* video_;
};

#endif /* _CARTOONIFIERCONTEXT_HPP_ */
