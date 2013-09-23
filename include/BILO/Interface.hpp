/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_INTERFACE_HPP_
#define _BIL_INTERFACE_HPP_

#include <BILO/Types.hpp>
#include <BILO/Size.hpp>

namespace BILO {

	class Drawable;
	class KeyEvent;
	class MouseEvent;

	/**
	 * @brief The main entry for this library
	 */
	class Interface
	{
	public:

		static Interface* instance ();

		static bool initialize ();

		static void release ();

		bool bind (Drawable* object);

		bool unbind (Drawable* object);

		void render ();

		void resizeEvent (int width, int height);

		void keyEvent (int key, int scancode, int action, int mods);

		void mouseButtonEvent (int button, int action, int mods);

		void cursorPosEvent (double xpos, double ypos);

		const Size& size () const;

		void resize (int width, int height);

		void resize (const Size& size);

		void dispatch_render_event (int layer, Drawable* obj);

		void dispatch_key_press_event (Drawable* obj, KeyEvent* event);

		void dispatch_mouse_press_event (Drawable* obj, MouseEvent* event);

		void dispatch_mouse_release_event (Drawable* obj, MouseEvent* event);

		void dispatch_mouse_move_event (Drawable* obj, MouseEvent* event);

	private:

		Interface ();

		~Interface ();

		double cursor_pos_x_;	/** cursor x position */
		double cursor_pos_y_;	/** cursor y position */

		Size size_;

		unsigned char m_ticktack;

		static Interface* interface;

#ifdef DEBUG
		void draw_grid (int width, int height);
#endif

	};
}


#endif /* _BIL_INTERFACE_HPP_ */
