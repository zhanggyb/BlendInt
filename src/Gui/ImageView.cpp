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

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/Service/Theme.hpp>

#include <BlendInt/Gui/ImageView.hpp>

namespace BlendInt {

#ifdef __OPENGL_CORE_330__

	const char* ImageView::vertex_shader =
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

	const char* ImageView::fragment_shader =
			"#version 330\n"
			"in vec2 f_texcoord;"
			"uniform sampler2D TexID;"
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = texture(TexID, f_texcoord);"
			"}";

#else	// legacy opengl

	const char* ImageView::vertex_shader = "#version 120\n"
			"attribute vec3 coord3d;"
			"attribute vec2 texcoord;"
			"uniform mat4 MVP;"
			"varying vec2 f_texcoord;"
			""
			"void main(void) {"
			"	gl_Position = MVP * vec4(coord3d, 1.0);"
			"	f_texcoord = texcoord;"
			"}";

	const char* ImageView::fragment_shader = "varying vec2 f_texcoord;"
			"uniform sampler2D tex;"
			""
			"void main(void) {"
			"	gl_FragColor = texture2D(tex, f_texcoord);"
			"}";

#endif

	ImageView::ImageView ()
	: Widget(), m_vao(0)
	{
		InitOnce();
	}

	ImageView::~ImageView ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	ResponseType ImageView::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		m_checkerboard->Draw(mvp);

		glBindVertexArray(m_vao);

		m_program->Use();

		glActiveTexture(GL_TEXTURE0);
		m_texture->Bind();

		if (m_texture->GetWidth() > 0) {
			m_program->SetUniform1i("TexID", 0);
			m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
			        glm::value_ptr(mvp));

			glEnableVertexAttribArray(0);
			m_vbo->Bind();
			glVertexAttribPointer(0,
							2,
							GL_FLOAT,
							GL_FALSE,
							0,
							BUFFER_OFFSET(0));

			glEnableVertexAttribArray(1);
			m_tbo->Bind();
			glVertexAttribPointer(1,
							2,
							GL_FLOAT,
							GL_FALSE,
							0,
							BUFFER_OFFSET(0));

			m_vbo->Bind();

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(0);

			m_vbo->Reset();
			m_texture->Reset();
		}
		m_program->Reset();

		glBindVertexArray(0);

		return Accept;
	}
	
	void ImageView::Open (const char* filename)
	{
		RefPtr<Image> image(new Image);

		if(image->Read(filename)) {
			m_texture->Bind();
			switch(image->channels()) {
				case 3:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					m_texture->SetImage(0, GL_RGB, image->width(), image->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels());
					break;

				case 4:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					m_texture->SetImage(0, GL_RGBA, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels());
					break;

				default:
					break;
			}

			m_texture->Reset();

			set_preferred_size(image->width(), image->height());
			set_size(image->width(), image->height());
			m_checkerboard->Resize(size());

			GLfloat w = static_cast<GLfloat>(size().width());
			GLfloat h = static_cast<GLfloat>(size().height());

			GLfloat vertices[] = {
				0.0, 0.0,
				w, 0.0,
				0.0, h,
				w, h
			};

			m_vbo->Bind();
			m_vbo->SetData(sizeof(vertices), vertices);
			m_vbo->Reset();
		}
	}
	
	void ImageView::Open (const RefPtr<Image> image)
	{
	}

	void ImageView::InitOnce ()
	{
		set_size(400, 300);
		set_preferred_size(400, 300);

		m_checkerboard.reset(new CheckerBoard(20));
		m_checkerboard->Resize(size());

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_texture.reset(new GLTexture2D);
		m_texture->Generate();
		m_texture->Bind();
		m_texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		m_texture->SetMinFilter(GL_LINEAR);
		m_texture->SetMagFilter(GL_LINEAR);
		m_texture->Reset();

		m_program.reset(new GLSLProgram);
		m_program->Create();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);
		m_program->Link();

		GLfloat vertices[] = {
			0.0, 0.0,
			400.f, 0.0,
			0.0, 300.f,
			400.f, 300.f,
		};

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();
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

		glBindVertexArray(0);
	}

}
