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

#ifndef _BLENDINT_GUI_IMAGEPLANE2D_HPP_
#define _BLENDINT_GUI_IMAGEPLANE2D_HPP_

#include <BlendInt/Core/Object.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class ImagePlane2D: public Object
	{
	public:

		ImagePlane2D ();

		virtual ~ImagePlane2D ();

		void SetCoord (GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1);

		GLTexture2D* texture () const
		{
			return texture_.get();
		}

		void Draw (GLfloat x, GLfloat y);

	private:

		GLuint vao_;

		GLBuffer<> vertex_buffer_;

		RefPtr<GLTexture2D> texture_;
	};


}

#endif /* _BLENDINT_GUI_IMAGEPLANE2D_HPP_ */
