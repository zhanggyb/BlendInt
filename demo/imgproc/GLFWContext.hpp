/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>

#include "MainLayout.hpp"

namespace BI=BlendInt;

class GLFWContext: public BI::Context
{
public:
	GLFWContext ();

	virtual ~GLFWContext ();

protected:

private:

	void OnResizeLayout (const BI::Size& size);

	MainLayout* m_layout;

};

#endif /* GLFWCONTEXT_HPP_ */
