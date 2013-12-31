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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BlendInt/GLTexture2D.hpp>

namespace BlendInt {

	GLTexture2D::GLTexture2D ()
	: m_index(0)
	{
	}

	GLTexture2D::~GLTexture2D ()
	{
		Clear();
	}

	void GLTexture2D::Generate (size_t size)
	{
		if(m_ids.size()) Clear ();

		m_ids.resize(size);

		glGenTextures((GLsizei)size, &(m_ids[0]));
	}

	void GLTexture2D::Bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_ids[m_index]);
	}

	void GLTexture2D::SetParameter (GLenum pname, GLint param)
	{
		glTexParameteri(GL_TEXTURE_2D, pname, param);
	}

	void GLTexture2D::SetParameter (GLenum pname, GLfloat param)
	{
		glTexParameterf(GL_TEXTURE_2D, pname, param);
	}

	void GLTexture2D::Map (GLint level, GLint internalFormat, GLsizei width,
	        GLsizei height, GLint border, GLenum format, GLenum type,
	        const GLvoid* data)
	{
		glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border,
		                         format, type, data);
	}

	void GLTexture2D::Unbind ()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::Clear ()
	{
		glDeleteTextures((GLsizei)m_ids.size(), &(m_ids[0]));
		m_ids.clear();
	}

}
