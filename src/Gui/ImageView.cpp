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
#include <gl.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/Service/Theme.hpp>

#include <BlendInt/Gui/ImageView.hpp>

namespace BlendInt {

	const char* ImageView::vertex_shader =
			"#version 120\n"
			"attribute vec3 coord3d;"
			"attribute vec2 texcoord;"
			"varying vec2 f_texcoord;"
			""
			"void main(void) {"
			"	gl_Position = gl_ModelViewProjectionMatrix * vec4(coord3d, 1.0);"
			"	f_texcoord = texcoord;"
			"}";

	const char* ImageView::fragment_shader =
			"varying vec2 f_texcoord;"
			"uniform sampler2D texture;"
			""
			"void main(void) {"
			"	gl_FragColor = texture2D(texture, f_texcoord);"
			"}";

	ImageView::ImageView ()
	: Widget(), m_texture(0), m_program(0), m_vbo(0), m_tbo(0), uniform_texture(-1), attribute_coord3d(-1), attribute_texcoord(-1)
	{
		InitOnce();
	}

	ImageView::ImageView (AbstractWidget* parent)
	: Widget(parent), m_texture(0), m_program(0), m_vbo(0), m_tbo(0), uniform_texture(-1), attribute_coord3d(-1), attribute_texcoord(-1)
	{
		InitOnce();
	}

	ImageView::~ImageView ()
	{
	}

	void ImageView::Draw ()
	{
		m_program->Use();

		glActiveTexture(GL_TEXTURE0);
		m_texture->Bind();

		glUniform1i(uniform_texture, 0);

		glEnableVertexAttribArray(attribute_coord3d);
		m_vbo->Bind();
		glVertexAttribPointer(
				attribute_coord3d,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				0
				);

		glEnableVertexAttribArray(attribute_texcoord);
		m_tbo->Bind();
		glVertexAttribPointer(
				attribute_texcoord,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				0
				);

		m_vbo->Bind();

		glDrawArrays(GL_POLYGON, 0, 4);

		glDisableVertexAttribArray(attribute_texcoord);
		glDisableVertexAttribArray(attribute_coord3d);

		m_vbo->Reset();
		m_texture->Reset();
		m_program->Reset();

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size().width(), 0);
			glVertex2i(size().width(), size().height());
			glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif
	}

	void ImageView::InitOnce ()
	{
//		glShadeModel(GL_FLAT);
		makeCheckImage();
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		m_texture.reset(new GLTexture2D);
		m_texture->Generate();
		m_texture->Bind();
		m_texture->SetWrapMode(GL_REPEAT, GL_REPEAT);
		m_texture->SetMinFilter(GL_LINEAR);
		m_texture->SetMagFilter(GL_LINEAR);

		m_texture->SetImage(checkImageWidth, checkImageHeight, _checkImage);

		m_texture->Reset();

		float vertices[] = {
			0.0, 0.0, 0.0,
			checkImageWidth, 0.0, 0.0,
			checkImageWidth, checkImageHeight, 0.0,
			0.0, checkImageHeight, 0.0
		};

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();
		m_vbo->Bind();
		m_vbo->SetData(4, sizeof(float) * 3, vertices);
		m_vbo->Reset();

		m_program.reset(new GLSLProgram);

		m_program->Create();
		m_program->AttachShaderPair(vertex_shader, fragment_shader);

		if(m_program->Link()) {
			m_program->Use();
			attribute_coord3d = m_program->GetAttributeLocation("coord3d");
			attribute_texcoord = m_program->GetAttributeLocation("texcoord");
			uniform_texture = m_program->GetUniformLocation("texture");

			if(attribute_texcoord == -1 || attribute_coord3d == -1 || uniform_texture == -1) {
				std::cerr << "Fail to get attribute" << std::endl;
			}
		}
		m_program->Reset();

		GLfloat texcoords[] = {
				0.0, 0.0,
				1.0, 0.0,
				1.0, 1.0,
				0.0, 1.0
		};

		m_tbo.reset(new GLArrayBuffer);

		m_tbo->Generate();
		m_tbo->Bind();
		m_tbo->SetData(4, sizeof(GLfloat) * 2, texcoords);
		m_tbo->Reset();

		Resize(checkImageWidth, checkImageHeight);
	}

	void ImageView::makeCheckImage ()
	{
		int i, j, c;
		for (i = 0; i < checkImageHeight; i++) {
			for (j = 0; j < checkImageWidth; j++) {
				c = (((i & 0x8) == 0) ^ (((j & 0x8) == 0))) * (255);
				_checkImage[i][j][0] = (GLubyte) c;
				_checkImage[i][j][1] = (GLubyte) c;
				_checkImage[i][j][2] = (GLubyte) c;
				_checkImage[i][j][3] = (GLubyte) c;
			}
		}
	}

}
