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

#include <BlendInt/AbstractExtraForm.hpp>

namespace BlendInt {

	AbstractExtraForm::AbstractExtraForm()
	: AbstractForm(), m_expand_x(false), m_expand_y(false)
	{
		m_maximal_size.set_width(65536);
		m_maximal_size.set_height(65536);
	}

	AbstractExtraForm::AbstractExtraForm(Object* super)
	: AbstractForm(super), m_expand_x(false), m_expand_y(false)
	{
		m_maximal_size.set_width(65536);
		m_maximal_size.set_height(65536);
	}

	AbstractExtraForm::AbstractExtraForm(const AbstractExtraForm& orig)
	: AbstractForm()
	{
		m_position = orig.position();
		set_size(orig.size());
		m_expand_x = orig.expand_x();
		m_expand_y = orig.expand_y();
		m_preferred_size = orig.preferred_size();
		m_minimal_size = orig.minimal_size();
		m_maximal_size = orig.maximal_size();
	}

	AbstractExtraForm::~AbstractExtraForm()
	{

	}

	bool AbstractExtraForm::contain (const Point& point)
	{
		if(point.x() < m_position.x() ||
				point.y() < m_position.y() ||
				point.x() > static_cast<int>(m_position.x() + size().width()) ||
				point.y() > static_cast<int>(m_position.y() + size().height()))
		{
			return false;
		}

		return true;
	}

	bool AbstractExtraForm::contain (int x, int y)
	{
		if(x < m_position.x() ||
				y < m_position.y() ||
				x > static_cast<int>(m_position.x() + size().width()) ||
				y > static_cast<int>(m_position.y() + size().height()))
		{
			return false;
		}

		return true;
	}

}
