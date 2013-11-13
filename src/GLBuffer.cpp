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

#include <GL/glew.h>

#include <stdexcept>
#include <iterator>
#include <iostream>

#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	GLBuffer::GLBuffer ()
	: m_index(0)
	{
	}

	GLBuffer::~GLBuffer ()
	{
		clear ();
	}

	void GLBuffer::generate (size_t num)
	{
		if(m_ids.size()) clear();

		m_ids.resize(num);
		m_properties.resize(num);

		glGenBuffers(num, &(m_ids[0]));
	}

	void GLBuffer::set_vertices (int vertices)
	{
		m_properties[m_index].vertices = vertices;
	}

	void GLBuffer::set_vertices (size_t index, int vertices)
	{
		m_properties[index].vertices = vertices;
	}

	void GLBuffer::set_unit_size (int size)
	{
		m_properties[m_index].unit_size = size;
	}

	void GLBuffer::set_unit_size (size_t index, int size)
	{
		m_properties[index].unit_size = size;
	}

	void GLBuffer::set_target (GLenum target)
	{
		m_properties[m_index].target = target;
	}

	void GLBuffer::set_target (size_t index, GLenum target)
	{
		m_properties[index].target = target;
	}

	void GLBuffer::set_usage (GLenum usage)
	{
		m_properties[m_index].usage = usage;
	}

	void GLBuffer::set_usage (size_t index, GLenum usage)
	{
		m_properties[index].usage = usage;
	}

	void GLBuffer::set_property (int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_properties[m_index].vertices = vertices;
		m_properties[m_index].unit_size = unit_size;
		m_properties[m_index].target = target;
		m_properties[m_index].usage = usage;
	}

	void GLBuffer::set_property (size_t index, int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_properties[index].vertices = vertices;
		m_properties[index].unit_size = unit_size;
		m_properties[index].target = target;
		m_properties[index].usage = usage;
	}

	void GLBuffer::bind ()
	{
		glBindBuffer(m_properties[m_index].target, m_ids[m_index]);
	}

	void GLBuffer::bind (size_t index)
	{
		glBindBuffer(m_properties[index].target, m_ids[m_index]);
	}

	void GLBuffer::unbind ()
	{
		glBindBuffer(m_properties[m_index].target, 0);
	}

	void GLBuffer::unbind (size_t index)
	{
		glBindBuffer(m_properties[index].target, 0);
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

	void GLBuffer::upload (const GLvoid* data)
	{
		glBufferData (m_properties[m_index].target,
				m_properties[m_index].unit_size * m_properties[m_index].vertices,
				data,
				m_properties[m_index].usage);
	}

	void GLBuffer::upload (size_t index, const GLvoid* data)
	{
		glBufferData (m_properties[index].target,
				m_properties[index].unit_size * m_properties[index].vertices,
				data,
				m_properties[index].usage);
	}

	void GLBuffer::destroy ()
	{
		glDeleteBuffers(1, &(m_ids[m_index]));

		std::vector<GLuint>::iterator it_id = m_ids.begin();
		std::vector<Property>::iterator it_pro = m_properties.begin();

		std::advance(it_id, m_index);
		std::advance(it_pro, m_index);

		m_ids.erase(it_id);
		m_properties.erase(it_pro);
	}

	void GLBuffer::destroy (size_t index)
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

	void GLBuffer::clear ()
	{
		glDeleteBuffers(m_ids.size(), &(m_ids[0]));

		m_ids.clear();
		m_properties.clear();
	}

}
