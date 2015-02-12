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

#include <opengl/gl-texture2d.hpp>

namespace BlendInt {

	/**
	 * @brief A special GLTexture2D class to store M x N sub images
	 */
	class IconTexture: public GLTexture2D
	{
	public:

		IconTexture ();

		virtual ~IconTexture ();

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

}
