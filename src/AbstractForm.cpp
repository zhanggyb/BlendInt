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

#include <BlendInt/AbstractForm.hpp>

#include <BlendInt/Utilities-inl.hpp>

namespace BlendInt {

	AbstractForm::AbstractForm()
	{

	}

	AbstractForm::AbstractForm(const AbstractForm& orig)
	{
		m_position = orig.position();
		m_size = orig.size();
	}

	AbstractForm::~AbstractForm()
	{

	}

	bool AbstractForm::resize(unsigned int width, unsigned int height)
	{
		Size new_size (width, height);

		if (m_size == new_size) return false;

		update(FormSize, &new_size);

		m_size.set_width(width);
		m_size.set_height(height);

		return true;
	}

	bool AbstractForm::resize(const Size& size)
	{
		if(m_size == size) return false;

		update(FormSize, &size);
		m_size = size;

		return true;
	}

	bool AbstractForm::set_position(int x, int y)
	{
		Point new_pos (x, y);
		if(m_position == new_pos) return false;

		update(FormPosition, &new_pos);
		m_position.set_x(x);
		m_position.set_y(y);

		return true;
	}

	bool AbstractForm::set_position(const Point& pos)
	{
		if(m_position == pos) return false;
		update(FormPosition, &pos);
		m_position = pos;

		return true;
	}

	bool AbstractForm::contain (const Point& point)
	{
		if(point.x() < m_position.x() ||
				point.y() < m_position.y() ||
				point.x() > static_cast<int>(m_position.x() + m_size.width()) ||
				point.y() > static_cast<int>(m_position.y() + m_size.height()))
		{
			return false;
		}

		return true;
	}

	bool AbstractForm::contain (int x, int y)
	{
		if(x < m_position.x() ||
				y < m_position.y() ||
				x > static_cast<int>(m_position.x() + m_size.width()) ||
				y > static_cast<int>(m_position.y() + m_size.height()))
		{
			return false;
		}

		return true;
	}

	void AbstractForm::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][2],
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int i;
		for (i = 0; i < totvert; i++) {
			copy_v2_v2(quad_strip[i * 2], outer_v[i]);
			copy_v2_v2(quad_strip[i * 2 + 1], inner_v[i]);
		}
		copy_v2_v2(quad_strip[i * 2], outer_v[0]);
		copy_v2_v2(quad_strip[i * 2 + 1], inner_v[0]);
	}

	void AbstractForm::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][6],
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int i;
		for (i = 0; i < totvert; i++) {
			copy_v2_v2(quad_strip[i * 2], outer_v[i]);
			copy_v2_v2(quad_strip[i * 2 + 1], inner_v[i]);
		}
		copy_v2_v2(quad_strip[i * 2], outer_v[0]);
		copy_v2_v2(quad_strip[i * 2 + 1], inner_v[0]);
	}

	void AbstractForm::verts_to_quad_strip_open (
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		for (int i = 0; i < totvert; i++) {
			quad_strip[i * 2][0] = outer_v[i][0];
			quad_strip[i * 2][1] = outer_v[i][1];
			quad_strip[i * 2 + 1][0] = outer_v[i][0];
			quad_strip[i * 2 + 1][1] = outer_v[i][1] - 1.0f;
		}
	}

	void AbstractForm::dispatch_render(AbstractForm* obj)
	{
		obj->render();
	}

}
