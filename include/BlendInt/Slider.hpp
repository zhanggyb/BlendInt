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

#ifndef _BLENDINT_SLIDER_HPP_
#define _BLENDINT_SLIDER_HPP_

#include <BlendInt/AbstractSlider.hpp>

namespace BlendInt {

	class SliderControl: public Widget
	{
	public:

		SliderControl ();

		SliderControl (Drawable* parent);

		virtual ~SliderControl ();

		bool pressed () const {return m_pressed;}

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		void update_shape (const Size* size);

		bool m_hover;

		bool m_pressed;

		Point m_move_start;

		Point m_position_origin;
	};

	/**
	 * @brief Slider widget
	 */
	class Slider: public AbstractSlider
	{
	public:

		Slider (Orientation orientation = Horizontal);

		Slider (Orientation orientation, Drawable* parent);

		virtual ~Slider ();

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		SliderControl* control_widget ()
		{
			return m_slider_control;
		}

	private:

		/**
		 * @brief calculate the space width in which slider can move
		 * @return
		 */
		int get_space ();

		inline float get_position ()
		{return value() * get_space() / ((float)maximum() - (float)minimum());}

		SliderControl* m_slider_control;
	};

}

#endif // _BLENDINT_SLIDER_HPP_
