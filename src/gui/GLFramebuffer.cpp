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
#include <gl.h>
#include <glext.h>
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
		glDeleteFramebuffers(1, &m_id);
	}

	void GLFramebuffer::Generate ()
	{
		if(m_id) {
			Clear();
		}

		glGenFramebuffers (1, &m_id);
	}

	void GLFramebuffer::Bind()
	{
		if(m_id) {
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		} else {
			std::cerr << "The frame buffer is not generated! call Generate() first." << std::endl;
		}
	}

	bool GLFramebuffer::Attach (const GLTexture2D& tex, GLenum attachment)
	{
		GLenum error = GL_NO_ERROR;

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex.id(), tex.level());

		error = glGetError();

		switch(error) {

			case GL_INVALID_ENUM: {
				std::cerr << "Target is not one of the accepted tokens."
						"Or the render buffer target is not GL_RENDERBUFFER."
						<< std::endl;
				break;
			}

			case GL_INVALID_OPERATION: {
				std::cerr << "Zero is bound to target."
						<< "Or the texture target and texture are not compatible." << std::endl;
				break;
			}

			default:
				break;
		}

		return error == GL_NO_ERROR ? true : false;
	}

	bool GLFramebuffer::Attach (const GLRenderbuffer& renderbuffer, GLenum attachment)
	{
		GLenum error = GL_NO_ERROR;

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.id());

		error = glGetError();

		switch(error) {

			case GL_INVALID_ENUM: {
				std::cerr << "Target is not one of the accepted tokens."
						"Or the render buffer target is not GL_RENDERBUFFER."
						<< std::endl;
				break;
			}

			case GL_INVALID_OPERATION: {
				std::cerr << "Zero is bound to target." << std::endl;
				break;
			}

			default:
				break;
		}

		return error == GL_NO_ERROR ? true : false;
	}

	void GLFramebuffer::Reset ()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLFramebuffer::Clear ()
	{
		//if(glIsFramebuffer(m_id)) {
		glDeleteFramebuffers(1, &m_id);
		//}

		m_id = 0;
	}

}
