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
#include <glm/gtx/matrix_transform_2d.hpp>

//#include <opencv2/imgproc/imgproc.hpp>

#include <BlendInt/Gui/CVVideoViewport.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	CVVideoViewport::CVVideoViewport()
	: Frame(),
	  vao_(0),
	  status_(VideoStop),
	  upload_(false)
	{
		set_size(640, 480);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		InitializeCVVideoView();

		timer_.reset(new Timer);
		timer_->SetInterval(1000 / 24);	// default 24 fps

		events()->connect(timer_->timeout(), this, &CVVideoViewport::OnUpdateFrame);

		// initialize thread mutex with default attribute.
		ThreadMutexAttrib attrib;
		attrib.initialize();

		mutex_.initialize(attrib);
	}

	CVVideoViewport::~CVVideoViewport()
	{
		glDeleteVertexArrays(1, &vao_);
		mutex_.destroy();
	}

	bool CVVideoViewport::IsExpandX() const
	{
		return true;
	}

	bool CVVideoViewport::IsExpandY() const
	{
		return true;
	}

	bool CVVideoViewport::OpenCamera(int n, const Size& resolution)
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

			frame_plane_.bind();
			float* ptr = (float*)frame_plane_.map();
			*(ptr + 4) = w;
			*(ptr + 9) = h;
			*(ptr + 12) = w;
			*(ptr + 13) = h;
			frame_plane_.unmap();
			frame_plane_.reset();

			retval = true;

			RequestRedraw();
		} else {
			DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
			status_ = VideoStop;
		}

		return retval;
	}

	void CVVideoViewport::Release()
	{
		video_stream_.release();
		frame_.release();
		status_ = VideoStop;
	}

	void CVVideoViewport::SetFPS (unsigned int fps)
	{
		timer_->SetInterval(1000 / fps);
	}

	void CVVideoViewport::Play()
	{
		if(video_stream_.isOpened()) {
			status_ = VideoPlay;
			//RequestRedraw();
			timer_->Start();
		} else {
			DBG_PRINT_MSG("%s", "video stream is not opened");
		}
	}

	void CVVideoViewport::Pause()
	{
		if(timer_->enabled()) {
			status_ = VideoPause;
			timer_->Stop();
			//RequestRedraw();
		}
	}

	void CVVideoViewport::Stop()
	{
		status_ = VideoStop;
		timer_->Stop();
		//upload_ = false;

		if(video_stream_.isOpened()) {
			video_stream_.release();
			frame_.release();

			RequestRedraw();
		}
	}

	Size CVVideoViewport::GetPreferredSize() const
	{
		Size prefer (640, 480);

		if(video_stream_.isOpened()) {

			double width = const_cast<cv::VideoCapture&>(video_stream_).get(CV_CAP_PROP_FRAME_WIDTH);
			double height = const_cast<cv::VideoCapture&>(video_stream_).get(CV_CAP_PROP_FRAME_HEIGHT);

			prefer.reset(width, height);
		}

		return prefer;
	}

	void CVVideoViewport::PerformPositionUpdate(
			const PositionUpdateRequest& request)
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

			model_matrix_ = glm::translate(glm::mat3(1.f), glm::vec2(x, y));

			set_position(*request.position());

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void CVVideoViewport::PerformSizeUpdate(const SizeUpdateRequest& request)
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
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool CVVideoViewport::PreDraw(const Context* context)
	{
		if(!visiable()) return false;

		const_cast<Context*>(context)->register_active_frame(this);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Context::shaders->SetWidgetProjectionMatrix(projection_matrix_);
		Context::shaders->SetWidgetModelMatrix(model_matrix_);

		return true;
	}

	ResponseType CVVideoViewport::Draw(const Context* context)
	{
		if(status_ == VideoStop) {
			return Finish;
		}

		glActiveTexture(GL_TEXTURE0);
		texture_.bind();

		if (mutex_.trylock()) {

			if(upload_) {

				if(frame_.data) {

					switch (frame_.channels()) {

						case 1: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
							texture_.SetImage(0, GL_RED, frame_.cols, frame_.rows,
									0, GL_RED, GL_UNSIGNED_BYTE, frame_.data);
							break;
						}

						case 2: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
							texture_.SetImage(0, GL_RG, frame_.cols, frame_.rows,
									0, GL_RG, GL_UNSIGNED_BYTE, frame_.data);
							break;
						}

						case 3: {
							glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
							texture_.SetImage(0, GL_RGB, frame_.cols, frame_.rows,
									0, GL_BGR, GL_UNSIGNED_BYTE, frame_.data);
							break;
						}

						case 4:	// opencv does not support alpha-channel, only masking, these code will never be called
						{
							glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
							texture_.SetImage(0, GL_RGBA, frame_.cols, frame_.rows,
									0, GL_BGRA, GL_UNSIGNED_BYTE, frame_.data);
							break;
						}

						default: {
							texture_.reset();
							return Finish;
							break;
						}
					}
				}

				upload_ = false;

			}

			mutex_.unlock();
		} else {
			DBG_PRINT_MSG("%s", "fail to lock: capturing video into frame");
		}

		Context::shaders->widget_image_program()->use();

		glUniform1i(Context::shaders->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
		glUniform2f(Context::shaders->location(Shaders::WIDGET_IMAGE_POSITION),
				(size().width() - frame_.cols)/2.f,
				(size().height() - frame_.rows) / 2.f);
		glUniform1i(Context::shaders->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		texture_.reset();
		GLSLProgram::reset();

		return Finish;
	}

	void CVVideoViewport::PostDraw(const Context* context)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, context->size().width(), context->size().height());
	}

	void CVVideoViewport::ProcessImage(cv::Mat& iamge)
	{
		// TODO: override this
	}

	void CVVideoViewport::InitializeCVVideoView()
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			640.f, 0.f, 	1.f, 1.f,
			0.f, 480.f,		0.f, 0.f,
			640.f, 480.f,	1.f, 0.f
		};

		frame_plane_.generate();
		frame_plane_.bind();
		frame_plane_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_IMAGE_COORD));
		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_IMAGE_UV));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_IMAGE_COORD), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_IMAGE_UV), 2, GL_FLOAT,
				GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		frame_plane_.reset();
        
		texture_.generate();
		texture_.bind();
		texture_.SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_.SetMinFilter(GL_LINEAR);
		texture_.SetMagFilter(GL_LINEAR);
        //texture_.SetImage(0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummy);
		texture_.reset();
	}

	void CVVideoViewport::OnUpdateFrame(Timer* t)
	{
		if(video_stream_.isOpened()) {

			if(mutex_.trylock()) {

				video_stream_ >> frame_;

				ProcessImage(frame_);

				upload_ = true;

				mutex_.unlock();

				RequestRedraw();
			} else {
				DBG_PRINT_MSG("%s", "Fail to lock, the frame is being read");
			}

		}

		/*
		if(!refresh()) {
			RequestRedrawInThread();
		} else {
			 DBG_PRINT_MSG("%s", "this viewport is refreshing");
		}
		*/
	}

}

#endif	// __USE_OPENCV__
