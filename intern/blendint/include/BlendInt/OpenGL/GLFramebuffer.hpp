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

#pragma once

#include <vector>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/OpenGL/GLHeader.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

namespace BlendInt {

	/**
	 * @brief C++ wrapper to GL Framebuffer object
	 *
	 * Usage of this class:
	 *
	 * @ingroup opengl
	 */
	class GLFramebuffer
	{
	public:

		GLFramebuffer ();

		~GLFramebuffer ();

		inline void generate ()
		{
			if(m_id != 0) clear();
			glGenFramebuffers (1, &m_id);
		}

		inline void bind () const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		void Attach (const GLTexture2D& tex, GLenum attachment = GL_COLOR_ATTACHMENT0);

		void Attach (const GLRenderbuffer& renderbuffer, GLenum attachment = GL_COLOR_ATTACHMENT0);

		inline void clear ()
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}

		GLuint id () const {return m_id;}

		static inline void reset ()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		static bool CheckStatus ();

	private:

		GLuint m_id;

	};

}
