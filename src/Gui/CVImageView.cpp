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

#ifdef __USE_OPENCV__

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
#include <opencv2/highgui/highgui.hpp>

#include <BlendInt/Gui/CVImageView.hpp>
#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	CVImageView::CVImageView ()
	: AbstractWidget()
	{
		m_background_color = Color(Color::SlateGray);
		InitializeCVImageView();
	}

	CVImageView::~CVImageView ()
	{
		glDeleteVertexArrays(2, m_vao);
	}

	void CVImageView::Open (const char* filename)
	{
		m_image = cv::imread(filename);

		if(m_image.data) {

			m_texture->Bind();
			switch (m_image.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					m_texture->SetImage(0, GL_RGB, m_image.cols, m_image.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, m_image.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					m_texture->SetImage(0, GL_RGBA, m_image.cols, m_image.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, m_image.data);
					break;
				}

				default:
					break;
			}
			m_texture->Reset();

			AdjustImageArea(size());
		}
	}

	void CVImageView::Load (const cv::Mat& image)
	{

	}

	void CVImageView::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {

				const Size* size_p = static_cast<const Size*>(request.data());

				m_background_buffer->Bind();
				VertexTool tool;
				tool.Setup(*size_p, 0, RoundNone, 0);
				tool.SetInnerBufferData(m_background_buffer.get());
				m_background_buffer->Reset();

				AdjustImageArea(*size_p);

				break;
			}

			default:
				break;
		}

	}

	ResponseType CVImageView::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		// draw background
		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4fv("Color", m_background_color.data());
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		m_texture->Bind();

		if (m_texture->GetWidth() > 0) {

			program = Shaders::instance->default_image_program();
			program->Use();
			program->SetUniform1i("TexID", 0);
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
			program->SetUniform1i("Gamma", 0);

			glBindVertexArray(m_vao[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		m_texture->Reset();
		program->Reset();

		return Accept;
	}
	
	bool CVImageView::IsExpandX () const
	{
		return true;
	}

	bool CVImageView::IsExpandY () const
	{
		return true;
	}

	Size CVImageView::GetPreferredSize () const
	{
		Size prefer(400, 300);

		if(m_texture && glIsTexture(m_texture->texture())) {
			prefer.reset(m_image.cols, m_image.rows);
		}

		return prefer;
	}

	void CVImageView::SetBackgroundColor(const Color& color)
	{
		m_background_color = color;
		Refresh();
	}

	void CVImageView::InitializeCVImageView ()
	{
		set_size(400, 300);

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

	bool CVImageView::UpdateGeometryTest (const WidgetUpdateRequest& request)
	{
		return true;
	}

	void CVImageView::BroadcastUpdate (const WidgetUpdateRequest& request)
	{
	}

	ResponseType CVImageView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType CVImageView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType CVImageView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType CVImageView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType CVImageView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType CVImageView::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType CVImageView::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType CVImageView::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void CVImageView::AdjustImageArea (const Size& size)
	{
		int w = std::min(size.width(), m_image.cols);
		int h = std::min(size.height(), m_image.rows);

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)m_image.cols / m_image.rows;
			if(ratio > ref_ratio) {
				w = h * ref_ratio;
			} else if (ratio < ref_ratio) {
				h = w / ref_ratio;
			}
		}

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

#endif	// __USE_OPENCV__
