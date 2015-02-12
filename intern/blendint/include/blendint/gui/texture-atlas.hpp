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
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#pragma once

#include <core/object.hpp>

namespace BlendInt {

	class TextureAtlas: public Object
	{

	public:

		TextureAtlas()
		: Object(),
		id_(0),
		width_(0),
		height_(0),
		last_x_(0),
		last_y_(0),
		last_row_height_(0)
		{}

		virtual ~TextureAtlas()
		{
			if(id_) glDeleteTextures(1, &id_);
		}

		void Generate (GLsizei width,
				GLsizei height);

		inline void bind () const
		{
			glBindTexture (GL_TEXTURE_2D, id_);
		}

		static inline void reset ()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/**
		 * @brief Upload glyph bitmap to the texture
		 * @param[in] bitmap_width
		 * @param[in] bitmap_rows
		 * @param[in] bitmap
		 * @param[out] x
		 * @param[out] y
		 *
		 * Upload glyph bitmap to texture and get the position in the
		 * texture for calculating UV.
		 *
		 * @note MUST call bind() before use this function
		 */
		bool Upload (int bitmap_width,
					 int bitmap_rows,
					 const unsigned char* bitmap,
					 int* ox,
					 int* oy);

	private:

		inline void clear()
		{
			glDeleteTextures(1, &id_);
			id_ = 0;

			width_ = 0;
			height_ = 0;
			last_x_ = 0;
			last_y_ = 0;
			last_row_height_ = 0;
		}

		GLuint id_;

		GLsizei width_;

		GLsizei height_;

		GLsizei last_x_;

		GLsizei last_y_;

		GLsizei last_row_height_;
		
	};

}
