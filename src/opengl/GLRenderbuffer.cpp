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

#include <BlendInt/opengl/GLRenderbuffer.hpp>

namespace BlendInt {

	GLRenderbuffer::GLRenderbuffer ()
	: m_id(0)
	{

	}

	GLRenderbuffer::~GLRenderbuffer ()
	{
		glDeleteRenderbuffers(1, &m_id);
	}

	void GLRenderbuffer::Generate ()
	{
		if(!m_id) {
			Clear();
		}

		glGenRenderbuffers(1, &m_id);
	}

	void GLRenderbuffer::Bind()
	{
		if(m_id) {
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		} else {
			std::cerr << "The render buffer is not generated! call Generate() first." << std::endl;
		}
	}

	void GLRenderbuffer::Reset()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void GLRenderbuffer::SetStorage (GLenum internalformat, GLsizei width,
	        GLsizei height)
	{
		if(glIsBuffer(m_id) == GL_FALSE) {
			std::cerr << "The element array buffer is not generated!" << std::endl;
			return;
		}

		GLint buffer = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &buffer);

		if(m_id != static_cast<GLuint>(buffer)) {
			std::cerr << "The current element array buffer binding is not the one to be set data, call Bind() first!" << std::endl;
			return;
		}

		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
	}

	void GLRenderbuffer::Clear ()
	{
		if(glIsRenderbuffer(m_id)) {
			glDeleteRenderbuffers(1, &m_id);
		}

		m_id = 0;
	}

}
