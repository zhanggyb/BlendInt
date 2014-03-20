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

#ifndef _BLENDINT_IMAGEVIEW_HPP_
#define _BLENDINT_IMAGEVIEW_HPP_

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class ImageView: public Widget
	{
	public:

		ImageView ();

		virtual ~ImageView ();

	protected:

		virtual void Draw (RedrawEvent* event);

	private:

		void InitOnce ();

		void makeCheckImage ();

		static const int checkImageWidth = 512;
		static const int checkImageHeight = 512;

		GLubyte _checkImage[checkImageHeight][checkImageWidth][4];

		GLuint m_vao;

		RefPtr<GLTexture2D> m_texture;
		RefPtr<GLSLProgram> m_program;
		RefPtr<GLArrayBuffer> m_vbo;
		RefPtr<GLArrayBuffer> m_tbo;	// texture coords

		static const char* vertex_shader;
		static const char* fragment_shader;

	};
}


#endif /* _BIL_IMAGEVIEW_HPP_ */
