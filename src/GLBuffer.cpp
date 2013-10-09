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

		m_buffers.resize(size);
		glGenBuffers (size, &m_buffers[0]);
		m_index = 0;

		m_buffer_sizes.resize(size, 0);
		m_buffer_targets.resize(size, 0);
		m_buffer_usages.resize(size, 0);

		return;
	}

	bool GLBuffer::reset (size_t index)
	{
		if(m_buffers.empty()) return false;
		if (index > (m_buffers.size() - 1)) return false;

		glDeleteBuffers(1, &m_buffers[index]);
		glGenBuffers(1, &m_buffers[index]);

		m_buffer_sizes[index] = 0;
		m_buffer_targets[index] = 0;
		m_buffer_usages[index] = 0;

		return true;
	}

	void GLBuffer::bind (GLenum target)
	{
		if(m_buffers.empty()) return;

		glBindBuffer (target, m_buffers[m_index]);

		m_buffer_targets[m_index] = target;
	}

	bool GLBuffer::bind (size_t index, GLenum target)
	{
		if(m_buffers.empty()) return false;

		if(index > (m_buffers.size() - 1)) return false;

		glBindBuffer (target, m_buffers[index]);

		m_buffer_targets[index] = target;

		return true;
	}

	void GLBuffer::rebind ()
	{
		if(m_buffers.empty()) return;

		glBindBuffer (m_buffer_targets[m_index], m_buffers[m_index]);
	}

	void GLBuffer::rebind (size_t index)
	{
		if(m_buffers.empty()) return;

		if(index > (m_buffers.size() - 1)) return;

		glBindBuffer (m_buffer_targets[index], m_buffers[index]);
	}

	void GLBuffer::unbind ()
	{
		glBindBuffer(m_buffer_targets[m_index], 0);
	}

	void GLBuffer::unbind (size_t index)
	{
		if(m_buffers.empty()) return;

		if(index > (m_buffers.size() - 1)) return;

		glBindBuffer (m_buffer_targets[index], 0);
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

		if(glIsBuffer (m_buffers[m_index])) {
			return true;
		}

		return false;
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

	void GLBuffer::upload (GLsizeiptr size, const GLvoid* data,
        GLenum usage)
	{
		glBufferData (m_buffer_targets[m_index], size, data, usage);
		m_buffer_sizes[m_index] = size;
		m_buffer_usages[m_index] = usage;
	}

	void GLBuffer::upload (size_t index, GLsizeiptr size, const GLvoid* data,
        GLenum usage)
	{
		if(m_buffers.empty()) return;

		if (index > (m_buffers.size() - 1)) return;

		glBufferData (m_buffer_targets[index], size, data, usage);
		m_buffer_sizes[index] = size;
		m_buffer_usages[index] = usage;
	}

	void GLBuffer::destroy ()
	{
		if(m_buffers.empty()) return;

		glDeleteBuffers(1, &m_buffers[m_index]);
		m_buffers[m_index] = 0;

		m_buffer_sizes[m_index] = 0;
		m_buffer_targets[m_index] = 0;
		m_buffer_usages[m_index] = 0;
	}

	bool GLBuffer::destroy (size_t index)
	{
		if(m_buffers.empty()) return false;

		if(index > (m_buffers.size() - 1)) return false;

		glDeleteBuffers(1, &m_buffers[index]);
		m_buffers[index] = 0;

		m_buffer_sizes[index] = 0;
		m_buffer_targets[index] = 0;
		m_buffer_usages[index] = 0;

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
		m_buffer_sizes.clear();
		m_buffer_targets.clear();
		m_buffer_usages.clear();
	}

}
