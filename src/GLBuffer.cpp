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
#include <GL/glew.h>

#include <iostream>

#include <BILO/GLBuffer.hpp>

namespace BILO {

	GLBuffer::GLBuffer ()
	: m_index(-1)
	{
	}

	GLBuffer::~GLBuffer ()
	{
		clear ();
	}

	void GLBuffer::generate (size_t size)
	{
		clear ();

		GLuint* buffer = new GLuint[size];
		glGenBuffers (size, buffer);

		m_buffers.resize(size);

		for(size_t i = 0; i < size; i++)
		{
			m_buffers[i].id = *(buffer + i);
		}
		m_index = 0;

		delete [] buffer;
	}

	void GLBuffer::append (size_t size)
	{
		GLuint* buffer = new GLuint[size];
		glGenBuffers (size, buffer);

		Property buf_data;

		for(size_t i = 0; i < size; i++)
		{
			buf_data.id = *(buffer + i);
			m_buffers.push_back(buf_data);
		}

		delete [] buffer;
	}

	bool GLBuffer::reset (size_t index)
	{
		if(m_buffers.empty()) return false;
		if (index > (m_buffers.size() - 1)) return false;

		glDeleteBuffers(1, &(m_buffers[index].id));
		glGenBuffers(1, &(m_buffers[index].id));

		m_buffers[index].vertices = 0;
		m_buffers[index].unit_size = 0;
		m_buffers[index].target = 0;
		m_buffers[index].usage = 0;

		return true;
	}

	void GLBuffer::set_vertices (int vertices)
	{
		m_buffers[m_index].vertices = vertices;
	}

	int GLBuffer::vertices () const
	{
		return m_buffers[m_index].vertices;
	}

	void GLBuffer::set_vertices (size_t index, int vertices)
	{
		m_buffers[index].vertices = vertices;
	}

	int GLBuffer::vertices (size_t index) const
	{
		return m_buffers[index].vertices;
	}

	void GLBuffer::set_unit_size (int size)
	{
		m_buffers[m_index].unit_size = size;
	}

	int GLBuffer::unit_size () const
	{
		return m_buffers[m_index].unit_size;
	}

	void GLBuffer::set_unit_size (size_t index, int size)
	{
		m_buffers[index].unit_size = size;
	}

	int GLBuffer::unit_size (size_t index) const
	{
		return m_buffers[index].unit_size;
	}

	void GLBuffer::set_target (GLenum target)
	{
		m_buffers[m_index].target = target;
	}

	GLenum GLBuffer::target () const
	{
		return m_buffers[m_index].target;
	}

	void GLBuffer::set_target (size_t index, GLenum target)
	{
		m_buffers[index].target = target;
	}

	GLenum GLBuffer::target (size_t index) const
	{
		return m_buffers[index].target;
	}

	void GLBuffer::set_usage (GLenum usage)
	{
		m_buffers[m_index].usage = usage;
	}

	GLenum GLBuffer::usage () const
	{
		return m_buffers[m_index].usage;
	}

	void GLBuffer::set_usage (size_t index, GLenum usage)
	{
		m_buffers[index].usage = usage;
	}

	GLenum GLBuffer::usage (size_t index) const
	{
		return m_buffers[index].usage;
	}

	void GLBuffer::set_property (int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_buffers[m_index].vertices = vertices;
		m_buffers[m_index].unit_size = unit_size;
		m_buffers[m_index].target = target;
		m_buffers[m_index].usage = usage;
	}

	void GLBuffer::set_property (size_t index, int vertices, int unit_size, GLenum target, GLenum usage)
	{
		m_buffers[index].vertices = vertices;
		m_buffers[index].unit_size = unit_size;
		m_buffers[index].target = target;
		m_buffers[index].usage = usage;
	}

	void GLBuffer::bind ()
	{
		if(m_buffers.empty()) return;

		glBindBuffer (m_buffers[m_index].target, m_buffers[m_index].id);
	}

	bool GLBuffer::bind (size_t index)
	{
		if(m_buffers.empty()) return false;

		if(index > (m_buffers.size() - 1)) return false;

		glBindBuffer (m_buffers[index].target, m_buffers[index].id);

		return true;
	}

	void GLBuffer::unbind ()
	{
		glBindBuffer(m_buffers[m_index].target, 0);
	}

	void GLBuffer::unbind (size_t index)
	{
		if(m_buffers.empty()) return;

		if(index > (m_buffers.size() - 1)) return;

		glBindBuffer (m_buffers[index].target, 0);
	}

	void GLBuffer::set_index (size_t index)
	{
		if(m_buffers.empty()) return;

		if(index > (m_buffers.size() - 1))
			return;

		m_index = index;
	}

	bool GLBuffer::is_buffer ()
	{
		if(m_buffers.empty()) return false;

		if(glIsBuffer (m_buffers[m_index].id)) {
			return true;
		}

		return false;
	}

	bool GLBuffer::is_buffer (size_t index)
	{
		if(m_buffers.empty()) return false;

		if (index > (m_buffers.size() - 1)) return false;

		if(glIsBuffer (m_buffers[index].id)) {
			return true;
		}

		return false;
	}

	void GLBuffer::upload (const GLvoid* data)
	{
		glBufferData (m_buffers[m_index].target,
				m_buffers[m_index].unit_size * m_buffers[m_index].vertices,
				data,
				m_buffers[m_index].usage);
	}

	void GLBuffer::upload (size_t index, const GLvoid* data)
	{
		glBufferData (m_buffers[index].target,
				m_buffers[index].unit_size * m_buffers[index].vertices,
				data,
				m_buffers[index].usage);
	}

	void GLBuffer::destroy ()
	{
		if(m_buffers.empty()) return;

		glDeleteBuffers(1, &(m_buffers[m_index].id));
		m_buffers[m_index].id = 0;
		m_buffers[m_index].vertices = 0;
		m_buffers[m_index].unit_size = 0;
		m_buffers[m_index].target = 0;
		m_buffers[m_index].usage = 0;
	}

	void GLBuffer::destroy (size_t index)
	{
		glDeleteBuffers(1, &(m_buffers[index].id));
		m_buffers[index].id = 0;
		m_buffers[index].vertices = 0;
		m_buffers[index].unit_size = 0;
		m_buffers[index].target = 0;
		m_buffers[index].usage = 0;
	}

	void GLBuffer::clear ()
	{
		if(m_buffers.empty()) return;

		GLuint* buffer = new GLuint[m_buffers.size()];

		for(size_t i = 0; i < m_buffers.size(); i++)
		{
			*(buffer + i) = m_buffers[i].id;
		}
		glDeleteBuffers (m_buffers.size(), buffer);

		delete [] buffer;

		m_buffers.clear();
	}

#ifdef DEBUG

	void GLBuffer::print (size_t index)
	{
		std::cout << "buffer data: "
				<< "id: " << m_buffers[index].id << " "
				<< "vertices: " << m_buffers[index].vertices << " "
				<< "unit size: " << m_buffers[index].unit_size << " "
				<< "target: " << m_buffers[index].target << " "
				<< "usage: " << m_buffers[index].usage << " "
				<< std::endl;
	}

#endif

}
