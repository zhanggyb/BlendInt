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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <algorithm>
#include <opencv2/highgui/highgui.hpp>

#include <core/image.hpp>

#include <opengl/opengl.hpp>

#include <gui/image-viewport.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Color ImageViewport::background_color = Color(Palette::Gray);

	ImageViewport::ImageViewport ()
	: AbstractRoundFrame(),
	  vao_(0)
	{
		set_size(640, 480);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		texture_.reset(new GLTexture2D);
		InitializeImageViewport();
	}

	ImageViewport::~ImageViewport ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	bool ImageViewport::IsExpandX () const
	{
		return true;
	}

	bool ImageViewport::IsExpandY () const
	{
		return true;
	}

	Size ImageViewport::GetPreferredSize () const
	{
		Size prefer(640, 480);

		if(texture_ && glIsTexture(texture_->id())) {
			texture_->bind();
			prefer.reset(texture_->GetWidth(), texture_->GetHeight());
			texture_->reset();
		}

		return prefer;
	}

	bool ImageViewport::OpenFile (const char* filename)
	{
		bool retval = false;

		Image image;

		if(image.Read(filename)) {

			image_plane_.bind();
			float* ptr = (float*)image_plane_.map(GL_READ_WRITE);
			*(ptr + 4) = image.width();
			*(ptr + 9) = image.height();
			*(ptr + 12) = image.width();
			*(ptr + 13) = image.height();
			image_plane_.unmap();
			image_plane_.reset();

			if(!texture_) {
				texture_.reset(new GLTexture2D);
				texture_->generate();
				texture_->bind();
				texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
				texture_->SetMinFilter(GL_LINEAR);
				texture_->SetMagFilter(GL_LINEAR);
				texture_->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			} else if(texture_->id() == 0) {
				texture_->generate();
				texture_->bind();
				texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
				texture_->SetMinFilter(GL_LINEAR);
				texture_->SetMagFilter(GL_LINEAR);
				texture_->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			} else {
				texture_->bind();
			}

			switch(image.channels()) {
				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, image.width(),
					        image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
					        image.pixels());
					retval = true;
					break;
				}

				case 4: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, image.width(),
					        image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					        image.pixels());
					retval = true;
					break;
				}

				default:
					break;
			}

			texture_->reset();
		}

		return retval;
	}

	bool ImageViewport::SetTexture(const RefPtr<GLTexture2D>& texture)
	{
		bool retval = false;

		if(texture && glIsTexture(texture->id())) {

			texture_ = texture;
			texture_->bind();
			image_plane_.bind();
			float* ptr = (float*)image_plane_.map(GL_READ_WRITE);
			*(ptr + 4) = texture_->GetWidth();
			*(ptr + 9) = texture_->GetHeight();
			*(ptr + 12) = texture_->GetWidth();
			*(ptr + 13) = texture_->GetHeight();
			image_plane_.unmap();
			image_plane_.reset();
			texture_->reset();

			retval = true;
		}

		return retval;
	}

#ifdef __USE_OPENCV__
	bool ImageViewport::LoadCVImage (const cv::Mat& image)
	{
		bool retval = false;

		if(image.data) {

			image_plane_.bind();
			float* ptr = (float*)image_plane_.map(GL_READ_WRITE);
			*(ptr + 4) = image.cols;
			*(ptr + 9) = image.rows;
			*(ptr + 12) = image.cols;
			*(ptr + 13) = image.rows;
			image_plane_.unmap();
			image_plane_.reset();

			if(!texture_) {
				texture_.reset(new GLTexture2D);
				texture_->generate();
				texture_->bind();
				texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
				texture_->SetMinFilter(GL_LINEAR);
				texture_->SetMagFilter(GL_LINEAR);
				texture_->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			} else if(texture_->id() == 0) {
				texture_->generate();
				texture_->bind();
				texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
				texture_->SetMinFilter(GL_LINEAR);
				texture_->SetMagFilter(GL_LINEAR);
				texture_->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			} else {
				texture_->bind();
			}

			texture_->bind();
			switch (image.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, image.cols, image.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
					retval = true;
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, image.cols, image.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, image.data);
					retval = true;
					break;
				}

				default:
					break;
			}
			texture_->reset();
		}

		return retval;
	}
#endif


	void ImageViewport::PerformPositionUpdate(const AbstractView* source, const AbstractView* target, int x, int y)
	{
		if(target == this) {

			Point offset = GetOffset();
			float x = static_cast<float>(x + offset.x());
			float y = static_cast<float>(y + offset.y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat3(1.f), glm::vec2(x, y));

			set_position(x, y);

		}

		if(source == this) {
			report_position_update(source, target, x, y);
		}
	}

	void ImageViewport::PerformSizeUpdate(const AbstractView* source, const AbstractView* target, int width, int height)
	{
		if(target == this) {
			Point offset = GetOffset();
			float x = static_cast<float>(position().x() + offset.x());
			float y = static_cast<float>(position().y() + offset.y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)width,
				y,
				y + (float)height,
				100.f, -100.f);

			set_size(width, height);
			//AdjustImageArea(*request.size());

			RequestRedraw();
		}

		if(source == this) {
			report_size_update(source, target, width, height);
		}
	}

	bool ImageViewport::PreDraw(AbstractWindow* context)
	{
		DeclareActiveFrame(context, this);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
		AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

		return true;
	}

	Response ImageViewport::Draw (AbstractWindow* context)
	{
		if(texture_ && glIsTexture(texture_->id())) {

			glActiveTexture(GL_TEXTURE0);
			texture_->bind();

			float w = texture_->GetWidth();
			float h = texture_->GetHeight();

			AbstractWindow::shaders()->widget_image_program()->use();

			glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
			glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION),
					(size().width() - w)/2.f,
					(size().height() - h) / 2.f);
			glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			texture_->reset();
			GLSLProgram::reset();

		}

		return Finish;
	}
	
	void ImageViewport::PostDraw(AbstractWindow* context)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, context->size().width(), context->size().height());
	}

	void ImageViewport::InitializeImageViewport ()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			640.f, 0.f, 	1.f, 1.f,
			0.f, 480.f,		0.f, 0.f,
			640.f, 480.f,	1.f, 0.f
		};

		image_plane_.generate();
		image_plane_.bind();
		image_plane_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_COORD));
		glEnableVertexAttribArray(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_UV));
		glVertexAttribPointer(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_COORD), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_UV), 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		image_plane_.reset();

		texture_->generate();
		texture_->bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		texture_->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		texture_->reset();

	}

}

#endif	// __USE_OPENCV__
