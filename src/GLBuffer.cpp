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
#include <iostream>

#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	GLBuffer::GLBuffer ()
	: m_key(0)
	{
	}

	GLBuffer::~GLBuffer ()
	{
		clear ();
	}

	void GLBuffer::create (int key)
	{
		if(m_map.count(key)) {
			return;
		}

		GLuint buffer_id;
		glGenBuffers(1, &buffer_id);

		m_map[key].id = buffer_id;
	}

	void GLBuffer::select (int key)
	{
		if(!m_map.count(key)) throw std::invalid_argument("Invalid key");

		m_key = key;
	}

	void GLBuffer::set_vertices (int vertices)
	{
		m_map[m_key].vertices = vertices;
	}

	void GLBuffer::set_vertices (int key, int vertices)
	{
		m_map[key].vertices = vertices;
	}

	void GLBuffer::set_unit_size (int size)
	{
		m_map[m_key].unit_size = size;
	}

	void GLBuffer::set_unit_size (int key, int size)
	{
		m_map[key].unit_size = size;
	}

	void GLBuffer::set_target (GLenum target)
	{
		m_map[m_key].target = target;
	}

	void GLBuffer::set_target (int key, GLenum target)
	{
		m_map[key].target = target;
	}

	void GLBuffer::set_usage (GLenum usage)
	{
		m_map[m_key].usage = usage;
	}

	void GLBuffer::set_usage (int key, GLenum usage)
	{
		m_map[key].usage = usage;
	}

	void GLBuffer::set_property (int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_map[m_key].vertices = vertices;
		m_map[m_key].unit_size = unit_size;
		m_map[m_key].target = target;
		m_map[m_key].usage = usage;
	}

	void GLBuffer::set_property (int key, int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_map[key].vertices = vertices;
		m_map[key].unit_size = unit_size;
		m_map[key].target = target;
		m_map[key].usage = usage;
	}

	void GLBuffer::bind ()
	{
		glBindBuffer (m_map[m_key].target, m_map[m_key].id);
	}

	void GLBuffer::bind (int key)
	{
		glBindBuffer (m_map[key].target, m_map[m_key].id);
	}

	void GLBuffer::unbind ()
	{
		glBindBuffer(m_map[m_key].target, 0);
	}

	void GLBuffer::unbind (int key)
	{
		glBindBuffer(m_map[key].target, 0);
	}

	bool GLBuffer::is_buffer ()
	{
		if(glIsBuffer (m_map[m_key].id)) {
			return true;
		}

		return false;
	}

	bool GLBuffer::is_buffer (int key)
	{
		if(glIsBuffer (m_map[key].id)) {
			return true;
		}

		return false;
	}

	void GLBuffer::upload (const GLvoid* data)
	{
		glBufferData (m_map[m_key].target,
				m_map[m_key].unit_size * m_map[m_key].vertices,
				data,
				m_map[m_key].usage);
	}

	void GLBuffer::upload (int key, const GLvoid* data)
	{
		glBufferData (m_map[key].target,
				m_map[key].unit_size * m_map[key].vertices,
				data,
				m_map[key].usage);
	}

	void GLBuffer::destroy ()
	{
		glDeleteBuffers(1, &(m_map[m_key].id));
		m_map[m_key].id = 0;
		m_map[m_key].vertices = 0;
		m_map[m_key].unit_size = 0;
		m_map[m_key].target = 0;
		m_map[m_key].usage = 0;
	}

	void GLBuffer::destroy (int key)
	{
		glDeleteBuffers(1, &(m_map[key].id));
		m_map[key].id = 0;
		m_map[key].vertices = 0;
		m_map[key].unit_size = 0;
		m_map[key].target = 0;
		m_map[key].usage = 0;
	}

	void GLBuffer::clear ()
	{
		std::map<int, Property>::iterator it;

		for(it = m_map.begin(); it != m_map.end(); it++)
		{
			glDeleteBuffers (1, &(it->second.id));
		}

		m_map.clear();
	}

}
