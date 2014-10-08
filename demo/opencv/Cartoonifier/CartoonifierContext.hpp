/*
 * A context works in GLFW
 */

#ifndef _CARTOONIFIERCONTEXT_HPP_
#define _CARTOONIFIERCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>

namespace BI=BlendInt;

class CartoonifierContext: public BI::Context
{
public:
	CartoonifierContext ();

	virtual ~CartoonifierContext ();

protected:

private:

	void OnResizeLayout (const BI::Size& size);
};

#endif /* _CARTOONIFIERCONTEXT_HPP_ */
