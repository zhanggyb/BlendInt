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

#ifndef _BILO_SLIDER_HPP_
#define _BILO_SLIDER_HPP_

#include <BILO/AbstractSlider.hpp>

#include <BILO/GLBuffer.hpp>

namespace BILO {

	class SliderControl: public Widget
	{
	public:
		SliderControl (Drawable* parent = 0);
		virtual ~SliderControl ();

		void set_radius (unsigned int radius);

		unsigned int radius () const {return m_radius;}

	protected:

		virtual void update (int type);
		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		unsigned int m_radius;

		/**
		 * vertexes for drawing circle shape
		 */
		float m_vertexes[20][2];
		GLBuffer m_buffer;
		bool m_hover;

		bool m_pressed;

		static const float circle_vertexes[20][2];
	};

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

		/**
		 * @brief calculate the space width in which slider can move
		 * @return
		 */
		int get_space ();

		inline float get_position ()
		{return value() * get_space() / ((float)maximum() - (float)minimum());}

		SliderControl* m_slider_control;

		/**
		 * If left mouse button is pressed
		 */
		bool m_pressed;

		Coord2d m_press_pos;
	};
}

#endif // _BILO_SLIDER_HPP_
