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

	using Stock::Shaders;

	ImageView::ImageView ()
	: AbstractScrollable()
	{
		set_size(400, 300);

		InitializeImageView();
	}

	ImageView::~ImageView ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool ImageView::Open (const char* filename)
	{
		bool retval = false;

		Image image;

		if(image.Read(filename)) {
			texture_->bind();

			switch(image.channels()) {
				case 3:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, image.width(),
					        image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				case 4:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, image.width(),
					        image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				default:
					break;
			}

			texture_->reset();

			image_size_.set_width(image.width());
			image_size_.set_height(image.height());

			AdjustImageArea(size());

			retval = true;
		}

		return retval;
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
		if(texture_ && glIsTexture(texture_->id())) {
			return image_size_;
		}

		return Size(400, 300);
	}

	void ImageView::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(GetHScrollBar(), GetVScrollBar());
			Refresh();
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void ImageView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			background_->bind();
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			background_->set_data(tool.inner_size(), tool.inner_data());
			background_->reset();

			set_size(*request.size());

			AdjustImageArea (*request.size());

			AdjustScrollBarGeometries(GetHScrollBar(), GetVScrollBar());
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType ImageView::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), 0.f, 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glm::vec3 pos(0.f);
		pos.x = (size().width() - checkerboard_->size().width()) / 2.f;
		pos.y = (size().height() - checkerboard_->size().height()) / 2.f;

		// draw checkerboard
		checkerboard_->Draw(pos);

		glActiveTexture(GL_TEXTURE0);
		texture_->bind();

		if (texture_->GetWidth() > 0) {
			program = Shaders::instance->image_program();
			program->use();
			glUniform3f(Shaders::instance->location(Stock::IMAGE_POSITION), 0.f, 0.f, 0.f);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_TEXTURE), 0);
			glUniform1i(Shaders::instance->location(Stock::IMAGE_GAMMA), 0);

			glBindVertexArray(vaos_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		texture_->reset();
		program->reset();

		return AbstractScrollable::Draw(profile);
	}
	
	void ImageView::InitializeImageView ()
	{
		checkerboard_.reset(new ChessBoard(20));
		checkerboard_->Resize(size());

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
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vaos_[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		plane_.reset(new GLArrayBuffer);
		plane_->generate();
		plane_->bind();
		plane_->set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::IMAGE_COORD));
		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::IMAGE_UV));
		glVertexAttribPointer (Shaders::instance->location (Stock::IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Shaders::instance->location (Stock::IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));
		SetScrollBar(hbar, vbar);
		AdjustScrollBarGeometries(hbar, vbar);
	}

	void ImageView::AdjustImageArea(const Size& size)
	{
		if(image_size_.width() == 0 || image_size_.height() == 0) {
			//checkerboard_->Resize(size);
			return;
		}

		int w = std::min(size.width(), image_size_.width());
		int h = std::min(size.height(), image_size_.height());

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)image_size_.width() / image_size_.height();
			if(ratio > ref_ratio) {
				w = h * ref_ratio;
			} else if (ratio < ref_ratio) {
				h = w / ref_ratio;
			}
		}

		checkerboard_->Resize(w, h);

		GLfloat x = (size.width() - w) / 2.f;
		GLfloat y = (size.height() - h) / 2.f;

		GLfloat vertices[] = {
			x, y,	0.f, 1.f,
			x + (GLfloat)w, y,		1.f, 1.f,
			x, y + (GLfloat)h,		0.f, 0.f,
			x + (GLfloat)w, y + (GLfloat)h,		1.f, 0.f
		};

		plane_->bind();
		plane_->set_data(sizeof(vertices), vertices);
		plane_->reset();
	}

}
