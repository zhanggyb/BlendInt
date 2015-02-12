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

#include <opengl/glarraybuffer.hpp>
#include <opengl/gl-texture2d.hpp>
#include <gui/abstract-icon.hpp>

namespace BlendInt {

	/**
	 * Icon displayed with pixels
	 */
	class PixelIcon: public AbstractIcon
	{
	public:

		PixelIcon (unsigned int width, unsigned int height);

		PixelIcon (unsigned int width, unsigned int height, const unsigned char* pixels, const GLfloat* uv = 0);

		PixelIcon (unsigned int width, unsigned int height, const RefPtr<GLTexture2D>& texture, const GLfloat* uv = 0);

		virtual ~PixelIcon ();

		void SetPixels (unsigned int width, unsigned int height, const unsigned char* pixels, const GLfloat* uv = 0);

		void SetTexture (unsigned int width, unsigned int height, const RefPtr<GLTexture2D>& texture, const GLfloat* uv = 0);

		virtual void Draw (float x, float y) const;

		virtual void Draw (float x, float y, short gamma = 0) const;

	protected:

		virtual void PerformSizeUpdate (const Size& size);

	private:

		/**
		 * @brief Create VBO, VAO and bind VBO to vao_
		 */
		void CreateVertexArray (unsigned int width, unsigned int height, const GLfloat* uv = 0);

		GLuint vao_;

		/**
		 * @brief Coord and UV vertex buffer
		 */
		RefPtr<GLArrayBuffer> vbo_;

		RefPtr<GLTexture2D> texture_;

		// disabled
		PixelIcon& operator = (const PixelIcon& orig);
	};
}
