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
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_CLOCKWIDGET_HPP_
#define _BLENDINT_CLOCKWIDGET_HPP_

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Core/Timer.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	/**
	 * @brief A widget to show a clock
	 *
	 * This widget is used for demo or test also.
	 */
	class Clock: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Clock);

	public:

		Clock();

		virtual ~Clock();

		void set_background (const Color& color)
		{
			m_background = color;
		}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void GenerateClockVertices (int radius, float border,
		        std::vector<GLfloat>& inner_vertices,
		        std::vector<GLfloat>& outer_vertices);

	private:

		void UpdateClockHands();

		void InitializeClock ();

		GLuint m_vao[2];

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;

		int m_angle;

		RefPtr<Timer> m_timer;

		Color m_background;
	};

}

#endif /* _BLENDINT_CLOCKWIDGET_HPP_ */
