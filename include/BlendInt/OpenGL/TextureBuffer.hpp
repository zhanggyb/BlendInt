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

#ifndef _BLENDINT_OPENGL_TEXTUREBUFFER_HPP_
#define _BLENDINT_OPENGL_TEXTUREBUFFER_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class TextureBuffer: public Object
	{
	public:

		TextureBuffer ();

		virtual ~TextureBuffer ();

		void SetCoord (GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1);

		GLTexture2D* texture () const
		{
			return m_texture.get();
		}

		void Draw (GLfloat x, GLfloat y);

	private:

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_vbo;

		RefPtr<GLTexture2D> m_texture;
	};

}

#endif /* _BLENDINT_OPENGL_TEXTUREBUFFER_HPP_ */
