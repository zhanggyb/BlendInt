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

	Color CVImageView::background_color = Color(Color::Gray);

	CVImageView::CVImageView ()
	: AbstractScrollable()
	{
		InitializeCVImageView();
	}

	CVImageView::~CVImageView ()
	{
		glDeleteVertexArrays(2, vaos_);
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

		if(cv_image_.data) {
			prefer.reset(cv_image_.cols, cv_image_.rows);
		}

		return prefer;
	}

	void CVImageView::Open (const char* filename)
	{
		cv_image_ = cv::imread(filename);

		if(cv_image_.data) {

			texture_->bind();
			switch (cv_image_.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, cv_image_.cols, cv_image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, cv_image_.cols, cv_image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				default:
					break;
			}
			texture_->reset();

			AdjustImageArea(size());
		}
	}

	void CVImageView::Load (const cv::Mat& image)
	{
		cv_image_ = image;
		if(cv_image_.data) {

			texture_->bind();
			switch (cv_image_.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, cv_image_.cols, cv_image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, cv_image_.cols, cv_image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				default:
					break;
			}
			texture_->reset();

			AdjustImageArea(size());
		}
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
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			background_->bind();
			background_->set_data(tool.inner_size(), tool.inner_data());
			background_->reset();

			set_size(*request.size());
			AdjustImageArea(*request.size());

			AdjustScrollBarGeometries(position().x(), position().y(), request.size()->width(), request.size()->height());
		}

		ReportSizeUpdate(request);
	}

	ResponseType CVImageView::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR), background_color.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
		texture_->bind();

		if (texture_->GetWidth() > 0) {

			program = Shaders::instance->image_program();
			program->use();
			glUniform1i(Shaders::instance->location(Stock::IMAGE_TEXTURE), 0);
			glUniform3f(Shaders::instance->location(Stock::IMAGE_POSITION), (float) position().x(), (float) position().y(), 0.f);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_GAMMA), 0);

			glBindVertexArray(vaos_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		texture_->reset();
		program->reset();

		DispatchDrawEvent(hbar(), profile);
		DispatchDrawEvent(vbar(), profile);

		return Accept;
	}
	
	void CVImageView::InitializeCVImageView ()
	{
		set_size(400, 300);

		texture_.reset(new GLTexture2D);
		texture_->generate();
		texture_->bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		texture_->reset();

		glGenVertexArrays(2, vaos_);
		glBindVertexArray(vaos_[0]);

		background_.reset(new GLArrayBuffer);
		background_->generate();
		background_->bind();

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);
		background_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vaos_[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		image_plane_.reset(new GLArrayBuffer);
		image_plane_->generate();
		image_plane_->bind();
		image_plane_->set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::IMAGE_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::IMAGE_UV));
		glVertexAttribPointer(Shaders::instance->location(Stock::IMAGE_COORD), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::IMAGE_UV), 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::reset();

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
		if(hbar()->Contain(event.global_position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->Contain(event.global_position())) {
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
		int w = std::min(size.width(), cv_image_.cols);
		int h = std::min(size.height(), cv_image_.rows);

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)cv_image_.cols / cv_image_.rows;
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

		image_plane_->bind();
		image_plane_->set_data(sizeof(vertices), vertices);
		image_plane_->reset();
	}

	void CVImageView::AdjustScrollArea(const Size& size)
	{
		//cv_image_.cols;
	}

}

#endif	// __USE_OPENCV__
