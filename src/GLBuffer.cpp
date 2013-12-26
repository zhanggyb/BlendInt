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

#include <BlendInt/GLBuffer.hpp>

#include <stdexcept>
#include <iterator>
#include <iostream>

namespace BlendInt {

	GLBuffer::GLBuffer ()
	: m_index(0)
	{
	}

	GLBuffer::~GLBuffer ()
	{
		Clear ();
	}

	void GLBuffer::Generate (size_t num)
	{
		if(m_ids.size()) Clear();

		m_ids.resize(num);
		m_properties.resize(num);

		glGenBuffers((GLsizei)num, &(m_ids[0]));
	}

	bool GLBuffer::is_buffer ()
	{
		if(glIsBuffer (m_ids[m_index])) {
			return true;
		}

		return false;
	}

	bool GLBuffer::is_buffer (size_t index)
	{
		if(glIsBuffer (m_ids[index])) {
			return true;
		}

		return false;
	}

	void GLBuffer::Destroy ()
	{
		glDeleteBuffers(1, &(m_ids[m_index]));

		std::vector<GLuint>::iterator it_id = m_ids.begin();
		std::vector<Property>::iterator it_pro = m_properties.begin();

		std::advance(it_id, m_index);
		std::advance(it_pro, m_index);

		m_ids.erase(it_id);
		m_properties.erase(it_pro);
	}

	void GLBuffer::Destroy (size_t index)
	{
		glDeleteBuffers(1, &(m_ids[index]));

		if(m_index > index) m_index -= 1;

		std::vector<GLuint>::iterator it_id = m_ids.begin();
		std::vector<Property>::iterator it_pro = m_properties.begin();

		std::advance(it_id, index);
		std::advance(it_pro, index);

		m_ids.erase(it_id);
		m_properties.erase(it_pro);
	}

	void GLBuffer::Clear ()
	{
		glDeleteBuffers((GLsizei)m_ids.size(), &(m_ids[0]));

		m_ids.clear();
		m_properties.clear();
	}

}
