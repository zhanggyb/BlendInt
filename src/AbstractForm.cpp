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

namespace BlendInt {

	void AbstractForm::resize (AbstractForm* form, unsigned int w, unsigned int h)
	{
		form->m_size.set_width(w);
		form->m_size.set_height(h);
	}

	void AbstractForm::resize (AbstractForm* form, const Size& size)
	{
		form->m_size = size;
	}

	void AbstractForm::set_position(AbstractForm* form, int x, int y)
	{
		form->m_position.set_x(x);
		form->m_position.set_y(y);
	}

	void AbstractForm::set_position(AbstractForm* form, const Point& pos)
	{
		form->m_position = pos;
	}

	AbstractForm::AbstractForm()
	{

	}

	AbstractForm::~AbstractForm()
	{

	}

}
