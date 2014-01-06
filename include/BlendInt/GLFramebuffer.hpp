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

#ifndef _BLENDINT_GLFRAMEBUFFER_HPP_
#define _BLENDINT_GLFRAMEBUFFER_HPP_

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif
#endif  // __UNIX__
#include <vector>

#include <bitset>

namespace BlendInt {

	/**
	 * @brief C++ wrapper to GL Framebuffer objeect
	 *
	 * @ingroup opengl
	 */
	class GLFramebuffer
	{
	public:

		GLFramebuffer ();

		~GLFramebuffer ();

		void Generate ();

		void Bind ();

		void Unbind ();

		void Clear ();

		GLuint id () const {return m_id;}

	private:

		GLuint m_id;

		std::bitset<8> m_flag;

	};

}

#endif /* _BLENDINT_GLFRAMEBUFFER_HPP_ */
