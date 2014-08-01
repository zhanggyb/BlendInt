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

	using Stock::Shaders;

	Color CVImageView::background_color = Color(Color::SlateGray);

	CVImageView::CVImageView ()
	: AbstractScrollable()
	{
		InitializeCVImageView();
	}

	CVImageView::~CVImageView ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	void CVImageView::Open (const char* filename)
	{
		image_ = cv::imread(filename);

		if(image_.data) {

			texture_->Bind();
			switch (image_.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, image_.cols, image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, image_.cols, image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				default:
					break;
			}
			texture_->Reset();

			AdjustImageArea(size());
		}
	}

	void CVImageView::Load (const cv::Mat& image)
	{

	}

	void CVImageView::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(), request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
	}

	void CVImageView::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			background_->Bind();
			tool.SetInnerBufferData(background_.get());
			background_->Reset();

			set_size(*request.size());
			AdjustImageArea(*request.size());

			AdjustScrollBarGeometries(position().x(), position().y(), request.size()->width(), request.size()->height());
		}

		ReportSizeUpdate(request);
	}

	ResponseType CVImageView::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color", background_color.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		texture_->Bind();

		if (texture_->GetWidth() > 0) {

			program = Shaders::instance->image_program();
			program->Use();
			program->SetUniform1i("TexID", 0);
			program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
			program->SetUniform1i("u_gamma", 0);

			glBindVertexArray(vaos_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		texture_->Reset();
		program->Reset();

		DispatchDrawEvent(hbar(), event);
		DispatchDrawEvent(vbar(), event);

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

		if(image_.data) {
			prefer.reset(image_.cols, image_.rows);
		}

		return prefer;
	}

	void CVImageView::InitializeCVImageView ()
	{
		set_size(400, 300);

		texture_.reset(new GLTexture2D);
		texture_->Generate();
		texture_->Bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		texture_->Reset();

		glGenVertexArrays(2, vaos_);
		glBindVertexArray(vaos_[0]);

		background_.reset(new GLArrayBuffer);
		background_->Generate();
		background_->Bind();

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		tool.SetInnerBufferData(background_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vaos_[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		plane_.reset(new GLArrayBuffer);
		plane_->Generate();
		plane_->Bind();
		plane_->SetData(sizeof(vertices), vertices);

		glEnableVertexAttribArray(Shaders::instance->image_attrib_coord());
		glEnableVertexAttribArray(Shaders::instance->image_attrib_uv());
		glVertexAttribPointer(Shaders::instance->image_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->image_attrib_uv(), 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		AdjustScrollBarGeometries(position().x(), position().y(), size().width(), size().height());
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
		if(hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

		return Ignore;
	}

	ResponseType CVImageView::MouseReleaseEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseReleaseEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseReleaseEvent(vbar(), event);
		}

		return Ignore;
	}

	ResponseType CVImageView::MouseMoveEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseMoveEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseMoveEvent(vbar(), event);
		}

		return Ignore;
	}

	void CVImageView::AdjustImageArea (const Size& size)
	{
		int w = std::min(size.width(), image_.cols);
		int h = std::min(size.height(), image_.rows);

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)image_.cols / image_.rows;
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

		plane_->Bind();
		plane_->SetData(sizeof(vertices), vertices);
		plane_->Reset();
	}

}

#endif	// __USE_OPENCV__
