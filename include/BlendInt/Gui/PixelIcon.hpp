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


#ifndef _BLENDINT_GUI_PIXELICON_HPP_
#define _BLENDINT_GUI_PIXELICON_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Gui/AbstractIcon.hpp>

namespace BlendInt {

	/**
	 * Icon displayed with pixels
	 */
	class PixelIcon: public AbstractIcon
	{
	public:

		PixelIcon (int width, int height);

		PixelIcon (int width, int height, const unsigned char* pixels, const GLfloat* uv = 0);

		PixelIcon (int width, int height, const RefPtr<GLTexture2D>& texture, const GLfloat* uv = 0);

		virtual ~PixelIcon ();

		void SetPixels (int width, int height, const unsigned char* pixels, const GLfloat* uv = 0);

		void SetTexture (int width, int height, const RefPtr<GLTexture2D>& texture, const GLfloat* uv = 0);

		virtual void Draw (const glm::vec3& pos, short gamma = 0) const;

	private:

		GLuint vao_;

		/**
		 * @brief Coord and UV vertex buffer
		 */
		RefPtr<GLArrayBuffer> buffer_;

		RefPtr<GLTexture2D> texture_;

		// disabled
		PixelIcon& operator = (const PixelIcon& orig);
	};
}

#endif /* _BIL_ICON_HPP_ */
