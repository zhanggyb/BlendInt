/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_INTERFACE_HPP_
#define _BIL_INTERFACE_HPP_

#include <BIL/Drawable.hpp>

namespace BIL {

	/**
	 * @brief The main entry for this library
	 */
	class Interface
	{
	public:

		static Interface* create ();

		bool initialize ();

		void terminate ();

		void render ();

		void resizeEvent (int width, int height);

		void keyEvent (int key, int scancode, int action, int mods);

		void mouseButtonEvent (int button, int action, int mods);

		void cursorPosEvent (double xpos, double ypos);

	private:

		Interface ();

		~Interface ();

		void render (Drawable* obj);

#ifdef DEBUG
		void drawGrid (int width, int height);
#endif

		double cursor_pos_x_;	/** cursor x position */
		double cursor_pos_y_;	/** cursor y position */

		Size size_;

		static Interface* interface;

	};
}


#endif /* _BIL_INTERFACE_HPP_ */
