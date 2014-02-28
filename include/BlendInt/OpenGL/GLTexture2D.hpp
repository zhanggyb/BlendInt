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

#ifndef _BLENDINT_TEXTURE2D_HPP_
#define _BLENDINT_TEXTURE2D_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	/**
	 * @brief C++ wrapper for 2D Texture
	 *
	 * @ingroup opengl
	 */
	class GLTexture2D: public Object
	{
	public:

		GLTexture2D();

		GLTexture2D(GLsizei width, GLsizei height);

		~GLTexture2D();

		inline GLuint id() const {return m_id;}

		void Generate ();

		void Bind ();

		void SetSize (GLsizei width, GLsizei height);

		/**
		 * @brief Get the index of the lowest defined mipmap level
		 * @return Index of the level
		 */
		GLint GetBaseLevel ();

		/**
		 * @brief Specifies the index of the lowest defined mipmap level.
		 * @param[in] level The index of the level, integer value, initial is 0
		 */
		void SetBaseLevel (GLint level);

		/**
		 * @brief Set the border color
		 * @param color
		 */
		void SetBorderColor (const GLint color[4]);

		/**
		 * @brief Set the border color
		 * @param color
		 */
		void SetBorderColor (const GLfloat color[4]);

		/**
		 * @brief Specifies the comparison operator used when GL_TEXTURE_COMPARE_MODE is set to GL_COMPARE_REF_TO_TEXTURE.
		 * @param value
		 */
		void SetCompareFunc (GLint func);

		/**
		 * @brief Specifies the texture comparison mode for currently bound depth textures
		 * @param mode
		 */
		void SetCompareMode (GLint mode);

		void SetLodBias (GLfloat value);

		void SetMinFilter (GLint filter);

		void SetMagFilter (GLint filter);

		/**
		 * @brief Sets the wrap parameter for texture coordinates
		 * @param[in] s_mode The wrap mode for S(X) coordinate
		 * @param[in] t_mode The wrap mode for T(Y) coordinate
		 *
		 * Both s_mode, t_mode should be one of:
		 * 	- GL_CLAMP_TO_EDGE
		 * 	- GL_CLAMP_TO_BORDER
		 * 	- GL_MIRRORED_REPEAT
		 * 	- GL_REPEAT
		 */
		void SetWrapMode (GLint s_mode, GLint t_mode);

		/**
		 * @brief Sets the wrap parameter for texture S(X) coordinate
		 * @param[in] mode The wrap mode for S(X) coordinate
		 *
		 * The parameter mode, should be one of:
		 * 	- GL_CLAMP_TO_EDGE
		 * 	- GL_CLAMP_TO_BORDER
		 * 	- GL_MIRRORED_REPEAT
		 * 	- GL_REPEAT
		 */
		void SetWrapModeS (GLint mode);

		/**
		 * @brief Sets the wrap parameter for texture T(Y) coordinate
		 * @param[in] mode The wrap mode for T(Y) coordinate
		 *
		 * The parameter mode, should be one of:
		 * 	- GL_CLAMP_TO_EDGE
		 * 	- GL_CLAMP_TO_BORDER
		 * 	- GL_MIRRORED_REPEAT
		 * 	- GL_REPEAT
		 */
		void SetWrapModeT (GLint mode);

		/**
		 * @brief Sets the minimal level-of-detail parameter
		 * @param value
		 */
		void SetMinLod (GLint value);

		/**
		 * @brief Sets the maximal level-of-detail parameter
		 * @param value
		 */
		void SetMaxLod (GLint value);

		/**
		 * @brief Sets the inex of the highest defined mipmap level.
		 * @param level Integer value, initial is 1000
		 */
		void SetMaxLevel (GLint level);

		void SetParameter (GLenum name, GLint value);

		void SetParameter (GLenum name, GLfloat value);

		void SetImage (GLsizei width, GLsizei height, const GLvoid* data);

		void CopySubimage (GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

		GLint level () const {return m_level;}

		static void Reset ();

		/**
		 * @brief Write the texture to file
		 * @param path
		 * @return
		 */
		bool WriteToFile (const std::string& path);

#ifdef DEBUG
		static void MakeCheckImage (unsigned char image[512][512][4]);
#endif

		void Clear();

	private:


		GLuint m_id;

		GLsizei m_width;
		GLsizei m_height;

		/**
		 * @brief The level-of-detail number
		 *
		 * Level 0 is the base image level. Level n is the nth mipmap reduction image.
		 *
		 * Default is 0.
		 */
		GLint m_level;

		/**
		 * @brief The number of color components in the texture
		 *
		 * Default is GL_RGBA8.
		 */
		GLint m_internal_format;

		/**
		 * @brief The format of the pixel data
		 *
		 * The following symbolic values are accepted:
		 * 	- GL_RED
		 * 	- GL_RG
		 * 	- GL_RGB
		 * 	- GL_BGR
		 * 	- GL_RGBA
		 * 	- GL_BGRA
		 *
		 * Default is GL_RGBA.
		 *
		 * This member variable is used in glTexImage2D() in SetData().
		 */
		GLenum m_format;

		/**
		 * @brief The data type of the pixel data
		 *
		 * The following symbolic values are accepted:
		 * 	- GL_UNSIGNED_BYTE
		 * 	- GL_BYTE
		 * 	- GL_UNSIGNED_SHORT
		 * 	- GL_SHORT
		 * 	- GL_UNSIGNED_INT
		 * 	- GL_INT
		 * 	- GL_FLOAT
		 * 	- GL_UNSIGNED_BYTE_3_3_2
		 * 	- GL_UNSIGNED_BYTE_2_3_3_REV
		 * 	- GL_UNSIGNED_SHORT_5_6_5
		 * 	- GL_UNSIGNED_SHORT_5_6_5_REV
		 * 	- GL_UNSIGNED_SHORT_4_4_4_4
		 * 	- GL_UNSIGNED_SHORT_4_4_4_4_REV
		 * 	- GL_UNSIGNED_SHORT_5_5_5_1
		 * 	- GL_UNSIGNED_SHORT_1_5_5_5_REV
		 * 	- GL_UNSIGNED_INT_8_8_8_8
		 * 	- GL_UNSIGNED_INT_8_8_8_8_REV
		 * 	- GL_UNSIGNED_INT_10_10_10_2
		 * 	- GL_UNSIGNED_INT_2_10_10_10_REV
		 *
		 * Default is GL_UNSIGNED_BYTE
		 *
		 * This member variable is used in glTexImage2D() in SetData().
		 */
		GLenum m_type;
	};

}

#endif	// _BLENDINT_TEXTURE2D_HPP_
