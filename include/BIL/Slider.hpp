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

#ifndef _BIL_SLIDER_HPP_
#define _BIL_SLIDER_HPP_

#include <BIL/AbstractSlider.hpp>

#include <BIL/GLBuffer.hpp>

namespace BIL {

	/**
	 * @brief Slider widget
	 */
	class Slider: public AbstractSlider
	{
	public:

		Slider (Orientation orientation, Drawable* parent = 0);

		virtual ~Slider ();

	protected:

		virtual void update (int property);

		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		void update_shape ();

		/**
		 * vertexes for drawing circle shape
		 */
		float m_vertexes[20][2];

		GLBuffer m_buffer;

		/**
		 * If the mouse cursor is on slider circle
		 */
		bool m_hover;

		/**
		 * If left mouse button is pressed
		 */
		bool m_pressed;

		static const float circle_vertexes[20][2];

	};
}

#endif /* _BIL_SLIDER_HPP_ */
