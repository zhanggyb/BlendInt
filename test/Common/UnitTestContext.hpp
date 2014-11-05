/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>

namespace BI=BlendInt;

class UnitTestContext: public BI::Context
{
public:
	UnitTestContext ();

	virtual ~UnitTestContext ();

	virtual void SynchronizeWindow ();

};


#endif /* GLFWCONTEXT_HPP_ */
