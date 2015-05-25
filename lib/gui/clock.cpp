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

#include <cmath>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/clock.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

Clock::Clock()
    : AbstractRoundWidget(), angle_(-90), timer_(0)
{
  set_size(160, 160);

  InitializeClock();
}

Clock::~Clock ()
{
  glDeleteVertexArrays(3, vao_);
}

Size Clock::GetPreferredSize() const
{
  return Size(160, 160);
}

void Clock::Start()
{
  timer_->Start();
}

void Clock::Stop()
{
  timer_->Stop();
}

Response Clock::Draw(AbstractWindow* context)
{
  shaders()->widget_triangle_program()->use();

  glUniform2f(shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
              pixel_size(size().width() / 2.f),
              pixel_size(size().height() / 2.f));
  glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
  glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

  glVertexAttrib4f(AttributeColor, 0.35f, 0.45f, 0.75f, 1.f);

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 72 + 2);

  glVertexAttrib4fv(AttributeColor, theme()->regular().outline.data());
  glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 1);

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 72 * 2 + 2);

  glBindVertexArray(vao_[2]);
  glVertexAttrib4f(AttributeColor, 1.f, 0.f, 0.f, 1.f);
  glUniform1f(shaders()->location(Shaders::WIDGET_TRIANGLE_ROTATION), -(float)angle_);
  glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray(0);

  glUniform1f(shaders()->location(Shaders::WIDGET_TRIANGLE_ROTATION), 0.f);
  GLSLProgram::reset();

  return Finish;
}

void Clock::OnUpdateClockHands()
{
  angle_ = angle_ + 6;
  if(angle_ < 360) {
    angle_ = angle_ % 360;
  }

  RequestRedraw();
}

void Clock::PerformSizeUpdate (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height)
{
  if(target == this) {
    int radius = std::min(width, height);
    radius = radius / 2;

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;
    GenerateClockVertices(pixel_size(radius), pixel_size(1.f),
                          inner_verts, outer_verts);

    buffer_.bind(0);
    buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    buffer_.bind(1);
    buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

    set_size(width, height);

    GLfloat second_hand_vertices[] = {
      pixel_size(-5.f), pixel_size(-1.f),
      pixel_size(size().width() / 2.f), pixel_size(-1.f),
      pixel_size(-5.f), pixel_size(1.f),
      pixel_size(size().width() / 2.f), pixel_size(1.f)
    };

    buffer_.bind(2);
    buffer_.set_sub_data(0, sizeof(second_hand_vertices), second_hand_vertices);
    buffer_.reset();

    RequestRedraw();
  }

  if(source == this) {
    report_size_update(source, target, width, height);
  }
}

void Clock::GenerateClockVertices (int radius,
                                   float border,
                                   std::vector<GLfloat>& inner_vertices,
                                   std::vector<GLfloat>& outer_vertices)
{
  if(inner_vertices.size() != (72 * 2 + 2 + 2))
    inner_vertices.resize(72 * 2 + 2 + 2);

  if(outer_vertices.size() != (72 * 4 + 4))
    outer_vertices.resize(72 * 4 + 4);

  double rad = 0.0;
  float x1 = 0.f;
  float y1 = 0.f;
  float x2 = 0.f;
  float y2 = 0.f;

  // 0 1 2 3 4 5
  // x y r g b a

  // the center point
  inner_vertices[0] = 0.f;
  inner_vertices[1] = 0.f;

  int i = 1;
  int j = 0;
  for(int angle = 0; angle < 360; angle = angle + 5) {
    rad = angle * M_PI / 180.0;

    x1 = (radius - border) * cos(rad);
    y1 = (radius - border) * sin(rad);
    x2 = radius * cos(rad);
    y2 = radius * sin(rad);

    inner_vertices[i * 2 + 0] = x1;
    inner_vertices[i * 2 + 1] = y1;

    outer_vertices[j * 4 + 0] = x1;
    outer_vertices[j * 4 + 1] = y1;
    outer_vertices[j * 4 + 2] = x2;
    outer_vertices[j * 4 + 3] = y2;

    i++; j++;
  }

  rad = 360 * M_PI / 180.0;
  x1 = (radius - border) * cos(rad);
  y1 = (radius - border) * sin(rad);
  x2 = radius * cos(rad);
  y2 = radius * sin(rad);

  inner_vertices[i * 2 + 0] = x1;
  inner_vertices[i * 2 + 1] = y1;

  outer_vertices[j * 4 + 0] = x1;
  outer_vertices[j * 4 + 1] = y1;
  outer_vertices[j * 4 + 2] = x2;
  outer_vertices[j * 4 + 3] = y2;
}

void Clock::InitializeClock ()
{
  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateClockVertices(pixel_size(80),
                        pixel_size(1.f),
                        inner_verts,
                        outer_verts);

  glGenVertexArrays(3, vao_);

  glBindVertexArray(vao_[0]);
  buffer_.generate();

  buffer_.bind(0);
  buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glBindVertexArray(vao_[1]);

  buffer_.bind(1);
  buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glBindVertexArray(vao_[2]);

  GLfloat second_hand_vertices[] = {
    pixel_size(-5.f), pixel_size(-1.f),
    pixel_size(size().width() / 2.f), pixel_size(-1.f),
    pixel_size(-5.f), pixel_size(1.f),
    pixel_size(size().width() / 2.f), pixel_size(1.f)
  };

  buffer_.bind(2);
  buffer_.set_data(sizeof(second_hand_vertices), second_hand_vertices);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glBindVertexArray(0);
  buffer_.reset();

  timer_.reset(new Timer);
  timer_->SetInterval(1000 / 30);

  timer_->timeout().connect(this, &Clock::OnUpdateClockHands);
}

}
