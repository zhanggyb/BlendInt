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

#include <algorithm>

#include <BIL/AbstractSlider.hpp>

namespace BILO {

	AbstractSlider::AbstractSlider(Orientation orientation,
								   Drawable* parent)
		: Widget (parent), m_orientation(orientation), m_step(1),
		  m_value(50), m_minimum(0), m_maximum(100)
	{
	}

	AbstractSlider::~AbstractSlider()
	{
	
	}

	void AbstractSlider::set_range (int value1, int value2)
	{
		m_minimum = std::min(value1, value2);
		m_maximum = std::max(value1, value2);
	}

	inline void AbstractSlider::set_minimum (int minimum)
	{
		m_minimum = minimum;
	}

	inline void AbstractSlider::set_maximum (int maximum)
	{
		m_maximum = maximum;
	}

	void AbstractSlider::set_value (int value)
	{
		m_value = value;
	}

	inline void AbstractSlider::set_step (int step)
	{
		m_step = step;
	}

	inline void AbstractSlider::set_orientation (Orientation orientation)
	{
		m_orientation = orientation;
	}

}
