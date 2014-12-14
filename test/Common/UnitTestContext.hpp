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

	void LinkResizeEvent (BI::AbstractFrame* frame);

	void UnlinkResizeEvent (BI::AbstractFrame* frame);

	virtual void SynchronizeWindow ();

};


#endif /* GLFWCONTEXT_HPP_ */
