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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <algorithm>

#include <opengl/opengl.hpp>

#include <gui/texture-view.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  TextureView::TextureView ()
  : AbstractScrollable()
  {
    set_size(400, 300);
    image_size_ = size();

    InitializeImageView();
  }

  TextureView::~TextureView ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  bool TextureView::OpenFile (const char* filename)
  {
    bool retval = false;

    Image image;

    if (image.Read(filename)) {

      if (!texture_) {

        texture_.reset(new GLTexture2D);
        texture_->generate();
        texture_->bind();
        texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture_->SetMinFilter(GL_LINEAR);
        texture_->SetMagFilter(GL_LINEAR);

      } else if (!glIsTexture(texture_->id())) {

        texture_->generate();
        texture_->bind();
        texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        texture_->SetMinFilter(GL_LINEAR);
        texture_->SetMagFilter(GL_LINEAR);

      } else {

        texture_->bind();

      }

      switch (image.channels()) {

        case 1: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
          texture_->SetImage(0, GL_RED, image.width(), image.height(), 0,
                             GL_RED, GL_UNSIGNED_BYTE, image.pixels());
          break;
        }

        case 2: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
          texture_->SetImage(0, GL_RG, image.width(), image.height(), 0, GL_RG,
                             GL_UNSIGNED_BYTE, image.pixels());
          break;
        }

        case 3: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
          texture_->SetImage(0, GL_RGB, image.width(), image.height(), 0,
                             GL_RGB, GL_UNSIGNED_BYTE, image.pixels());
          break;
        }

        case 4: {
          glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
          texture_->SetImage(0, GL_RGBA, image.width(), image.height(), 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, image.pixels());
          break;
        }

        default:
          break;
      }

      texture_->reset();

      image_size_.set_width(image.width());
      image_size_.set_height(image.height());

      //AdjustImageArea(size());
      chessboard_->Resize(image_size_);

      vbo_.bind(1);
      float* ptr = (float*) vbo_.map(GL_READ_WRITE);
      *(ptr + 4) = image.width();
      *(ptr + 9) = image.height();
      *(ptr + 12) = image.width();
      *(ptr + 13) = image.height();
      vbo_.unmap();
      vbo_.reset();

      RequestRedraw();

      retval = true;
    }

    return retval;
  }

  void TextureView::LoadImage (const RefPtr<Image>& image)
  {
  }

  void TextureView::SetTexture (const RefPtr<GLTexture2D>& texture)
  {
    if (texture_ == texture) return;

    texture_ = texture;

    if (texture_->id()) {
      texture_->bind();

      image_size_ = texture_->GetSize();

      chessboard_->Resize(image_size_);

      vbo_.bind(1);
      float* ptr = (float*) vbo_.map(GL_READ_WRITE);
      *(ptr + 4) = image_size_.width();
      *(ptr + 9) = image_size_.height();
      *(ptr + 12) = image_size_.width();
      *(ptr + 13) = image_size_.height();
      vbo_.unmap();
      vbo_.reset();

      texture_->reset();
    }

    ///AdjustImageArea(size());

    RequestRedraw();
  }

  void TextureView::Clear ()
  {

  }

  bool TextureView::IsExpandX () const
  {
    return true;
  }

  bool TextureView::IsExpandY () const
  {
    return true;
  }

  Size TextureView::GetPreferredSize () const
  {
    return image_size_;
  }

  void TextureView::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
  {
    if (target == this) {

      set_size(width, height);

      std::vector<GLfloat> inner_verts;
      GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

      vbo_.bind(0);
      vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
      vbo_.reset();

      //AdjustImageArea (*request.size());

      RequestRedraw();
    }

    if (source == this) {
      report_size_update(source, target, width, height);
    }
  }

  bool TextureView::PreDraw (AbstractWindow* context)
  {
    Point offset = GetOffset();
    glm::mat3 matrix = glm::translate(
        AbstractWindow::shaders()->widget_model_matrix(),
        glm::vec2(position().x() + offset.x(), position().y() + offset.y()));

    AbstractWindow::shaders()->PushWidgetModelMatrix();
    AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

    // draw background and stencil mask

    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
        0);
    glUniform4f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
        0.208f, 0.208f, 0.208f, 1.0f);

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    context->BeginPushStencil();	// inner stencil
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    context->EndPushStencil();

    return true;
  }

  Response TextureView::Draw (AbstractWindow* context)
  {
    float x = (size().width() - image_size_.width()) / 2.f;
    float y = (size().height() - image_size_.height()) / 2.f;

    // draw checkerboard
    chessboard_->Draw(x, y);

    if (texture_ && glIsTexture(texture_->id())) {

      // draw texture
      glActiveTexture(GL_TEXTURE0);

      texture_->bind();

      AbstractWindow::shaders()->widget_image_program()->use();
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION),
          x, y);
      glUniform1i(
          AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE),
          0);
      glUniform1i(
          AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA), 0);

      glBindVertexArray(vao_[1]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      texture_->reset();

    }

    return Finish;
  }

  void TextureView::PostDraw (AbstractWindow* context)
  {
    // draw background again to unmask stencil
    AbstractWindow::shaders()->widget_inner_program()->use();

    glBindVertexArray(vao_[0]);

    context->BeginPopStencil();	// pop inner stencil
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    context->EndPopStencil();

    AbstractWindow::shaders()->PopWidgetModelMatrix();
  }

  void TextureView::InitializeImageView ()
  {
    chessboard_.reset(new ChessBoard(20));
    chessboard_->Resize(size());

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
    glVertexAttribPointer(AttributeUV, 2,
    GL_FLOAT,
                          GL_FALSE, sizeof(GLfloat) * 4,
                          BUFFER_OFFSET(2 * sizeof(GLfloat)));

    glBindVertexArray(0);
    vbo_.reset();
  }

/*
 void TextureView::AdjustImageArea(const Size& size)
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

 vbo_.bind(1);
 vbo_.set_data(sizeof(vertices), vertices);
 vbo_.reset();
 }
 */
}
