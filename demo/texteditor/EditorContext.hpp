/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/TextEntry.hpp>

#include <BlendInt/Gui/Context.hpp>

#include "FontView.hpp"

namespace BI=BlendInt;

class EditorContext: public BI::Context
{
public:

	EditorContext ();

	virtual ~EditorContext ();

private:

	void InitContext ();

	BI::TextEntry* m_vshader_file;
	BI::TextEntry* m_fshader_file;
	BI::Button* m_ok;
	//BI::Label* m_label;

	FontView* m_fontview;
};

#endif /* GLFWCONTEXT_HPP_ */
