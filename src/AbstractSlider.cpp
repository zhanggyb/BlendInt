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

#include <algorithm>

#include <BlendInt/AbstractSlider.hpp>

namespace BlendInt {

	AbstractSlider::AbstractSlider(Orientation orientation)
		: RoundWidget (), m_orientation(orientation),
		  m_value(0), m_minimum(0), m_maximum(100), m_step(5)
	{
	}

	AbstractSlider::AbstractSlider(Orientation orientation,
								   AbstractWidget* parent)
		: RoundWidget (parent), m_orientation(orientation),
		  m_value(0), m_minimum(0), m_maximum(100), m_step(5)
	{
	}

	AbstractSlider::~AbstractSlider()
	{
	
	}

	void AbstractSlider::set_range (int value1, int value2)
	{
		int minimum = std::min(value1, value2);
		int maximum = std::max(value1, value2);

		if(m_minimum != minimum) {
			m_minimum = minimum;
			//update(SliderPropertyMinimum, 0);
		}
		if(m_maximum != maximum) {
			m_maximum = maximum;
			//update(SliderPropertyMaximum, 0);
		}
	}

	void AbstractSlider::set_minimum (int minimum)
	{
		if(m_minimum == minimum) return;

		m_minimum = minimum;
		//update (SliderPropertyMinimum, 0);
	}

	void AbstractSlider::set_maximum (int maximum)
	{
		if(m_maximum == maximum) return;

		m_maximum = maximum;
		//update(SliderPropertyMaximum, 0);
	}

	void AbstractSlider::set_value (int value)
	{
		if(m_value == value) return;

		if(value < m_minimum || value > m_maximum) return;

		m_value = value;

		//update(SliderPropertyValue, 0);
	}

	void AbstractSlider::set_step (int step)
	{
		if(m_step == step) return;

		m_step = step;

		//update(SliderPropertyStep, 0);
	}

	void AbstractSlider::set_orientation (Orientation orientation)
	{
		if(m_orientation == orientation) return;

		m_orientation = orientation;

		//update(SliderPropertyOrientation, 0);
	}

}
