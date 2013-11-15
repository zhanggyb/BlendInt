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

	AbstractExtraForm::AbstractExtraForm(const AbstractExtraForm& orig)
	: AbstractForm()
	{
		set_position(orig.position());
		resize(orig.size());
		m_expand_x = orig.expand_x();
		m_expand_y = orig.expand_y();
		m_preferred_size = orig.preferred_size();
		m_minimal_size = orig.minimal_size();
		m_maximal_size = orig.maximal_size();
	}

	AbstractExtraForm::~AbstractExtraForm()
	{

	}


	bool AbstractExtraForm::set_position(int x, int y)
	{
		Point new_pos (x, y);
		if(m_position == new_pos) return false;

		update(FormPosition, &new_pos);
		m_position.set_x(x);
		m_position.set_y(y);

		return true;
	}

	bool AbstractExtraForm::set_position(const Point& pos)
	{
		if(m_position == pos) return false;
		update(FormPosition, &pos);
		m_position = pos;

		return true;
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

	bool AbstractExtraForm::resize (unsigned int width, unsigned int height)
	{
		if(width < m_minimal_size.width() ||
				height < m_minimal_size.height() ||
				width > m_maximal_size.width() ||
				height > m_maximal_size.height())
			return false;

		return AbstractForm::resize(width, height);
	}

	bool AbstractExtraForm::resize (const Size& size)
	{
		if(size.width() < m_minimal_size.width() ||
				size.height() < m_minimal_size.height() ||
				size.width() > m_maximal_size.width() ||
				size.height() > m_maximal_size.height())
			return false;

		return AbstractForm::resize(size);
	}

	bool AbstractExtraForm::set_preferred_size (unsigned int width,
	        unsigned int height)
	{
		// check the param first
		if (width < m_minimal_size.width() ||
				height < m_minimal_size.height() ||
				width > m_maximal_size.width() ||
				height > m_maximal_size.height())
			return false;

		Size new_pref_size(width, height);

		if (m_preferred_size == new_pref_size)
			return false;

		update(FormPreferredSize, &new_pref_size);

		m_preferred_size.set_width(width);
		m_preferred_size.set_height(height);

		return true;
	}

	bool AbstractExtraForm::set_preferred_size(const Size& size)
	{
		if (size.width() < m_minimal_size.width() ||
				size.height() < m_minimal_size.height()||
				size.width() > m_maximal_size.width() ||
				size.height() > m_maximal_size.height())
			return false;

		if(m_preferred_size == size) return false;

		update(FormPreferredSize, &size);

		m_preferred_size = size;

		return true;
	}

	bool AbstractExtraForm::set_minimal_size(unsigned int width, unsigned int height)
	{
		if(width > m_preferred_size.width() ||
				height > m_preferred_size.height())
			return false;

		Size new_min_size(width, height);

		if (m_minimal_size == new_min_size) return false;

		update(FormMinimalSize, &new_min_size);

		m_minimal_size = new_min_size;

		return true;
	}

	bool AbstractExtraForm::set_minimal_size(const Size& size)
	{
		if(size.width() > m_preferred_size.width() ||
				size.height() > m_preferred_size.height())
			return false;

		if (m_minimal_size.equal(size)) return false;

		update(FormMinimalSize, &size);

		m_minimal_size = size;

		return true;
	}

	bool AbstractExtraForm::set_maximal_size(unsigned int width, unsigned int height)
	{
		if(width < m_preferred_size.width() ||
				height < m_preferred_size.height())
			return false;

		Size new_max_size (width, height);

		if(m_maximal_size == new_max_size) return false;

		update(FormMaximalSize, &new_max_size);

		m_maximal_size = new_max_size;

		return true;
	}

	bool AbstractExtraForm::set_maximal_size(const Size& size)
	{
		if(size.width() < m_preferred_size.width() ||
				size.height() < m_preferred_size.height())
			return false;

		if(m_maximal_size == size) return false;

		update(FormMaximalSize, &size);

		m_maximal_size = size;

		return true;
	}

}
