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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif  // __UNIX__
#include <vector>

#include <iostream>

#include <BlendInt/GLFramebuffer.hpp>

namespace BlendInt {

	GLFramebuffer::GLFramebuffer()
	: m_id(0)
	{

	}

	GLFramebuffer::~GLFramebuffer ()
	{
		Clear();
	}

	void GLFramebuffer::Generate ()
	{
		if(!m_flag[0]) {
			glGenFramebuffers (1, &m_id);
			m_flag.set(0);
		}
	}

	void GLFramebuffer::Bind ()
	{
		if(!m_flag[0]) {
			std::cerr
			        << "The framebuffer is not generated, call GLFramebuffer::Generate() first!"
			        << std::endl;
			return;
		} else if (!m_flag[1]) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			m_flag.set(1);
		}
	}

	void GLFramebuffer::Unbind ()
	{
		if(m_flag[1]) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			m_flag.reset(1);
		}
	}

	void GLFramebuffer::Clear ()
	{
		if(m_flag[1]) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		if(glIsFramebuffer(m_id)) {
			glDeleteFramebuffers(1, &m_id);
		}

		m_id = 0;
		m_flag.reset();
	}

}
