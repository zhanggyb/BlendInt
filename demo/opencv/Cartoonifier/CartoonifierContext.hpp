/*
 * A context works in GLFW
 */

#ifndef _CARTOONIFIERCONTEXT_HPP_
#define _CARTOONIFIERCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

namespace BI=BlendInt;

class CartoonifierContext: public BI::Context
{
public:
	CartoonifierContext ();

	virtual ~CartoonifierContext ();

protected:

private:

	BI::ToolBox* CreateToolBoxOnce ();

	void OnResizeLayout (const BI::Size& size);
};

#endif /* _CARTOONIFIERCONTEXT_HPP_ */
