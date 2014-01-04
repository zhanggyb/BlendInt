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
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

namespace BlendInt {

	/**
	 * @brief 2D Texture
	 */
	class Texture2D
	{
	public:

		Texture2D();

		Texture2D(GLsizei width, GLsizei height);

		~Texture2D();

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

		void SetImage (GLint level, GLint internalFormat, GLint border, GLenum format, GLenum type, const GLvoid* data);

		void Unbind ();

	private:

		void Clear();

		GLuint m_id;

		GLsizei m_width;
		GLsizei m_height;

		bool m_bind;
	};

}

#endif	// _BLENDINT_TEXTURE2D_HPP_
