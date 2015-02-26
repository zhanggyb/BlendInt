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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <gui/cv-image-view.hpp>

namespace BlendInt {

	CVImageView::CVImageView()
	: AbstractScrollable(),
	  off_screen_context_(0),
	  playing_(false),
	  count(0)
	{
		set_size(400, 300);

		timer_.reset(new Timer);
		timer_->SetInterval(1000 / 15);	// default: 15 FPS

		events()->connect(timer_->timeout(), this, &CVImageView::OnUpdateFrame);

		MutexAttrib attrib;
		attrib.initialize();
		mutex_.initialize(attrib);
		attrib.destroy();

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		vbo_.generate();

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		vbo_.bind(0);
		vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vao_[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		vbo_.bind(1);
		vbo_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(AttributeCoord);
		glEnableVertexAttribArray(AttributeUV);
		glVertexAttribPointer(AttributeCoord, 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(AttributeUV, 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_.reset();

		texture_.generate();
		texture_.bind();
		texture_.SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_.SetMinFilter(GL_LINEAR);
		texture_.SetMagFilter(GL_LINEAR);
		texture_.reset();
	}

	CVImageView::~CVImageView ()
	{
		if(video_stream_.isOpened()) {
			video_stream_.release();
			image_.release();
		}

		if(off_screen_context_) {
			delete off_screen_context_;
			off_screen_context_ = 0;
		}

		glDeleteVertexArrays(2, vao_);

		mutex_.destroy();
	}

	bool CVImageView::IsExpandX() const
	{
		return true;
	}

	bool CVImageView::IsExpandY() const
	{
		return true;
	}

	Size CVImageView::GetPreferredSize() const
	{
		Size prefer(400, 300);

		if(image_.data) {
			prefer.reset(image_.cols, image_.rows);
		}

		return prefer;
	}

	bool CVImageView::OpenCamera (int n, const Size& resolution)
	{
		bool retval = false;

		if(video_stream_.isOpened()) {
			video_stream_.release();
		}

		video_stream_.open(n);
		if(video_stream_.isOpened()) {

			video_stream_.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width());
			video_stream_.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height());

			float w = const_cast<cv::VideoCapture&>(video_stream_).get(CV_CAP_PROP_FRAME_WIDTH);
			float h = const_cast<cv::VideoCapture&>(video_stream_).get(CV_CAP_PROP_FRAME_HEIGHT);

			vbo_.bind(1);
			float* ptr = (float*)vbo_.map();
			*(ptr + 4) = w;
			*(ptr + 9) = h;
			*(ptr + 12) = w;
			*(ptr + 13) = h;
			vbo_.unmap();
			vbo_.reset();

			retval = true;

		} else {
			DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
		}

		return retval;
	}

	bool CVImageView::OpenFile(const std::string& filename)
	{
		bool status_before = (image_.data != 0);

		image_ = cv::imread(filename);

		if(image_.data) {

			vbo_.bind(1);
			float* ptr = (float*)vbo_.map();
			*(ptr + 4) = image_.cols;
			*(ptr + 9) = image_.rows;
			*(ptr + 12) = image_.cols;
			*(ptr + 13) = image_.rows;
			vbo_.unmap();
			vbo_.reset();

			texture_.bind();

			switch (image_.channels()) {

				case 1: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					texture_.SetImage(0, GL_RED, image_.cols, image_.rows,
							0, GL_RED, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				case 2: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
					texture_.SetImage(0, GL_RG, image_.cols, image_.rows,
							0, GL_RG, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				case 3: {
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_.SetImage(0, GL_RGB, image_.cols, image_.rows,
							0, GL_BGR, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_.SetImage(0, GL_RGBA, image_.cols, image_.rows,
							0, GL_BGRA, GL_UNSIGNED_BYTE, image_.data);
					break;
				}

				default: {
					break;
				}
			}

		}

		bool status_after = (image_.data != 0);

		if((status_before == false) && (status_after == false)) {
			return false;
		}

		RequestRedraw();
		return image_.data ? true : false;
	}

	void CVImageView::Play ()
	{
		AbstractWindow* win = AbstractWindow::GetWindow(this);
		if(win == nullptr) return;

		if(playing_) return;

		if(video_stream_.isOpened()) {
			assert(off_screen_context_ == 0);
			off_screen_context_ = win->CreateSharedContext(win->size().width(), win->size().height(), false);

			timer_->Start();
			playing_ = true;
		}
	}

	void CVImageView::Pause ()
	{
		if(timer_->enabled()) {
			timer_->Stop();
		}
	}

	void CVImageView::Stop ()
	{
		if(!playing_) return;

		timer_->Stop();

		if(video_stream_.isOpened()) {
			video_stream_.release();
			image_.release();
		}

		if(off_screen_context_) {
			delete off_screen_context_;
			off_screen_context_ = 0;
		}

		playing_ = false;
	}

	void CVImageView::Release ()
	{
	}

	void CVImageView::ProcessImage(cv::Mat& image)
	{
		// override this
	}

	void CVImageView::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			vbo_.bind(0);
			vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			vbo_.reset();

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool CVImageView::PreDraw(AbstractWindow* context)
	{
		if(!visiable()) return false;

		Point offset = GetOffset();
		glm::mat3 matrix = glm::translate(AbstractWindow::shaders->widget_model_matrix(),
				glm::vec2(position().x() + offset.x(),
						position().y() + offset.y()));

		AbstractWindow::shaders->PushWidgetModelMatrix();
		AbstractWindow::shaders->SetWidgetModelMatrix(matrix);

		// draw background and stencil mask

		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR), 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		context->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		context->EndPushStencil();

		return true;
	}

	Response CVImageView::Draw (AbstractWindow* context)
	{
		if(mutex_.trylock()) {

			texture_.bind();

			AbstractWindow::shaders->widget_image_program()->use();

			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_POSITION),
					(size().width() - image_.cols)/2.f,
					(size().height() - image_.rows) / 2.f);
			glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

			glBindVertexArray(vao_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			mutex_.unlock();

		} else {
			DBG_PRINT_MSG("%s", "Fail to lock mutex, writing texture in thread");
		}

		return Finish;
	}

	void CVImageView::PostDraw(AbstractWindow* context)
	{
		// draw background again to unmask stencil
		AbstractWindow::shaders->widget_inner_program()->use();

		glBindVertexArray(vao_[0]);

		context->BeginPopStencil();	// pop inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		context->EndPopStencil();

		AbstractWindow::shaders->PopWidgetModelMatrix();
	}

	void CVImageView::OnUpdateFrame (Timer* sender)
	{
		if(off_screen_context_ && (video_stream_.isOpened())) {

			if(mutex_.trylock()) {

				video_stream_ >> image_;
				off_screen_context_->MakeCurrent();

				if(image_.data) {
					ProcessImage(image_);
				}

				if(image_.data) {

					texture_.bind();

					switch (image_.channels()) {

						case 1: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
							texture_.SetImage(0, GL_RED, image_.cols, image_.rows,
									0, GL_RED, GL_UNSIGNED_BYTE, image_.data);
							break;
						}

						case 2: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
							texture_.SetImage(0, GL_RG, image_.cols, image_.rows,
									0, GL_RG, GL_UNSIGNED_BYTE, image_.data);
							break;
						}

						case 3: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
							texture_.SetImage(0, GL_RGB, image_.cols, image_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, image_.data);
							break;
						}

						case 4:	{
							// opencv does not support alpha-channel, only masking, these code will never be called
							glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
							texture_.SetImage(0, GL_RGBA, image_.cols, image_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, image_.data);
							break;
						}

						default: {
							break;
						}
					}

				}

				mutex_.unlock();

				RequestRedraw();

			} else {
				DBG_PRINT_MSG("%s", "Fail to lock mutex, lost one frame");
			}

		}
	}

}

#endif	// __USE_OPENCV__
