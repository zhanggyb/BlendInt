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

#include <algorithm>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	ImageView::ImageView ()
	: AbstractWidget()
	{
		InitOnce();
	}

	ImageView::~ImageView ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void ImageView::Open (const char* filename)
	{
		Image image;

		if(image.Read(filename)) {
			m_texture->Bind();

			switch(image.channels()) {
				case 3:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					m_texture->SetImage(0, GL_RGB, image.width(),
					        image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				case 4:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					m_texture->SetImage(0, GL_RGBA, image.width(),
					        image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				default:
					break;
			}

			m_texture->Reset();

			m_image_size.set_width(image.width());
			m_image_size.set_height(image.height());

			AdjustImageArea(size());
		}
	}

	void ImageView::Load (const RefPtr<Image>& image)
	{
	}

	void ImageView::Clear()
	{

	}

	bool ImageView::IsExpandX () const
	{
		return true;
	}

	bool ImageView::IsExpandY () const
	{
		return true;
	}

	Size ImageView::GetPreferredSize () const
	{
		if(m_texture && glIsTexture(m_texture->texture())) {
			return m_image_size;
		}

		return Size(400, 300);
	}

	void ImageView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			m_background_buffer->Bind();
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			tool.SetInnerBufferData(m_background_buffer.get());
			m_background_buffer->Reset();

			AdjustImageArea (*request.size());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType ImageView::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glm::vec3 pos(0.f);
		pos.x = position().x() + (size().width() - m_checkerboard->size().width()) / 2.f;
		pos.y = position().y() + (size().height() - m_checkerboard->size().height()) / 2.f;

		// draw checkerboard
		m_checkerboard->Draw(pos);

		glActiveTexture(GL_TEXTURE0);
		m_texture->Bind();

		if (m_texture->GetWidth() > 0) {
			program = Shaders::instance->default_image_program();
			program->Use();
			program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
			program->SetUniform1i("TexID", 0);
			program->SetUniform1i("u_gamma", 0);

			glBindVertexArray(m_vao[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		m_texture->Reset();
		program->Reset();

		return Accept;
	}
	
	void ImageView::InitOnce ()
	{
		set_size(400, 300);

		m_checkerboard.reset(new CheckerBoard(20));
		m_checkerboard->Resize(size());

		m_texture.reset(new GLTexture2D);
		m_texture->Generate();
		m_texture->Bind();
		m_texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		m_texture->SetMinFilter(GL_LINEAR);
		m_texture->SetMagFilter(GL_LINEAR);
		m_texture->Reset();

		glGenVertexArrays(2, m_vao);
		glBindVertexArray(m_vao[0]);

		m_background_buffer.reset(new GLArrayBuffer);
		m_background_buffer->Generate();
		m_background_buffer->Bind();

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		tool.SetInnerBufferData(m_background_buffer.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(m_vao[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		m_image_buffer.reset(new GLArrayBuffer);
		m_image_buffer->Generate();
		m_image_buffer->Bind();
		m_image_buffer->SetData(sizeof(vertices), vertices);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	ResponseType ImageView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ImageView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ImageView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void ImageView::AdjustImageArea(const Size& size)
	{
		if(m_image_size.width() == 0 || m_image_size.height() == 0) {
			m_checkerboard->Resize(size);
			return;
		}

		int w = std::min(size.width(), m_image_size.width());
		int h = std::min(size.height(), m_image_size.height());

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)m_image_size.width() / m_image_size.height();
			if(ratio > ref_ratio) {
				w = h * ref_ratio;
			} else if (ratio < ref_ratio) {
				h = w / ref_ratio;
			}
		}

		m_checkerboard->Resize(w, h);

		GLfloat x = (size.width() - w) / 2.f;
		GLfloat y = (size.height() - h) / 2.f;

		GLfloat vertices[] = {
			x, y,	0.f, 1.f,
			x + (GLfloat)w, y,		1.f, 1.f,
			x, y + (GLfloat)h,		0.f, 0.f,
			x + (GLfloat)w, y + (GLfloat)h,		1.f, 0.f
		};

		m_image_buffer->Bind();
		m_image_buffer->SetData(sizeof(vertices), vertices);
		m_image_buffer->Reset();
	}

}
