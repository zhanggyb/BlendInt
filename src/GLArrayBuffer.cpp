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

#include <BlendInt/GLArrayBuffer.hpp>

namespace BlendInt {

	GLArrayBuffer::GLArrayBuffer ()
	: m_id(0),
	  m_vertices(0),
	  m_unit_size(0)
	{
	}

	GLArrayBuffer::~GLArrayBuffer ()
	{
		Clear();
	}

	void GLArrayBuffer::Generate ()
	{
		Clear();

		glGenBuffers(1, &m_id);
	}

	void GLArrayBuffer::Clear ()
	{
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}

	void GLArrayBuffer::Bind ()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void GLArrayBuffer::Unbind ()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void GLArrayBuffer::SetData (GLsizeiptr size, const GLvoid* data, GLenum usage)
	{
		glBufferData (GL_ARRAY_BUFFER,	size,	data,	usage);
	}

	GLenum GLArrayBuffer::GetUsage ()
	{
		GLint usage = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);

		return usage;
	}

	GLint GLArrayBuffer::GetBufferSize ()
	{
		GLint size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &size);

		return size;
	}

}
