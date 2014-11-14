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

#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Color CVImageViewport::background_color = Color(Color::Gray);

	CVImageViewport::CVImageViewport ()
	: Frame(),
	  vao_(0)
	{
		set_size(640, 480);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);

		InitializeCVImageView();
	}

	CVImageViewport::~CVImageViewport ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	bool CVImageViewport::IsExpandX () const
	{
		return true;
	}

	bool CVImageViewport::IsExpandY () const
	{
		return true;
	}

	Size CVImageViewport::GetPreferredSize () const
	{
		Size prefer(640, 480);

		if(cv_image_.data) {
			prefer.reset(cv_image_.cols, cv_image_.rows);
		}

		return prefer;
	}

	void CVImageViewport::OpenFile (const char* filename)
	{
		cv_image_ = cv::imread(filename);

		if(cv_image_.data) {

			image_plane_.bind();
			float* ptr = (float*)image_plane_.map();
			*(ptr + 4) = cv_image_.cols;
			*(ptr + 9) = cv_image_.rows;
			*(ptr + 12) = cv_image_.cols;
			*(ptr + 13) = cv_image_.rows;
			image_plane_.unmap();
			image_plane_.reset();

			texture_.bind();
			switch (cv_image_.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_.SetImage(0, GL_RGB, cv_image_.cols, cv_image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_.SetImage(0, GL_RGBA, cv_image_.cols, cv_image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				default:
					break;
			}
			texture_.reset();

			//AdjustImageArea(size());
		}
	}

	void CVImageViewport::LoadImage (const cv::Mat& image)
	{
		cv_image_ = image;
		if(cv_image_.data) {

			image_plane_.bind();
			float* ptr = (float*)image_plane_.map();
			*(ptr + 4) = cv_image_.cols;
			*(ptr + 9) = cv_image_.rows;
			*(ptr + 12) = cv_image_.cols;
			*(ptr + 13) = cv_image_.rows;
			image_plane_.unmap();
			image_plane_.reset();

			texture_.bind();
			switch (cv_image_.channels()) {

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_.SetImage(0, GL_RGB, cv_image_.cols, cv_image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_.SetImage(0, GL_RGBA, cv_image_.cols, cv_image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, cv_image_.data);
					break;
				}

				default:
					break;
			}
			texture_.reset();

			//AdjustImageArea(size());
		}
	}

	void CVImageViewport::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(request.position()->x() + offset().x());
			float y = static_cast<float>(request.position()->y() + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

			set_position(*request.position());

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void CVImageViewport::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			float x = static_cast<float>(position().x() + offset().x());
			float y = static_cast<float>(position().y() + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());
			//AdjustImageArea(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool CVImageViewport::PreDraw(Profile& profile)
	{
		if(!visiable()) return false;

		assign_profile_frame(profile);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetWidgetProjectionMatrix(projection_matrix_);
		Shaders::instance->SetWidgetModelMatrix(model_matrix_);

		return true;
	}

	ResponseType CVImageViewport::Draw (Profile& profile)
	{
		glActiveTexture(GL_TEXTURE0);
		texture_.bind();

		float w = texture_.GetWidth();
		float h = texture_.GetHeight();

		Shaders::instance->widget_image_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_IMAGE_TEXTURE), 0);
		glUniform2f(Shaders::instance->location(Stock::WIDGET_IMAGE_POSITION),
				(size().width() - w)/2.f,
				(size().height() - h) / 2.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_IMAGE_GAMMA), 0);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		texture_.reset();
		GLSLProgram::reset();

		return Accept;
	}
	
	void CVImageViewport::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, profile.context()->size().width(), profile.context()->size().height());
	}

	void CVImageViewport::InitializeCVImageView ()
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

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_IMAGE_COORD));
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_IMAGE_UV));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_IMAGE_COORD), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_IMAGE_UV), 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		image_plane_.reset();

		texture_.generate();
		texture_.bind();
		texture_.SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_.SetMinFilter(GL_LINEAR);
		texture_.SetMagFilter(GL_LINEAR);
		texture_.SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		texture_.reset();

	}

	void CVImageViewport::AdjustImageArea (const Size& size)
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

		image_plane_.bind();
		image_plane_.set_data(sizeof(vertices), vertices);
		image_plane_.reset();
	}

	void CVImageViewport::AdjustScrollArea(const Size& size)
	{
		//cv_image_.cols;
	}

}

#endif	// __USE_OPENCV__
