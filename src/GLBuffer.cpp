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
#include <GL/glew.h>

#include <BIL/GLBuffer.hpp>

namespace BILO {

	GLBuffer::GLBuffer (unsigned int size)
	: m_index(0), m_buffers(size)
	{
		if(size > 0) {
			glGenBuffers (size, &m_buffers[0]);
		}
	}

	GLBuffer::~GLBuffer ()
	{
		clear ();
	}

	bool GLBuffer::generate (unsigned int size)
	{
		if(size> 0) {
			clear ();
			m_buffers.resize(size);
			glGenBuffers (size, &m_buffers[0]);
			m_index = 0;
			return true;
		} else {
			return false;
		}
	}

	bool GLBuffer::reset (size_t index)
	{
		if(m_buffers.empty()) return false;
		if (index > (m_buffers.size() - 1)) return false;

		glDeleteBuffers(1, &m_buffers[index]);
		glGenBuffers(1, &m_buffers[index]);

		return true;
	}

	void GLBuffer::bind (GLenum target)
	{
		if(m_buffers.empty()) return;

		glBindBuffer (target, m_buffers[m_index]);
	}

	bool GLBuffer::bind (size_t index, GLenum target)
	{
		if(m_buffers.empty()) return false;

		if(index > (m_buffers.size() - 1)) return false;

		glBindBuffer (target, m_buffers[index]);

		return true;
	}

	void GLBuffer::unbind (GLenum target)
	{
		glBindBuffer(target, 0);
	}

	void GLBuffer::set_index (size_t index)
	{
		if(m_buffers.empty()) return;

		if(index > (m_buffers.size() - 1))
			return;

		m_index = index;
	}

	bool GLBuffer::is_buffer (size_t index)
	{
		if(m_buffers.empty()) return false;

		if (index > (m_buffers.size() - 1)) return false;

		if(glIsBuffer (m_buffers[index])) {
			return true;
		}

		return false;
	}

	void GLBuffer::upload (GLenum target, GLsizeiptr size, const GLvoid* data,
        GLenum usage)
	{
		glBufferData (target, size, data, usage);
	}

	void GLBuffer::destroy ()
	{
		if(m_buffers.empty()) return;

		glDeleteBuffers(1, &m_buffers[m_index]);
		m_buffers[m_index] = 0;
	}

	bool GLBuffer::destroy (size_t index)
	{
		if(m_buffers.empty()) return false;

		if(index > (m_buffers.size() - 1)) return false;

		glDeleteBuffers(1, &m_buffers[index]);
		m_buffers[index] = 0;

		return true;
	}

	void GLBuffer::clear ()
	{
		if(m_buffers.empty()) return;

		std::vector<GLuint>::iterator it;
		for(it = m_buffers.begin(); it != m_buffers.end(); it++)
		{
			glDeleteBuffers (m_buffers.size(), &m_buffers[0]);
		}

		m_buffers.clear();
	}

}
