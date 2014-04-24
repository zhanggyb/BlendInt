/*
 * A context works in GLFW
 */

#ifndef GLFWCONTEXT_HPP_
#define GLFWCONTEXT_HPP_

#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BI=BlendInt;

class EditorContext: public BI::Context
{
public:

		EditorContext ();

		virtual ~EditorContext ();

private:

		void InitContext ();

		BI::Button* m_ok;
		BI::Label* m_label;

};

#endif /* GLFWCONTEXT_HPP_ */
