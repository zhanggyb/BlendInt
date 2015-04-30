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

#include <blendint/gui/cv-image-view.hpp>

namespace BlendInt {

CVImageView::CVImageView ()
    : AbstractScrollable(), off_screen_context_(0), flags_(0)
{
  set_size(400, 300);
  image_size_.reset(400, 300);

  timer_.reset(new Timer);

  timer_->timeout().connect(this, &CVImageView::OnUpdateFrame);

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
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(0));

  glBindVertexArray(vao_[1]);

  GLfloat vertices[] = { 0.f, 0.f, 0.f, 1.f, 400.f, 0.f, 1.f, 1.f, 0.f, 300.f,
      0.f, 0.f, 400.f, 300.f, 1.f, 0.f };

  vbo_.bind(1);
  vbo_.set_data(sizeof(vertices), vertices);

  glEnableVertexAttribArray(AttributeCoord);
  glEnableVertexAttribArray(AttributeUV);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE,
                        sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
  glVertexAttribPointer(AttributeUV, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
                        BUFFER_OFFSET(2 * sizeof(GLfloat)));

  glBindVertexArray(0);
  vbo_.reset();

  std::vector<unsigned char> buf(4 * 4 * 4, 255);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  texture_.generate();
  texture_.bind();
  texture_.SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  texture_.SetMinFilter(GL_LINEAR);
  texture_.SetMagFilter(GL_LINEAR);
  texture_.SetImage(0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buf[0]);
  texture_.reset();
}

CVImageView::~CVImageView ()
{
  if (video_stream_.isOpened()) {
    video_stream_.release();
    image_.release();
  }

  glDeleteVertexArrays(2, vao_);

  if (off_screen_context_) {
    delete off_screen_context_;
  }
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
  return image_size_;
}

bool CVImageView::OpenCamera (int n, int fps, const Size& resolution)
{
  bool retval = false;

  if (flags_ & DisplayModeMask) {
    DBG_PRINT_MSG("%s", "Playing video, call Stop() and Release() first");
    return retval;
  }

  DBG_ASSERT(!video_stream_.isOpened());

  video_stream_.open(n);
  if (video_stream_.isOpened()) {

    video_stream_.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width());
    video_stream_.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height());

    image_size_.reset((int) video_stream_.get(CV_CAP_PROP_FRAME_WIDTH),
                      (int) video_stream_.get(CV_CAP_PROP_FRAME_HEIGHT));

    vbo_.bind(1);
    float* ptr = (float*) vbo_.map(GL_READ_WRITE);
    *(ptr + 4) = image_size_.width();
    *(ptr + 9) = image_size_.height();
    *(ptr + 12) = image_size_.width();
    *(ptr + 13) = image_size_.height();
    vbo_.unmap();
    vbo_.reset();

    fps = fps <= 0 ? 15 : (fps > 60 ? 60 : fps);
    timer_->SetInterval(1000 / fps);

    SETBIT(flags_, DisplayModeMask);
    SETBIT(flags_, DeviceTypeMask);
    CLRBIT(flags_, StreamingMask);
    CLRBIT(flags_, PlaybackMask);

    if (off_screen_context_ == 0) {

      AbstractWindow* win = AbstractWindow::GetWindow(this);
      off_screen_context_ = win->CreateSharedContext(win->size().width(),
                                                     win->size().height(),
                                                     WindowInvisible);

    }

    DBG_ASSERT(off_screen_context_);

    retval = true;

  } else {
    DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
    flags_ = 0;
  }

  return retval;
}

bool CVImageView::OpenImageFile (const std::string& filename)
{
  if (flags_ & DisplayModeMask) {
    DBG_PRINT_MSG("%s", "Playing video, call Stop() and Release() first");
    return false;
  }

  image_ = cv::imread(filename);

  if (image_.data) {

    image_size_.reset(image_.cols, image_.rows);

    vbo_.bind(1);
    float* ptr = (float*) vbo_.map(GL_READ_WRITE);
    *(ptr + 4) = image_size_.width();
    *(ptr + 9) = image_size_.height();
    *(ptr + 12) = image_size_.width();
    *(ptr + 13) = image_size_.height();
    vbo_.unmap();
    vbo_.reset();

    texture_.bind();

    switch (image_.channels()) {

      case 1: {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        texture_.SetImage(0, GL_RED, image_.cols, image_.rows, 0, GL_RED,
                          GL_UNSIGNED_BYTE, image_.data);
        break;
      }

      case 2: {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        texture_.SetImage(0, GL_RG, image_.cols, image_.rows, 0, GL_RG,
                          GL_UNSIGNED_BYTE, image_.data);
        break;
      }

      case 3: {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
        texture_.SetImage(0, GL_RGB, image_.cols, image_.rows, 0, GL_BGR,
                          GL_UNSIGNED_BYTE, image_.data);
        break;
      }

      case 4: {    // opencv does not support alpha-channel,
        // only masking, these code will never be
        // called
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        texture_.SetImage(0, GL_RGBA, image_.cols, image_.rows, 0, GL_BGRA,
                          GL_UNSIGNED_BYTE, image_.data);
        break;
      }

      default: {
        break;
      }
    }

    flags_ = 0;
    DBG_ASSERT(off_screen_context_ == 0);

    RequestRedraw();
    return true;

  } else {
    return false;
  }

}

bool CVImageView::OpenVideoFile (const std::string& filename, int fps)
{
  bool retval = false;

  if (flags_ & DisplayModeMask) {
    DBG_PRINT_MSG("%s", "Playing video, call Stop() and Release() first");
    return false;
  }

  DBG_ASSERT(!video_stream_.isOpened());

  video_stream_.open(filename);
  if (video_stream_.isOpened()) {

    image_size_.reset((int) video_stream_.get(CV_CAP_PROP_FRAME_WIDTH),
                      (int) video_stream_.get(CV_CAP_PROP_FRAME_HEIGHT));

    vbo_.bind(1);
    float* ptr = (float*) vbo_.map(GL_READ_WRITE);
    *(ptr + 4) = image_size_.width();
    *(ptr + 9) = image_size_.height();
    *(ptr + 12) = image_size_.width();
    *(ptr + 13) = image_size_.height();
    vbo_.unmap();
    vbo_.reset();

    fps = fps <= 0 ? 15 : (fps > 60 ? 60 : fps);
    timer_->SetInterval(1000 / fps);

    SETBIT(flags_, DisplayModeMask);
    CLRBIT(flags_, DeviceTypeMask);
    CLRBIT(flags_, StreamingMask);
    CLRBIT(flags_, PlaybackMask);

    if (off_screen_context_ == 0) {

      AbstractWindow* win = AbstractWindow::GetWindow(this);
      off_screen_context_ = win->CreateSharedContext(win->size().width(),
                                                     win->size().height(),
                                                     WindowInvisible);

    }

    DBG_ASSERT(off_screen_context_);

    retval = true;

  } else {
    DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
  }

  return retval;
}

void CVImageView::Play ()
{
  if (!(flags_ & DisplayModeMask)) return;

  if (!(flags_ & StreamingMask)) {

    DBG_ASSERT((flags_ & PlaybackMask) == 0);
    DBG_ASSERT(video_stream_.isOpened());

    SETBIT(flags_, StreamingMask);
    SETBIT(flags_, VideoPlayMask);

    timer_->Start();

  } else {

    if ((flags_ & VideoPauseMask) || (flags_ & VideoStopMask)) {

      CLRBIT(flags_, PlaybackMask);
      SETBIT(flags_, VideoPlayMask);
      timer_->Start();

    }

  }
}

void CVImageView::Pause ()
{
  if (!(flags_ & DisplayModeMask)) return;

  if (flags_ & StreamingMask) {

    if (flags_ & VideoPlayMask) {
      CLRBIT(flags_, PlaybackMask);
      SETBIT(flags_, VideoPauseMask);
      timer_->Stop();
    }

  } else {
    DBG_ASSERT((flags_ & PlaybackMask) == 0);
  }
}

void CVImageView::Stop ()
{
  if (!(flags_ & DisplayModeMask)) return;

  if (flags_ & StreamingMask) {

    if (flags_ & VideoPlayMask) {
      CLRBIT(flags_, PlaybackMask);
      SETBIT(flags_, VideoStopMask);
      timer_->Stop();
    }

  } else {
    DBG_ASSERT((flags_ & PlaybackMask) == 0);
  }
}

void CVImageView::Release ()
{
  if (flags_ & DisplayModeMask) { // play video
    video_stream_.release();
    timer_->Stop();
    if (off_screen_context_) {
      delete off_screen_context_;
      off_screen_context_ = 0;
    }
  }

  DBG_ASSERT(off_screen_context_ == 0);

  image_.release();
  flags_ = 0;

  std::vector<unsigned char> buf(4 * 4 * 4, 255);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  texture_.SetImage(0, GL_RGBA, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, &buf[0]);
}

void CVImageView::ProcessImage (cv::Mat& image)
{
  // override this
}

void CVImageView::DrawTexture ()
{
  // TODO: use double textures
  glBindVertexArray(vao_[1]);
  if (mutex_.trylock()) {
    texture_.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    mutex_.unlock();
  } else {
    DBG_PRINT_MSG("%s", "fail to lock");
  }
}

void CVImageView::PerformSizeUpdate (const AbstractView* source,
                                     const AbstractView* target,
                                     int width,
                                     int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

bool CVImageView::PreDraw (AbstractWindow* context)
{
  Point offset = GetOffset();
  glm::mat3 matrix = glm::translate(
      AbstractWindow::shaders()->widget_model_matrix(),
      glm::vec2(position().x() + offset.x(), position().y() + offset.y()));

  AbstractWindow::shaders()->PushWidgetModelMatrix();
  AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

  // draw background and stencil mask

  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
              0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
              0);
  glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
              0.208f, 0.208f, 0.208f, 1.0f);
  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  context->BeginPushStencil();	// inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPushStencil();

  return true;
}

Response CVImageView::Draw (AbstractWindow* context)
{
  AbstractWindow::shaders()->widget_image_program()->use();

  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION),
      (size().width() - image_size_.width()) / 2.f,
      (size().height() - image_size_.height()) / 2.f);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA),
              0);

  DrawTexture();

  return Finish;
}

void CVImageView::PostDraw (AbstractWindow* context)
{
  // draw background again to unmask stencil
  AbstractWindow::shaders()->widget_inner_program()->use();

  glBindVertexArray(vao_[0]);

  context->BeginPopStencil();	// pop inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPopStencil();

  AbstractWindow::shaders()->PopWidgetModelMatrix();
}

void CVImageView::OnUpdateFrame ()
{
  video_stream_ >> image_;

  if (mutex_.trylock()) {
    
    off_screen_context_->MakeCurrent();

    if (image_.data) ProcessImage(image_);

    if (image_.data) {

      texture_.bind();

      switch (image_.channels()) {

        case 1: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          texture_.SetImage(0, GL_RED, image_.cols, image_.rows, 0, GL_RED,
                            GL_UNSIGNED_BYTE, image_.data);
          break;
        }

        case 2: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
          texture_.SetImage(0, GL_RG, image_.cols, image_.rows, 0, GL_RG,
                            GL_UNSIGNED_BYTE, image_.data);
          break;
        }

        case 3: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
          texture_.SetImage(0, GL_RGB, image_.cols, image_.rows, 0, GL_BGR,
                            GL_UNSIGNED_BYTE, image_.data);
          break;
        }

        case 4: {
          // opencv does not support alpha-channel, only masking, these code will never be called
          glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
          texture_.SetImage(0, GL_RGBA, image_.cols, image_.rows, 0, GL_BGRA,
                            GL_UNSIGNED_BYTE, image_.data);
          break;
        }

        default: {
          break;
        }

      }

    }

    RequestRedraw();
    mutex_.unlock();

  } else {
    DBG_PRINT_MSG("%s", "Fail to lock mutex, lost one frame");
  }
}

}

#endif	// __USE_OPENCV__
