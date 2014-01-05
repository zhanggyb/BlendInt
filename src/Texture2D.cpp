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
#include <boost/filesystem.hpp>

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/Texture2D.hpp>

namespace BlendInt {

	Texture2D::Texture2D ()
			: m_id(0), m_width(512), m_height(512), m_level(0), m_internal_format(GL_RGBA8), m_format(GL_RGBA), m_type(GL_UNSIGNED_BYTE)
	{

	}

	Texture2D::Texture2D (GLsizei width, GLsizei height)
			: m_id(0), m_width(width), m_height(height), m_level(0), m_internal_format(GL_RGBA8), m_format(GL_RGBA), m_type(GL_UNSIGNED_BYTE)
	{

	}

	Texture2D::~Texture2D ()
	{
		Clear();
	}

	void Texture2D::Generate ()
	{
		if(!m_flag[0]) {
			glGenTextures(1, &m_id);
			m_flag.set(0);
		}
	}

	void Texture2D::Bind ()
	{
		if (!m_flag[0]) {
			std::cerr
			        << "The texture is not generated, call Texture2D::Generate() first!"
			        << std::endl;
			return;
		} else if (!m_flag[1])		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			m_flag[1] = 1;
		}
	}

	void Texture2D::SetSize (GLsizei width, GLsizei height)
	{
		if(m_flag[2]) {
			// TODO: if reset image?
		}

		m_width = width;
		m_height = height;
	}

	GLint Texture2D::GetBaseLevel()
	{
		return 0;
	}
	
	void Texture2D::SetBaseLevel (GLint level)
	{
		if (m_flag[1]) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() before this function!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetParameter (GLenum name, GLint value)
	{
		if (m_flag[1]) {
			glTexParameteri(GL_TEXTURE_2D, name, value);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() before this funciton!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetParameter (GLenum name, GLfloat value)
	{
		if (m_flag[1]) {
			glTexParameterf(GL_TEXTURE_2D, name, value);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() before this function!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::SetImage (GLsizei width, GLsizei height, const GLvoid* data)
	{
		if (m_flag[1]) {
			m_width = width;
			m_height = height;
			glTexImage2D(GL_TEXTURE_2D, m_level, m_internal_format, m_width,
			        m_height, 0, m_format, m_type, data);
			if(data) {
				m_flag.set(2);
			} else {
				m_flag.reset(2);
			}
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() before this function!"
			        << std::endl;
			return;
		}
	}

	void Texture2D::Unbind ()
	{
		if (m_flag[1]) {
			glBindTexture(GL_TEXTURE_2D, 0);
			m_flag.reset(1);
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
		if (m_flag[1]) {
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_mode);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_mode);
		} else {
			std::cerr
			        << "The texture is not bound, call Texture2D::Bind() before this function!"
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

	bool Texture2D::WriteToFile (const std::string& filename)
	{
		if(!m_flag[2]) return false;

		using namespace boost;

		filesystem::path pathname (filename);

		if(filesystem::exists(pathname)) {
			if(filesystem::is_directory(pathname)) {
			}
			if(filesystem::is_regular_file(pathname)) {

			}

			std::cerr << pathname << " exists, abort writing to this file!" << std::endl;
			return false;
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
	void Texture2D::MakeCheckImage(unsigned char image[512][512][4])
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

	void Texture2D::Clear ()
	{
		if(m_flag[1]) {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (glIsTexture(m_id)) {
			glDeleteTextures(1, &m_id);
		}

		m_id = 0;
		m_flag.reset();
	}

}
