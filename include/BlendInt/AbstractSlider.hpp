/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_ABSTRACTSLIDER_HPP_
#define _BIL_ABSTRACTSLIDER_HPP_

#include <BlendInt/Widget.hpp>
#include <BlendInt/Types.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief The abstract class for slider widgets
	 */
	class AbstractSlider: public Widget
	{

	public:

		enum SliderPropertyTypeExt {
			SliderPropertyOrientation = WidgetPropertyLast + 1,
			SliderPropertyValue,
			SliderPropertyMinimum,
			SliderPropertyMaximum,
			SliderPropertyStep
		};

		AbstractSlider(Orientation orientation = Horizontal);

		AbstractSlider(Orientation orientation, Drawable* parent);

		virtual ~AbstractSlider ();

		void set_range (int value1, int value2);

		void set_minimum (int minimum);
		
		int minimum () const {return m_minimum;}

		void set_maximum (int maximum);

		int maximum () const {return m_maximum;}

		void set_value (int value);

		int value () const {return m_value;}
		
		void set_step (int step);

		int step () const {return m_step;}

		void set_orientation (Orientation orientation);

		Orientation orientation () const {return m_orientation;}

		Cpp::EventRef<int> slider_moved () {return m_slider_moved;}

		Cpp::EventRef<> slider_pressed () {return m_slider_pressed;}

		Cpp::EventRef<> slider_released () {return m_slider_released;}

		Cpp::EventRef<int> value_changed () {return m_value_changed;}

	protected:

		virtual void render () = 0;

		Cpp::Event<int> m_slider_moved;

		Cpp::Event<> m_slider_pressed;

		Cpp::Event<> m_slider_released;

		Cpp::Event<int> m_value_changed;

	private:

		Orientation m_orientation;

		int m_value;

		int m_minimum;
		int m_maximum;
		int m_step;


	};

}

#endif	// _BIL_ABSTRACTSLIDER_HPP_
