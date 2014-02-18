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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_SCREENBUFFER_HPP_
#define _BLENDINT_SCREENBUFFER_HPP_

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

namespace BlendInt {

	class Interface;

	/**
	 * @brief A special class used in Interface only to work as a screen buffer
	 *
	 */
	class ScreenBuffer: public Object
	{
	public:

		friend class Interface;

		void Render ();

	private:

		ScreenBuffer ();

		~ScreenBuffer ();

		void InitOnce ();

		void Resize (float width, float height, float depth = 0.0);

		void RenderToTexture ();

		GLTexture2D* m_texture;

		GLSLProgram* m_program;

		GLArrayBuffer* m_vbo;
		GLArrayBuffer* m_tbo;	// texture coords

		GLint uniform_texture;
		GLint attribute_coord3d;
		GLint attribute_texcoord;

		static const char* vertex_shader;
		static const char* fragment_shader;

	};

}

#endif /* _BLENDINT_SCREENBUFFER_HPP_ */
