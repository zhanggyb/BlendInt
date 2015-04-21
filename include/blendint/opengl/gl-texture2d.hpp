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

#pragma once

#include <blendint/core/object.hpp>
#include <blendint/core/size.hpp>
#include <blendint/opengl/opengl.hpp>

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

		~GLTexture2D();

		inline GLuint id() const {return id_;}

		inline void generate ()
		{
			if(id_ != 0) clear();
			glGenTextures(1, &id_);
		}

		inline void bind () const
		{
			glBindTexture(GL_TEXTURE_2D, id_);
		}

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

		GLint GetWidth (GLint level = 0) const;

		GLint GetHeight (GLint level = 0) const;

		Size GetSize (GLint level = 0) const;

		void SetImage (GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data);

		void SetSubImage (GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* data);

		void CopySubimage (GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

		static inline void reset ()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/**
		 * @brief Write the texture to file
		 * @param path
		 * @return
		 */
		bool WriteToFile (const std::string& path, GLint level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

		inline void clear()
		{
			glDeleteTextures(1, &id_);
			id_ = 0;
		}

		static GLuint GetTextureBinding ();

	protected:

		inline void set_parameter (GLenum name, GLint value);

		inline void set_parameter (GLenum name, GLfloat value);

		inline void get_parameter (GLenum pname, GLfloat* params);

		inline void get_parameter (GLenum pname, GLint* params);

		inline void get_level_parameter (GLint level, GLenum pname, GLfloat* params);

		inline void get_level_parameter (GLint level, GLenum pname, GLint* params);

	private:

		GLuint id_;
	};

}
