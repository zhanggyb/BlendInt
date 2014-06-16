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

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/Gui/Icon.hpp>

namespace BlendInt {

	/**
	 * Icon displayed with pixels
	 */
	class PixelIcon: public Icon
	{
	public:

		PixelIcon (int width, int height);

		PixelIcon (int width, int height, const unsigned char* pixels);

		PixelIcon (int width, int height, const RefPtr<GLTexture2D>& texture, const GLfloat* uv);

		PixelIcon (const PixelIcon& orig);

		~PixelIcon ();

		void SetPixels (const unsigned char* pixels);

		void SetTexture (const RefPtr<GLTexture2D>& texture, const GLfloat* uv);

		virtual void Draw (const glm::mat4& mvp, short gamma = 0);

		virtual void Draw (const glm::mat4& mvp, int x, int y, int restrict_width, int restrict_height);

	protected:

		virtual void UpdateGeometry (const UpdateRequest& request);

	private:

		GLuint m_vao;

		/**
		 * @brief Coord and UV vertex buffer
		 */
		RefPtr<GLArrayBuffer> m_buffer;

		RefPtr<GLTexture2D> m_texture;

		// disabled
		PixelIcon& operator = (const PixelIcon& orig);
	};
}

#endif /* _BIL_ICON_HPP_ */
