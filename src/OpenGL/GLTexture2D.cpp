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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <boost/filesystem.hpp>

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	GLTexture2D::GLTexture2D ()
	: Object(),
	  m_id(0),
	  m_width(512),
	  m_height(512),
	  m_level(0),
	  m_internal_format(GL_RGBA8),
	  m_format(GL_RGBA),
	  m_type(GL_UNSIGNED_BYTE)
	{

	}

	GLTexture2D::GLTexture2D (GLsizei width, GLsizei height)
	: Object(),
	  m_id(0),
	  m_width(width),
	  m_height(height),
	  m_level(0),
	  m_internal_format(GL_RGBA8),
	  m_format(GL_RGBA),
	  m_type(GL_UNSIGNED_BYTE)
	{

	}

	GLTexture2D::~GLTexture2D ()
	{
		if(m_id) {
			glDeleteTextures(1, &m_id);
		}
	}

	void GLTexture2D::Generate ()
	{
		if(m_id) {
			Clear();
		}

		glGenTextures(1, &m_id);
	}

	void GLTexture2D::Bind()
	{
		if(m_id) {
			glBindTexture(GL_TEXTURE_2D, m_id);
		} else {
			std::cerr << "The texture is not generated! call Generate() first." << std::endl;
		}
	}

	void GLTexture2D::SetSize (GLsizei width, GLsizei height)
	{
		m_width = width;
		m_height = height;
	}

	GLint GLTexture2D::GetBaseLevel()
	{
		return 0;
	}

	void GLTexture2D::SetBaseLevel (GLint level)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level);
	}

	void GLTexture2D::SetParameter (GLenum name, GLint value)
	{
		glTexParameteri(GL_TEXTURE_2D, name, value);
	}

	void GLTexture2D::SetParameter (GLenum name, GLfloat value)
	{
		glTexParameterf(GL_TEXTURE_2D, name, value);
	}

	void GLTexture2D::SetImage (GLsizei width, GLsizei height, const GLvoid* data)
	{
		m_width = width;
		m_height = height;
		glTexImage2D(GL_TEXTURE_2D, m_level, m_internal_format, m_width,
				m_height, 0, m_format, m_type, data);
	}

	void GLTexture2D::CopySubimage(GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
	{
		glCopyTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, x, y, width, height);
	}

	void GLTexture2D::Reset ()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::SetMinFilter (GLint filter)
	{
		SetParameter(GL_TEXTURE_MIN_FILTER, filter);
	}

	void GLTexture2D::SetMagFilter (GLint filter)
	{
		SetParameter(GL_TEXTURE_MAG_FILTER, filter);
	}

	void GLTexture2D::SetWrapMode (GLint s_mode, GLint t_mode)
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_mode);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_mode);
	}

	void GLTexture2D::SetWrapModeS (GLint mode)
	{
		SetParameter(GL_TEXTURE_WRAP_S, mode);
	}

	void GLTexture2D::SetWrapModeT (GLint mode)
	{
		SetParameter(GL_TEXTURE_WRAP_T, mode);
	}

	GLint GLTexture2D::GetWidth () const
	{
		GLint width = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
		  	m_level,
		  	GL_TEXTURE_WIDTH,
		  	&width);

		return width;
	}

	GLint GLTexture2D::GetHeight () const
	{
		GLint height = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
		  	m_level,
		  	GL_TEXTURE_HEIGHT,
		  	&height);

		return height;
	}

	bool GLTexture2D::WriteToFile (const std::string& filename)
	{
		//if(!m_flag[2]) return false;

		using namespace boost;

		filesystem::path pathname (filename);

		if(filesystem::exists(pathname)) {
			if(filesystem::is_directory(pathname)) {
			}
			if(filesystem::is_regular_file(pathname)) {

			}

			std::cerr << "Warning: file \"" << pathname << "\" exists, overwrite to this file!" << std::endl;
			//return false;
		} else {

		}

		// demo code
		unsigned char pixels[m_width * m_height * 4];

		glGetTexImage (GL_TEXTURE_2D, m_level, m_format, m_type, pixels);

		ImageOutput* out = ImageOutput::create(filename);
		if(!out)
			return false;

		ImageSpec spec (m_width, m_height, 4, TypeDesc::UINT8);
		out->open(filename, spec);
		out->write_image(TypeDesc::UINT8, pixels);
		out->close();
		delete out;

		return true;
	}

#ifdef DEBUG
	void GLTexture2D::MakeCheckImage(unsigned char image[512][512][4])
	{
		int i, j, c;
		for (i = 0; i < 512; i++) {
			for (j = 0; j < 512; j++) {
				c = (((i & 0x8) == 0) ^ (((j & 0x8) == 0))) * 255;
				image[i][j][0] = (GLubyte) c;
				image[i][j][1] = (GLubyte) c;
				image[i][j][2] = (GLubyte) c;
				image[i][j][3] = (GLubyte) c;
			}
		}
	}
#endif

	void GLTexture2D::Clear ()
	{
		//if (glIsTexture(m_id)) {
		glDeleteTextures(1, &m_id);
		//}

		m_id = 0;
	}

}

