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

#ifndef _BILO_WIDGET_HPP_
#define _BILO_WIDGET_HPP_

#include <BILO/Drawable.hpp>
#include <BILO/Theme.hpp>
#include <BILO/Color.hpp>
#include <BILO/Theme.hpp>
#include <BILO/Rect.hpp>
#include <BILO/GLBuffer.hpp>

#include <Cpp/Events.hpp>

namespace BILO {

	class Widget: public Drawable
	{
		DISALLOW_COPY_AND_ASSIGN(Widget);

	public:

		Widget ();

		Widget (Drawable* parent);

		virtual ~Widget ();

	protected:

		virtual bool update (int type, const void* property);

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

		GLBuffer m_buffer;

		static const float quarter_corner_vertexes[9][2];

	private:

		void update_shape ();

		/**
		 * @brief draw a round box when render
		 */
		void draw_roundbox (int mode);

		void draw_box (int mode, float minx, float miny, float maxx, float maxy);

		/**
		 * vertexes for drawing shape
		 */
		//float m_vertexes[36][2];

	};

} /* namespace BILO */
#endif /* _BIL_WIDGET_H_ */
