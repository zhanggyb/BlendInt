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

#include <blendint/stock/shaders.hpp>

#include <blendint/gui/slider.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

Slider::Slider (Orientation orientation)
    : AbstractSlider<int>(orientation),
    vao_(0),
    last_value_(0),
    pressed_(false)
{
  slide_icon_.Resize(14, 14);

  if (orientation == Vertical) {
    set_size(18, 200);
  } else {
    set_size(200, 18);
  }

  glGenVertexArrays(1, &vao_);
  vbo_.generate();

  glBindVertexArray(vao_);

  vbo_.bind(0);

  std::vector<GLfloat> line_verts(8, 0.f);

  if(orientation == Horizontal) {

    // line_verts[0] = 0.f;
    // line_verts[1] = 0.f;

    line_verts[2] = size().width();
    // line_verts[3] = 0.f;

    //line_verts[4] = 0.f;
    line_verts[5] = pixel_size(1);

    line_verts[6] = size().width();
    line_verts[7] = pixel_size(1);

  } else {

    // line_verts[0] = 0.f;
    // line_verts[1] = 0.f;

    line_verts[2] = pixel_size(1);
    // line_verts[3] = 0.f;

    // line_verts[4] = 0.f;
    line_verts[5] = size().height();

    line_verts[6] = pixel_size(1);
    line_verts[7] = size().height();

  }

  vbo_.set_data (sizeof(GLfloat) * line_verts.size(), &line_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

Slider::~Slider ()
{
  glDeleteVertexArrays(1, &vao_);
}

bool Slider::IsExpandX () const
{
  return orientation() == Horizontal ? true : false;
}

bool Slider::IsExpandY () const
{
  return orientation() == Vertical ? true : false;
}

Size Slider::GetPreferredSize () const
{
  if (orientation() == Horizontal)
    return Size(200, 18);
  else
    return Size(18, 200);
}

void Slider::PerformOrientationUpdate (Orientation orientation)
{
  /*
    m_line->bind();
    GLfloat* buf_p = (GLfloat*) m_line->map(GL_READ_WRITE);
    if (orientation == Horizontal) {
    *(buf_p + 0) = m_slide_icon.size().width() / 2;
    *(buf_p + 1) = size().height() / 2;
    *(buf_p + 2) = size().width() - m_slide_icon.size().width() / 2;
    *(buf_p + 3) = *(buf_p + 0);
    } else {
    *(buf_p + 0) = size().width() / 2;
    *(buf_p + 1) = m_slide_icon.size().height() / 2;
    *(buf_p + 2) = *(buf_p + 0);
    *(buf_p + 3) = size().height()
    - m_slide_icon.size().height() / 2;
    }
    m_line->unmap();
    m_line->reset();

    RequestRedraw();
  */
}

void Slider::PerformMinimumUpdate (int minimum)
{
  if (value() < minimum) {
    set_value(minimum);
  }
}

void Slider::PerformMaximumUpdate (int maximum)
{
  if (value() > maximum) {
    set_value(maximum);
  }
}

void Slider::PerformValueUpdate (int value)
{
}

void Slider::PerformStepUpdate (int step)
{
}

void Slider::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if (target == this) {
    set_size(width, height);

    vbo_.bind(0);
    GLfloat* buf_p = (GLfloat*) vbo_.map(GL_READ_WRITE);
    if (orientation() == Horizontal) {
      *(buf_p + 2) = (GLfloat)size().width();
      *(buf_p + 6) = (GLfloat)size().width();
    } else {
      *(buf_p + 5) = (GLfloat)size().height();
      *(buf_p + 7) = (GLfloat)size().height();
    }
    vbo_.unmap();
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

Response Slider::Draw (AbstractWindow* context)
{
  float x = 0.f;
  float y = 0.f;

  shaders()->widget_outer_program()->use();
  glBindVertexArray(vao_);

  glUniform4fv(shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, theme()->regular().outline.data());

  if (orientation() == Horizontal) {

    // ----- draw line

    glUniform2f(shaders()->
                location(Shaders::WIDGET_OUTER_OFFSET),
                0.f,
                size().height() / 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform2f(shaders()->
                location(Shaders::WIDGET_OUTER_OFFSET),
                0.f,
                size().height() / 2 - 1.f);
    glUniform4f(shaders()->
                location(Shaders::WIDGET_OUTER_COLOR),
                1.f, 1.f, 1.f, 0.16f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    x += get_slider_position();
    y += size().height() / 2.f - slide_icon_.size().height() / 2.f;

  } else {

    // ----- draw line

    glUniform2f(shaders()->
                location(Shaders::WIDGET_OUTER_OFFSET),
                size().width() / 2,
                0.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUniform2f(shaders()->
                location(Shaders::WIDGET_OUTER_OFFSET),
                size().width() / 2 + 1.f,
                0.f);
    glUniform4f(shaders()->
                location(Shaders::WIDGET_OUTER_COLOR),
                1.f, 1.f, 1.f, 0.16f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    x += size().width() / 2.f - slide_icon_.size().width() / 2.f;
    y += get_slider_position();

  }

  slide_icon_.Draw(x, y);

  return Finish;
}

Response Slider::PerformMouseMove (AbstractWindow* context)
{
  if (pressed_) {

    int new_value = value();

    // DO not fire if cursor is out of range, otherwise too many events
    if (GetNewValue(context->GetGlobalCursorPosition(), &new_value)) {
      set_value(new_value);
      fire_slider_moved_event(value());
      RequestRedraw();
    }

    return Finish;

  } else {
    if (CursorOnSlideIcon(context->local_cursor_position())) {

      //m_slide_icon.set_highlight(true);

      RequestRedraw();

      return Finish;
    } else {

      //m_slide_icon.set_highlight(false);
      RequestRedraw();
      return Finish;
    }
  }
}

Response Slider::PerformMousePress (AbstractWindow* context)
{
  if (CursorOnSlideIcon(context->local_cursor_position())) {
    pressed_ = true;
    last_value_ = value();
    last_cursor_ = context->GetGlobalCursorPosition();
    fire_slider_pressed();

  }

  return Finish;
}

Response Slider::PerformMouseRelease (AbstractWindow* context)
{
  if (pressed_) {
    pressed_ = false;

    if (CursorOnSlideIcon(context->local_cursor_position())) {
      fire_slider_released();
    }

  }

  return Finish;
}

int Slider::GetSpace ()
{
  int space = 0;

  if (orientation() == Horizontal) {
    space = size().width() - slide_icon_.size().width();	// m_line_start.x() is the radius of m_switch
  } else {
    space = size().height() - slide_icon_.size().height();	// m_line_start.y() is the radius of m_switch
  }

  return space;
}

bool Slider::CursorOnSlideIcon (const Point& cursor)
{
  bool ret = false;

  glm::vec2 icon_center;	// slide switch center position

  int radius = 0;

  if (orientation() == Horizontal) {
    radius = slide_icon_.size().width() / 2;
    icon_center.x = radius + get_slider_position();
    icon_center.y = size().height() / 2;
  } else {
    radius = slide_icon_.size().height() / 2;
    icon_center.x = size().width() / 2;
    icon_center.y = radius + get_slider_position();
  }

  glm::vec2 cursor_pos(cursor.x(), cursor.y());
  float distance = glm::distance(icon_center, cursor_pos);

  if (orientation() == Horizontal && distance <= radius) {
    ret = true;
  } else if (orientation() == Vertical && distance <= radius) {
    ret = true;
  } else {
    ret = false;
  }

  return ret;
}

bool Slider::GetNewValue (const Point& cursor, int* vout)
{
  bool ret = false;

  int offset = 0;
  int move_space = GetSpace();

  if (move_space == 0) return false;

  if (orientation() == Horizontal) {
    offset = cursor.x() - last_cursor_.x();

  } else {
    offset = cursor.y() - last_cursor_.y();
  }

  int val = last_value_ + (offset * (maximum() - minimum())) / move_space;

  if (val > maximum()) {
    *vout = maximum();
  } else if (val < minimum()) {
    *vout = minimum();
  } else {
    *vout = val;
    ret = true;
  }

  return ret;
}

}
