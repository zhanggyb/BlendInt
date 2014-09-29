/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/StaticFrame.hpp>
#include <BlendInt/Gui/Frame.hpp>

namespace BI=BlendInt;

class TexBufContext: public BI::Context
{
public:
	TexBufContext ();

	virtual ~TexBufContext ();

protected:

private:

	void CreateWidgets ();

	void OnTakeScreenShot ();

	BI::Button* m_button;

	BI::Panel* m_panel1;
	
	BI::StaticPanel* m_panel2;
};

#endif /* GLFWCONTEXT_HPP_ */
