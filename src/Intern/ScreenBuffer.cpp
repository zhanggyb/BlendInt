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
#include <gl3.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include "ScreenBuffer.hpp"

namespace BlendInt {

#ifdef __OPENGL_CORE_330__
	const char* ScreenBuffer::vertex_shader =
			"#version 330\n"
			"layout(location = 0) in vec2 Coord2D;"
			"layout(location = 1) in vec2 UVCoord;"
			"uniform mat4 MVP;"
			"out vec2 f_texcoord;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(Coord2D, 0.0, 1.0);"
			"	f_texcoord = UVCoord;"
			"}";

	const char* ScreenBuffer::fragment_shader =
			"#version 330\n"
			"in vec2 f_texcoord;"
			"uniform sampler2D TexID;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = texture(TexID, f_texcoord);"
			"}";

#else	// use legacy opengl

	const char* ScreenBuffer::vertex_shader =
			"#version 120\n"
			"attribute vec3 coord3d;"
			"attribute vec2 texcoord;"
			"varying vec2 f_texcoord;"
			"uniform mat4 MVP;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(coord3d, 1.0);"
			"	f_texcoord = texcoord;"
			"}";

	const char* ScreenBuffer::fragment_shader =
			"varying vec2 f_texcoord;"
			"uniform sampler2D tex;"
			""
			"void main(void) {"
			"	gl_FragColor = texture2D(tex, f_texcoord);"
			"}";

#endif

	unsigned int ScreenBuffer::max_widgets_layer_buffer_size = 4;

	ScreenBuffer::ScreenBuffer()
	: Object(), m_vao(0)
	{
		InitOnce();
	}

	ScreenBuffer::~ScreenBuffer()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ScreenBuffer::Render(const glm::mat4& mvp, GLTexture2D* texture)
	{
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_vao);

		m_program->Use();

		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glActiveTexture(GL_TEXTURE0);
		texture->Bind();

		m_program->SetUniform1i("TexID", 0);

		glEnableVertexAttribArray(0);
		m_vbo->Bind();

		glVertexAttribPointer(
				0,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				BUFFER_OFFSET(0)
				);

		glEnableVertexAttribArray(1);
		m_tbo->Bind();
		glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				BUFFER_OFFSET(0)
				);

		m_vbo->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		m_vbo->Reset();
		texture->Reset();
		m_program->Reset();

		glBindVertexArray(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ScreenBuffer::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();

		GLfloat vertices[] = {
			0.0, 0.0,
			0.0, 0.0,
			0.0, 0.0,
			0.0, 0.0
		};

		m_vbo->Bind();
		m_vbo->SetData(sizeof(vertices), vertices);
		m_vbo->Reset();

		GLfloat uv[] = {
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0
		};

		m_tbo.reset(new GLArrayBuffer);
		m_tbo->Generate();
		m_tbo->Bind();
		m_tbo->SetData(sizeof(uv), uv);
		m_tbo->Reset();

		m_program.reset(new GLSLProgram);

		m_program->Create();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);
		m_program->Link();
		glBindVertexArray(0);
	}

	void ScreenBuffer::Resize (GLfloat width, GLfloat height)
	{
		GLfloat vertices[] = {
			0.f, 0.f,
			width, 0.f,
			0.f, height,
			width, height
		};

		glBindVertexArray(m_vao);
		m_vbo->Bind();
		m_vbo->UpdateData(vertices, sizeof(vertices));
		m_vbo->Reset();

		glBindVertexArray(0);
	}

	void ScreenBuffer::SaveToFile (const char* filename)
	{
		//m_texture->WriteToFile(filename);
	}


}
