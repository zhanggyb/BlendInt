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

#include <blendint/gui/abstract-window.hpp>
#include <blendint/gui/grid-guides.hpp>

namespace BlendInt {

GridGuides::GridGuides (int w, int h)
{
  set_size(w, h);

  // fill texture
  std::vector<unsigned int> pixels(kUnit * kUnit, 0);

  // sub-grid lines
  int subdiv_unit = kUnit / kDivision;
  for (int i = 1; i <= (kDivision / 2); i++) {
    memset(&pixels[kUnit / 2 * kUnit + i * subdiv_unit * kUnit], 0x0F0F0F0F,
           sizeof(unsigned int) * kUnit);
    memset(&pixels[kUnit / 2 * kUnit - i * subdiv_unit * kUnit], 0x0F0F0F0F,
           sizeof(unsigned int) * kUnit);
  }

  for (int j = 1; j <= (kDivision / 2); j++) {

    for (int i = 0; i <= kUnit; i++) {
      pixels[kUnit * i + kUnit / 2 + j * subdiv_unit] = 0x0F0F0F0F;
    }

    for (int i = 0; i <= kUnit; i++) {
      pixels[kUnit * i + kUnit / 2 - j * subdiv_unit] = 0x0F0F0F0F;
    }

  }

  // main grid line
  memset(&pixels[kUnit / 2 * kUnit], 0x0000002F, sizeof(unsigned int) * kUnit);
  for (int i = 0; i < kUnit; i++) {
    pixels[kUnit * i + kUnit / 2] = 0x2F000000;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  texture_.reset(new GLTexture2D);
  texture_->generate();
  texture_->bind();
  texture_->SetWrapMode(GL_REPEAT, GL_REPEAT);
  texture_->SetMinFilter(GL_LINEAR);
  texture_->SetMagFilter(GL_LINEAR);
  texture_->SetImage(0, GL_RGBA, kUnit, kUnit, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, &pixels[0]);
  texture_->reset();

  glGenVertexArrays(1, &vao_);
  vbo_.generate();
  GLfloat vertices[] = {
      0.f, 0.f,            0.f, 0.f,     // left-bottom
      (float)w, 0.f,       1.f * w / kUnit, 0.f,   // right-bottom
      0.f, (float)h,       0.f, 1.f * h / kUnit,   // left-top
      (float)w, (float)h,  1.f * w / kUnit, 1.f * h / kUnit  // right-top
  };

  glBindVertexArray(vao_);

  vbo_.bind(0);
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

GridGuides::~GridGuides ()
{
  glDeleteVertexArrays(1, &vao_);
}

void GridGuides::Draw (int x, // x coord
                                 int y, // y coord
                                 const float* color_ptr, // color
                                 short gamma,
                                 float rotate,
                                 float scale_x,
                                 float scale_y) const
{
  // draw texture
  glActiveTexture(GL_TEXTURE0);

  texture_->bind();

  AbstractWindow::shaders()->widget_image_program()->use();
  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION), x,
      y);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA),
              gamma);

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GridGuides::DrawInRect (const Rect& rect, // rectangel to draw
                                       int align, // alignment
                                       const float* color_ptr, // color
                                       short gamma, // gamma
                                       float rotate, // rotate
                                       bool scale) const
{
  int x = rect.left();
  int y = rect.bottom();

  if (align & AlignLeft) {
    x = rect.left() + size().width() / 2;
  } else if (align & AlignRight) {
    x = rect.right() - size().width() / 2;
  } else if (align & AlignHorizontalCenter) {
    x = rect.hcenter();
  }

  if (align & AlignTop) {
    y = rect.top() - size().height() / 2;
  } else if (align & AlignBottom) {
    y = rect.bottom() + size().height() / 2;
  } else if (align & AlignVerticalCenter) {
    y = rect.vcenter();
  }

  // draw texture
  glActiveTexture(GL_TEXTURE0);

  texture_->bind();

  AbstractWindow::shaders()->widget_image_program()->use();
  glUniform2f(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_POSITION), x,
      y);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);
  glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_IMAGE_GAMMA),
              gamma);

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GridGuides::PerformSizeUpdate (int w, int h)
{
  set_size(w, h);

  GLfloat vertices[] = {
      0.f, 0.f,            0.f, 0.f,     // left-bottom
      (float)w, 0.f,       1.f * w / kUnit, 0.f,   // right-bottom
      0.f, (float)h,       0.f, 1.f * h / kUnit,   // left-top
      (float)w, (float)h,  1.f * w / kUnit, 1.f * h / kUnit  // right-top
  };

  vbo_.bind(0);
  vbo_.set_data(sizeof(vertices), vertices);
  vbo_.reset();
}

} // namespace BlendInt
