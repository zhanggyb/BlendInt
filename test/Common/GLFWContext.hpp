/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>

namespace BI=BlendInt;

class GLFWContext: public BI::Context
{
public:
	GLFWContext ();

	virtual ~GLFWContext ();

};


#endif /* GLFWCONTEXT_HPP_ */
