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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/Texture2D.hpp>

namespace BlendInt {

	Texture2D::Texture2D ()
			: m_id(0), m_width(512), m_height(512), m_bind(false)
	{

	}

	Texture2D::Texture2D (GLsizei width, GLsizei height)
			: m_id(0), m_width(width), m_height(height), m_bind(false)
	{

	}

	Texture2D::~Texture2D ()
	{
		Clear();
	}

	void Texture2D::Generate ()
	{
		Clear();

		glGenTextures(1, &m_id);
	}

	void Texture2D::Bind ()
	{
		if (!m_bind) {
			glBindTexture(GL_TEXTURE_2D, m_id);
			m_bind = true;
		}
	}

	void Texture2D::SetSize (GLsizei width, GLsizei height)
	{
		if (m_bind) {
			// TODO: if should reset the texture
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}

		m_width = width;
		m_height = height;
	}

	GLint Texture2D::GetBaseLevel()
	{
		if(m_bind) {
			return 0;
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return 0;
		}
	}
	
	void Texture2D::SetBaseLevel (GLint level)
	{
		if (m_bind) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetParameter (GLenum name, GLint value)
	{
		if (m_bind) {
			glTexParameteri(GL_TEXTURE_2D, name, value);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetParameter (GLenum name, GLfloat value)
	{
		if (m_bind) {
			glTexParameterf(GL_TEXTURE_2D, name, value);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetImage (GLint level, GLint internalFormat, GLint border,
	        GLenum format, GLenum type, const GLvoid* data)
	{
		if (m_bind) {
			glTexImage2D(GL_TEXTURE_2D, level, internalFormat, m_width,
			        m_height, border, format, type, data);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::Unbind ()
	{
		if (m_bind) {
			glBindTexture(GL_TEXTURE_2D, 0);
			m_bind = false;
		}
	}

	void Texture2D::SetMinFilter (GLint filter)
	{
		SetParameter(GL_TEXTURE_MIN_FILTER, filter);
	}

	void Texture2D::SetMagFilter (GLint filter)
	{
		SetParameter(GL_TEXTURE_MAG_FILTER, filter);
	}

	void Texture2D::SetWrapMode (GLint s_mode, GLint t_mode)
	{
		if (m_bind) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_mode);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_mode);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() first!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetWrapModeS (GLint mode)
	{
		SetParameter(GL_TEXTURE_WRAP_S, mode);
	}

	void Texture2D::SetWrapModeT (GLint mode)
	{
		SetParameter(GL_TEXTURE_WRAP_T, mode);
	}

	void Texture2D::Clear ()
	{
		if (glIsTexture(m_id)) {
			glDeleteTextures(1, &m_id);
		}

		m_id = 0;
		m_bind = false;
	}

}
