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
		Destroy(m_tbo);
		Destroy(m_vbo);
		Destroy(m_program);

		delete m_texture;
	}

	void ImageView::Draw ()
	{
		//glBlendFunc(GL_ONE, GL_ZERO);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ImageView::InitOnce ()
	{
//		glShadeModel(GL_FLAT);
		makeCheckImage();
//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		m_texture = new GLTexture2D;
//		m_texture->Generate();
//		m_texture->Bind();
//		m_texture->SetWrapMode(GL_REPEAT, GL_REPEAT);
//		m_texture->SetMinFilter(GL_LINEAR);
//		m_texture->SetMagFilter(GL_LINEAR);
//
//		m_texture->SetImage(checkImageWidth, checkImageHeight, _checkImage);
//
//		m_texture->Reset();

		RenderToTexture();

		float vertices[] = {
			0.0, 0.0, 0.0,
			checkImageWidth, 0.0, 0.0,
			checkImageWidth, checkImageHeight, 0.0,
			0.0, checkImageHeight, 0.0
		};

		m_vbo = new GLArrayBuffer;
		Retain(m_vbo);
		m_vbo->Generate();
		m_vbo->Bind();
		m_vbo->SetData(4, sizeof(float) * 3, vertices);
		m_vbo->Reset();

		m_program = new GLSLProgram;
		Retain(m_program);

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

		m_tbo = new GLArrayBuffer;
		Retain(m_tbo);

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

	void ImageView::RenderToTexture()
	{
		GLsizei width = checkImageWidth;
		GLsizei height = checkImageHeight;

		// Create and set texture to render to.
		GLTexture2D* tex = m_texture;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(width, height, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		        GL_TEXTURE_2D, tex->id(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		        width, height);

		//-------------------------

//		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		        GL_RENDERBUFFER, rb);

		//-------------------------
		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
				std::cout << "good" << std::endl;
				break;
			default:
				std::cerr << "Fail to check framebuffer status" << std::endl;
				break;
		}

		//-------------------------
		//and now render to GL_TEXTURE_2D
		fb->Bind();

		glClearColor(0.0, 0.0, 0.0, 0.0);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// ---------------------------------------------

		glTranslatef(50, 50, 0);

		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;
		Size rsize(200, 200);

		vert_sum = generate_round_vertices(&rsize, DefaultBorderWidth(), RoundAll, 5.0, inner_v, outer_v);

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; // + 2 because the last pair is wrapped

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, inner_v);
		glDrawArrays(GL_POLYGON, 0, vert_sum.total);
		glDisableClientState(GL_VERTEX_ARRAY);

		// draw outline
		unsigned char tcol[4] = {themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		//tcol[3] = 225;
		glColor4ubv(tcol);

		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, quad_strip);
			glDrawArrays(GL_QUAD_STRIP, 0, vert_sum.total * 2 + 2);
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisable(GL_BLEND);

		// ---------------------------------------------
		// tex->WriteToFile("imageview.png");

		//Bind 0, which means render to back buffer
		fb->Reset();
		tex->Reset();

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		//Bind 0, which means render to back buffer, as a result, fb is unbound
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		fb->Reset();
		delete fb; fb = 0;

	}

}
