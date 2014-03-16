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
			"layout(location = 0) in vec3 coord3d;"
			"layout(location = 1) in vec2 texcoord;"
			"uniform mat4 MVP;"
			"out vec2 f_texcoord;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(coord3d, 1.0);"
			"	f_texcoord = texcoord;"
			"}";

	const char* ScreenBuffer::fragment_shader =
			"#version 330\n"
			"in vec2 f_texcoord;"
			"uniform sampler2D tex;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = texture(tex, f_texcoord);"
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
	: Object(), m_vao(0), uniform_texture(-1), attribute_coord3d(-1), attribute_texcoord(-1)
	{
		InitOnce();
	}

	ScreenBuffer::~ScreenBuffer()
	{
		for(std::vector<WidgetsLayerBuffer>::iterator it = m_widgets_layer_buffers.begin(); it != m_widgets_layer_buffers.end(); it++)
		{
			(*it).texture_buffer->Clear();
			delete (*it).texture_buffer;
			(*it).texture_buffer = 0;
		}

		m_widgets_layer_buffers.clear();

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

		glUniform1i(uniform_texture, 0);

		m_vbo->Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				sizeof(GLfloat) * 5,
				BUFFER_OFFSET(0)
				);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				sizeof(GLfloat) * 5,
				BUFFER_OFFSET(3 * sizeof(GLfloat))
				);

		glDrawArrays(GL_TRIANGLES, 0, 2 * 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

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

		float vertices[] = {
			// 1, 2, 3: coordinates	4, 5: UV
			0.0, 0.0, 0.0,				0.f, 0.f,
			0.0, 0.0, 0,0,				1.f, 0.f,
			0.0, 0.0, 0.0,				1.f, 1.f,

			0.0, 0.0, 0.0,				0.f, 0.f,
			0.0, 0.0, 0,0,				1.f, 1.f,
			0.0, 0.0, 0.0,				0.f, 1.f,
		};

		m_vbo->Bind();
		m_vbo->SetData(sizeof(vertices), vertices);
		m_vbo->Reset();

		m_program.reset(new GLSLProgram);

		m_program->Create();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);

		if(m_program->Link()) {
			m_program->Use();

			attribute_coord3d = m_program->GetAttributeLocation("coord3d");
			attribute_texcoord = m_program->GetAttributeLocation("texcoord");
			uniform_texture = m_program->GetUniformLocation("tex");

			if(attribute_texcoord == -1 || attribute_coord3d == -1 || uniform_texture == -1) {
				std::cerr << "Fail to get attribute" << std::endl;
			}

			m_program->Reset();
		}

		glBindVertexArray(0);
	}

	void ScreenBuffer::Resize (float width, float height, float depth)
	{
		float vertices[] = {
			0.0, 0.0, depth,		0.f, 0.f,
			width, 0.0, depth,	1.f, 0.f,
			width, height, depth,1.f, 1.f,

			0.0, 0.0, depth,	0.f, 0.f,
			width, height, depth,	1.f, 1.f,
			0.0, height, depth,	0.f, 1.f
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
