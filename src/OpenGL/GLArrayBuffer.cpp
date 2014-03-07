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

#include <iostream>

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	GLArrayBuffer::GLArrayBuffer()
		: Object(), m_id(0)
	{
	}

	GLArrayBuffer::~GLArrayBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void GLArrayBuffer::Generate()
	{
		if(!m_id)
			Clear();

		glGenBuffers(1, &m_id);
	}

	void GLArrayBuffer::Clear()
	{
		//if(glIsBuffer(m_id)) {
			glDeleteBuffers(1, &m_id);
		//}
		m_id = 0;
	}

	bool GLArrayBuffer::IsBuffer ()
	{
		return glIsBuffer(m_id);
	}

	void GLArrayBuffer::SetData (GLsizeiptr size, const GLvoid* data,
	        GLenum usage)
	{
		if(glIsBuffer(m_id) == GL_FALSE) {
			std::cerr << "The array buffer is not generated!" << std::endl;
			return;
		}

		GLint buffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer);

		if(m_id != static_cast<GLuint>(buffer)) {
			std::cerr << "The current array buffer binding is not the one to be set data, call Bind() first." << std::endl;
			return;
		}

		glBufferData (GL_ARRAY_BUFFER, size, data, usage);
	}

	/*
	void GLArrayBuffer::SetData (int vertices, size_t size, const GLvoid* data,
	        GLenum usage)
	{
		if(glIsBuffer(m_id) == GL_FALSE) {
			std::cerr << "The array buffer is not generated!" << std::endl;
			return;
		}

		GLint buffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer);

		if(m_id != static_cast<GLuint>(buffer)) {
			std::cerr << "The current array buffer binding is not the one to be set data, call Bind() first." << std::endl;
			return;
		}

		m_vertices = vertices;
		glBufferData (GL_ARRAY_BUFFER, size * m_vertices, data, usage);
	}
	*/

	void GLArrayBuffer::UpdateData (const GLvoid* data, GLsizeiptr size, GLintptr offset)
	{
		if(glIsBuffer(m_id) == GL_FALSE) {
			std::cerr << "The array buffer is not generated!" << std::endl;
			return;
		}

		GLint buffer = 0;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &buffer);

		if(m_id != static_cast<GLuint>(buffer)) {
			std::cerr << "The current array buffer binding is not the one to be set data, call Bind() first." << std::endl;
			return;
		}

		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	GLvoid* GLArrayBuffer::Map (GLenum access)
	{
		GLvoid* ptr = 0;

		ptr = glMapBuffer(GL_ARRAY_BUFFER, access);

		GLenum error = glGetError();

		switch(error) {
			case GL_INVALID_ENUM:
				break;

			case GL_OUT_OF_MEMORY:
				break;

			case GL_INVALID_OPERATION:
				break;

			default:
				break;
		}

		return ptr;
	}

	bool GLArrayBuffer::Unmap ()
	{
		return glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void GLArrayBuffer::Bind()
	{
		if(m_id) {
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		} else {
			std::cerr << "The array buffer is not generated! call Generate() first." << std::endl;
		}
	}

	void GLArrayBuffer::Reset()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLenum GLArrayBuffer::GetUsage ()
	{
		GLint usage = 0;

		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);

		return usage;
	}

	GLint GLArrayBuffer::GetBufferSize ()
	{
		GLint buffer_size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &buffer_size);

		return buffer_size;
	}

}

