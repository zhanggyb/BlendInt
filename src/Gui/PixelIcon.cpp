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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/PixelIcon.hpp>
#include <BlendInt/Stock/ShaderManager.hpp>

namespace BlendInt {

	PixelIcon::PixelIcon (int width, int height)
	: Icon(),
	  m_vao(0)
	{
		set_size(width, height);

		glGenVertexArrays(1, &m_vao);

		GLfloat vertices[] = {
						// x, 	y, 	u, 	v
						0.f, 0.f, 0.f, 0.f,
						(GLfloat)width, 0.f, 1.f, 0.f,
						0.f, (GLfloat)height, 0.f, 1.f,
						(GLfloat)width, (GLfloat)height, 1.f, 1.f
		};

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();
		m_buffer->Bind();
		m_buffer->SetData(sizeof(vertices), vertices);
		m_buffer->Reset();
	}

	PixelIcon::PixelIcon (int width, int height, const unsigned char* pixels)
	: Icon(),
	  m_vao(0)
	{
		set_size(width, height);

		glGenVertexArrays(1, &m_vao);

		GLfloat vertices[] = {
						// x, 	y, 	u, 	v
						0.f, 0.f, 0.f, 0.f,
						(GLfloat)width, 0.f, 1.f, 0.f,
						0.f, (GLfloat)height, 0.f, 1.f,
						(GLfloat)width, (GLfloat)height, 1.f, 1.f
		};

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();
		m_buffer->Bind();
		m_buffer->SetData(sizeof(vertices), vertices);
		m_buffer->Reset();

		m_texture.reset(new GLTexture2D);
		m_texture->Generate();
		m_texture->Bind();
		m_texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		m_texture->SetMinFilter(GL_LINEAR);
		m_texture->SetMagFilter(GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		m_texture->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		m_texture->Reset();
	}

	PixelIcon::PixelIcon (int width, int height,
					const RefPtr<GLTexture2D>& texture, const GLfloat* uv)
	: Icon(),
	  m_vao(0)
	{
		set_size(width, height);

		glGenVertexArrays(1, &m_vao);

		GLfloat vertices[] = {
						// x, 	y, 	u, 	v
						0.f, 0.f, *(uv + 0), *(uv + 1),
						(GLfloat)width, 0.f, *(uv + 2), *(uv + 3),
						0.f, (GLfloat)height, *(uv + 4), *(uv + 5),
						(GLfloat)width, (GLfloat)height, *(uv + 6), *(uv + 7)
		};

		m_buffer.reset(new GLArrayBuffer);
		m_buffer->Generate();
		m_buffer->Bind();
		m_buffer->SetData(sizeof(vertices), vertices);
		m_buffer->Reset();

		m_texture = texture;
	}

	PixelIcon::PixelIcon (const PixelIcon& orig)
	: Icon(),
	  m_vao(orig.m_vao)
	{
		m_buffer = orig.m_buffer;
		m_texture = orig.m_texture;
		set_size(orig.size());
	}

	PixelIcon::~PixelIcon ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void PixelIcon::SetPixels (const unsigned char* pixels)
	{

	}

	void PixelIcon::SetTexture (const RefPtr<GLTexture2D>& texture,
					const GLfloat* uv)
	{
	}

	void PixelIcon::Draw (const glm::mat4& mvp, short gamma)
	{
		if(m_texture) {
			glBindVertexArray(m_vao);

			RefPtr<GLSLProgram> program = ShaderManager::instance->default_image_program();

			program->Use();
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
			program->SetUniform1i("Gamma", gamma);

			glActiveTexture(GL_TEXTURE0);
			program->SetUniform1i("TexID", 0);

			m_texture->Bind();

			glEnableVertexAttribArray(0);	// 0: Coord
			glEnableVertexAttribArray(1);	// 1: Texture UV
			m_buffer->Bind();

			glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(2 * sizeof(GLfloat)));

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			m_buffer->Reset();
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);

			m_texture->Reset();
			program->Reset();

			glBindVertexArray(0);
		}
	}
	
	void PixelIcon::Draw (const glm::mat4& mvp, int x, int y,
			int restrict_width, int restrict_height)
	{
	}

	void PixelIcon::UpdateGeometry(const UpdateRequest& request)
	{

	}
	
	PixelIcon& PixelIcon::operator = (const PixelIcon& orig)
	{
		return *this;
	}

}
