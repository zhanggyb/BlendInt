/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_ABSTRACTWIDGET_HPP_
#define _BLENDINT_GUI_ABSTRACTWIDGET_HPP_

#include <glm/glm.hpp>
#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractInteractiveForm.hpp>

namespace BlendInt {

	/**
	 * @brief A Normal widget
	 *
	 * @ingroup widgets
	 */
	class AbstractWidget: public AbstractInteractiveForm
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractWidget);

	public:

		AbstractWidget ();

		virtual ~AbstractWidget();

		Cpp::EventRef<AbstractWidget*> destroyed ()
		{
			return *destroyed_;
		}

		inline glm::vec2 get_relative_position (const glm::mat4& model_matrix)
		{
			return glm::vec2(model_matrix[0][0] * position().x() + model_matrix[1][0] * position().y(),
					model_matrix[0][1] * position().x() + model_matrix[1][1] * position().y());
		}

		inline glm::vec2 get_relative_position (const glm::mat2& model_matrix)
		{
			return model_matrix * glm::vec2 (position().x(), position().y());
		}

	protected:

		virtual bool PreDraw (Profile& profile);

		// virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		Cpp::ConnectionScope* events() const {return events_.get();}

	private:

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		boost::scoped_ptr<Cpp::Event<AbstractWidget*> > destroyed_;

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTWIDGET_HPP_ */
