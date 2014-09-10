/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>

#include "MainSpace.hpp"

namespace BI=BlendInt;

class CVContext: public BI::Context
{
public:
	CVContext ();

	virtual ~CVContext ();

protected:

private:

	void OnResizeLayout (const BI::Size& size);

	MainSpace* m_layout;

};

#endif /* GLFWCONTEXT_HPP_ */
