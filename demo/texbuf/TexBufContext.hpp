/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/FramePanel.hpp>
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

	BI::Frame* m_panel;
	//BI::FramePanel* m_panel;
	
	BI::FramePanel* view3d_frame_;
	
	BI::FramePanel* label_frame_;
	
	BI::FramePanel* toggle_frame_;
	
};

#endif /* GLFWCONTEXT_HPP_ */
