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


#ifndef _BLENDINT_PIXELICON_HPP_
#define _BLENDINT_PIXELICON_HPP_

#include <vector>

#include <BlendInt/AbstractForm.hpp>

namespace BlendInt {

	/**
	 * Icon displayed with pixels
	 */
	class PixelIcon: public AbstractForm
	{
	public:

		PixelIcon ();

		PixelIcon (const unsigned char* pixels, int width, int height);

		PixelIcon (const PixelIcon& orig);

		~PixelIcon ();

		void set_pixels (const unsigned char* pixels);

		const unsigned char* pixels () const {return &m_pixels[0];}

		void scale (float ratio);

		void scale (int width, int height);

		void display ();

		void display (float x, float y);

	protected:

		virtual void update (int type, const void* data);

		virtual void render ();

	private:

		std::vector<unsigned char> m_pixels;

		static const int default_icon_size = 16;	// 16 x 16

		// disabled
		PixelIcon& operator = (const PixelIcon& orig);
	};
}

#endif /* _BIL_ICON_HPP_ */
