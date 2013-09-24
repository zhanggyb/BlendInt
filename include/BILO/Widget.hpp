/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_WIDGET_H_
#define _BIL_WIDGET_H_

#include <BILO/Drawable.hpp>
#include <BILO/Theme.hpp>
#include <BILO/Color.hpp>
#include <BILO/Theme.hpp>
#include <BILO/Rect.hpp>
#include <BILO/GLBuffer.hpp>

#include <Cpp/Events.hpp>

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

/* used for transp checkers */
#define UI_TRANSP_DARK 100
#define UI_TRANSP_LIGHT 160

#define UI_TEXT_MARGIN_X 0.4f

namespace BILO {

	class Widget: public Drawable
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		Widget (Drawable* parent);

		virtual ~Widget ();

	protected:

		virtual void update (int property);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		/**
		 * @brief get the GLBuffer
		 * @return
		 */
		GLBuffer& buffer () {return m_buffer;}

		Cpp::ConnectionScope m_events;

		static const float quarter_corner_vertexes[9][2];

		static const float circle_vertexes[20][2];

		GLBuffer m_buffer;

	private:

		void update_shape ();

		/**
		 * vertexes for drawing shape
		 */
		float m_vertexes[36][2];

	};

} /* namespace BILO */
#endif /* _BIL_WIDGET_H_ */
