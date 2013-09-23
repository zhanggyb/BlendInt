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

#include <algorithm>

#include <BILO/AbstractSlider.hpp>

namespace BILO {

	AbstractSlider::AbstractSlider(Orientation orientation)
		: Widget (), m_orientation(orientation),
		  m_value(0), m_minimum(0), m_maximum(100), m_step(1)
	{
	}

	AbstractSlider::AbstractSlider(Orientation orientation,
								   Drawable* parent)
		: Widget (parent), m_orientation(orientation),
		  m_value(0), m_minimum(0), m_maximum(100), m_step(1)
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
			update(SliderPropertyMinimum);
		}
		if(m_maximum != maximum) {
			m_maximum = maximum;
			update(SliderPropertyMaximum);
		}
	}

	void AbstractSlider::set_minimum (int minimum)
	{
		if(m_minimum == minimum) return;

		m_minimum = minimum;
		update (SliderPropertyMinimum);
	}

	void AbstractSlider::set_maximum (int maximum)
	{
		if(m_maximum == maximum) return;

		m_maximum = maximum;
		update(SliderPropertyMaximum);
	}

	void AbstractSlider::set_value (int value)
	{
		if(m_value == value) return;

		if(value < m_minimum || value > m_maximum) return;

		m_value = value;

		update(SliderPropertyValue);
	}

	void AbstractSlider::set_step (int step)
	{
		if(m_step == step) return;

		m_step = step;

		update(SliderPropertyStep);
	}

	void AbstractSlider::set_orientation (Orientation orientation)
	{
		if(m_orientation == orientation) return;

		m_orientation = orientation;

		update(SliderPropertyOrientation);
	}

}
