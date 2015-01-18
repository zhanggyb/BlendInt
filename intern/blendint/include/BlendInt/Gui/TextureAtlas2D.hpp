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

#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	/**
	 * @brief A special GLTexture2D class to store M x N sub images
	 */
	class TextureAtlas2D: public GLTexture2D
	{
	public:

		TextureAtlas2D ();

		virtual ~TextureAtlas2D ();

		void Generate (int width,
						int height,
						short cell_x,
						short cell_y,
						short xoffset = 1,
						short yoffset = 1,
						short xspace = 1,
						short yspace = 1);

		bool SetSubImage (int index,
						int bitmap_width,
						int bitmap_rows,
						const unsigned char* bitmap_buffer,
						int* r_x,
						int* r_y,
						bool clear = false);

		bool SetSubImage (int hindex,
						int yindex,
						int bitmap_width,
						int bitmap_rows,
						const unsigned char* bitmap_buffer,
						int* r_x,
						int* r_y,
						bool clear = false);

		int GetMaxNumber () const;

		int GetColumns () const;

		int GetRows () const;

		short offset_x () const
		{
			return offset_x_;
		}

		short offset_y () const
		{
			return offset_y_;
		}

		short space_x () const
		{
			return space_x_;
		}

		short space_y () const
		{
			return space_y_;
		}

		short cell_width () const
		{
			return cell_width_;
		}

		short cell_height () const
		{
			return cell_height_;
		}

	private:

		short cell_width_;
		short cell_height_;

		short offset_x_;
		short offset_y_;

		short space_x_;
		short space_y_;
	};

	// ----------------------

	class TextureAtlas2DExt: public Object
	{

	public:

		TextureAtlas2DExt()
		: Object(),
		id_(0),
		width_(0),
		height_(0),
		last_x_(0),
		last_y_(0),
		space_(0)
		{}

		virtual ~TextureAtlas2DExt()
		{
			if(id_) glDeleteTextures(1, &id_);
		}

		inline void generate (GLsizei width, GLsizei height)
		{
			if(id_) clear ();

			glGenTextures(1, &id_);

			width_ = width;
			height_ = height;
		}

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
		 */
		bool Upload (int bitmap_width,
					 int bitmap_rows,
					 const unsigned char* bitmap,
					 int* x,
					 int* y);

	private:

		inline void clear()
		{
			glDeleteTextures(1, &id_);
			id_ = 0;

			last_x_ = 0;
			last_y_ = 0;
		}

		GLuint id_;

		GLsizei width_;

		GLsizei height_;

		GLsizei last_x_;

		GLsizei last_y_;

		GLsizei h_;
		
		GLsizei space_;

	};

}
