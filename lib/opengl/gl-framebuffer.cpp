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

#include <iostream>

#include <blendint/opengl/opengl.hpp>
#include <blendint/opengl/gl-framebuffer.hpp>

namespace BlendInt {

	GLFramebuffer::GLFramebuffer()
	: m_id(0)
	{

	}

	GLFramebuffer::~GLFramebuffer ()
	{
		glDeleteFramebuffers(1, &m_id);
	}

	void GLFramebuffer::Attach (const GLTexture2D& tex, GLenum attachment)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex.id(), 0);
	}

	void GLFramebuffer::Attach (const GLRenderbuffer& renderbuffer, GLenum attachment)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.id());
	}

	bool GLFramebuffer::CheckStatus()
	{
		GLenum status;
		bool ret = false;

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
				ret = true;
				break;

#ifdef DEBUG
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cerr << "The target is the default framebuffer, but the default framebuffer does not exist" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cerr << "Any of the framebuffer attachment points are framebuffer incomplete" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cerr << "The framebuffer does not have at least one image attached to it" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cerr << "The value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAWBUFFERi" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cerr << "GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER" << std::endl;
				break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cerr << "The combination of internal formats of the attached images violates an implementation-dependent set of restrictions" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cerr << "The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; "
				"If the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; "
				"or, if the attached images are a mix of renderbuffers and textures, "
				"the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES."
				"The value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures; "
				"or, if the attached images are a mix of renderbuffers and textures, "
				"the value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures" << std::endl;
				break;

			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cerr << "Any framebuffer attachment is layered, and any populated attachment is not layered, "
				"or if all populated color attachments are not from textures of the same target" << std::endl;
				break;
#endif	// DEBUG

			default:
				std::cerr << "Error status" << std::endl;
				break;
		}

		return ret;
	}

}
