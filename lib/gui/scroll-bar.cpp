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

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/scroll-bar.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

  ScrollBar::ScrollBar (Orientation orientation)
      : AbstractSlider<int>(orientation), last_value_(0), pressed_(false)
  {
    if (orientation == Horizontal) {
      slide_.Resize(32, 14);
      set_size(200, 14);
    } else {
      slide_.Resize(14, 32);
      set_size(14, 200);
    }

    set_round_type(RoundAll);

    InitScrollBarOnce();
  }

  ScrollBar::~ScrollBar ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void ScrollBar::SetSliderPercentage (int percentage)
  {
    if (percentage < 0 || percentage > 100)
      return;

    if (orientation() == Horizontal) {

      int w = percentage * size().width() / 100;

      if (w < slide_.size().width())
        w = slide_.size().width();

      slide_.Resize(w, slide_.size().height());

      RequestRedraw();

    } else {

      int h = percentage * size().height() / 100;

      if (h < slide_.size().height())
        h = slide_.size().height();

      slide_.Resize(slide_.size().width(), h);

      RequestRedraw();

    }
  }

  int ScrollBar::GetSliderPercentage () const
  {
    int percentage = 0;

    if (orientation() == Horizontal) {
      percentage = slide_.size().width() * 100 / size().width();
    } else {
      percentage = slide_.size().height() * 100 / size().height();
    }

    return percentage;
  }

  bool ScrollBar::IsExpandX () const
  {
    return (orientation() == Horizontal) ? true : false;
  }

  bool ScrollBar::IsExpandY () const
  {
    return (orientation() == Vertical) ? true : false;
  }

  Size ScrollBar::GetPreferredSize () const
  {
    return (orientation() == Horizontal) ? Size(200, 14) : Size(14, 200);
  }

  void ScrollBar::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
  {
    if (target == this) {
      float radius = std::min(width, height)
          / 2.f;

      Orientation slot_orient;
      if (orientation() == Vertical) {
        slot_orient = Horizontal;
        slide_.Resize(radius * 2 + 0.5f, slide_.size().height());
        slide_.SetRadius(radius);
      } else {
        slot_orient = Vertical;
        slide_.Resize(slide_.size().width(), radius * 2 + 0.5f);
        slide_.SetRadius(radius);
      }

      set_size(width, height);

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      if (AbstractWindow::theme()->scroll().shaded) {

        short shadetop = AbstractWindow::theme()->scroll().shadetop;
        short shadedown = AbstractWindow::theme()->scroll().shadedown;
        if (orientation() == Vertical) {
          shadetop = AbstractWindow::theme()->scroll().shadedown;
          shadedown = AbstractWindow::theme()->scroll().shadetop;
        }

        GenerateVertices(size(), default_border_width(), round_type(), radius,
            slot_orient, shadetop, shadedown, &inner_verts, &outer_verts);
      } else {
        GenerateVertices(size(), default_border_width(), round_type(), radius,
            &inner_verts, &outer_verts);
      }

      vbo_.bind(0);
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
          &inner_verts[0]);

      vbo_.bind(1);
      vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
          &outer_verts[0]);

      vbo_.reset();
      RequestRedraw();
    }

    if (source == this) {
      report_size_update(source, target, width, height);
    }
  }

  void ScrollBar::PerformOrientationUpdate (Orientation orientation)
  {
  }

  void ScrollBar::PerformMinimumUpdate (int minimum)
  {
    if (value() < minimum) {
      set_value(minimum);
    }
  }

  void ScrollBar::PerformMaximumUpdate (int maximum)
  {
    if (value() > maximum) {
      set_value(maximum);
    }
  }

  void ScrollBar::PerformValueUpdate (int value)
  {
  }

  void ScrollBar::PerformStepUpdate (int step)
  {
  }

  Response ScrollBar::Draw (AbstractWindow* context)
  {
    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
        0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
        context->theme()->scroll().shaded);

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
        1, AbstractWindow::theme()->scroll().inner.data());

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET), 0.f,
        0.f);
    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
        1, AbstractWindow::theme()->scroll().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
        outline_vertex_count(round_type()) * 2 + 2);

    if (emboss()) {
      glUniform4f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.f,
          1.f, 1.f, 0.16f);
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
          0.f, 0.f - 1.f);
      glDrawArrays(GL_TRIANGLE_STRIP, 0,
          emboss_vertex_count(round_type()) * 2);
    }

    float x = 0.f, y = 0.f;

    if (orientation() == Horizontal) {
      x = GetSlidePosition();
    } else {
      y = GetSlidePosition();
    }

    slide_.Draw(x, y);
    return Finish;
  }

  void ScrollBar::PerformHoverOut (AbstractWindow* context)
  {
    /*
     if(m_slide.highlight()) {
     m_slide.set_highlight(false);
     RequestRedraw();
     }
     */
  }

  Response ScrollBar::PerformMousePress (AbstractWindow* context)
  {
    if (CursorOnSlideIcon(context->local_cursor_position())) {

      last_cursor_position_ = context->GetGlobalCursorPosition();
      last_value_ = value();
      pressed_ = true;
      fire_slider_pressed();

    }

    return Finish;
  }

  Response ScrollBar::PerformMouseMove (AbstractWindow* context)
  {
    if (pressed_) {

      int new_value = value();

      // DO not fire if cursor is out of range, otherwise too many events
      if (GetNewValue(context->GetGlobalCursorPosition(), &new_value)) {
        set_value(new_value);
        RequestRedraw();
        fire_slider_moved_event(value());
      }

    }

    return Finish;
  }

  Response ScrollBar::PerformMouseRelease (AbstractWindow* context)
  {
    if (pressed_) {

      pressed_ = false;

      RequestRedraw();

      if (CursorOnSlideIcon(context->local_cursor_position())) {
        fire_slider_released();
      }

    }

    return Finish;
  }

  void ScrollBar::InitScrollBarOnce ()
  {
    glGenVertexArrays(2, vao_);

    Size slot_size = slide_.size();
    float slot_radius;
    Orientation slot_orient;
    if (orientation() == Horizontal) {
      slot_size.set_width(size().width());
      slot_radius = slide_.size().height() / 2.f;
      slot_orient = Vertical;
    } else {
      slot_size.set_height(size().height());
      slot_radius = slide_.size().width() / 2.f;
      slot_orient = Horizontal;
    }

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->scroll().shaded) {

      short shadetop = AbstractWindow::theme()->scroll().shadetop;
      short shadedown = AbstractWindow::theme()->scroll().shadedown;
      if (orientation() == Vertical) {
        shadetop = AbstractWindow::theme()->scroll().shadedown;
        shadedown = AbstractWindow::theme()->scroll().shadetop;
      }

      GenerateVertices(slot_size, default_border_width(), round_type(),
          slot_radius, slot_orient, shadetop, shadedown, &inner_verts,
          &outer_verts);
    } else {
      GenerateVertices(slot_size, default_border_width(), round_type(),
          slot_radius, &inner_verts, &outer_verts);
    }

    vbo_.generate();

    glBindVertexArray(vao_[0]);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0));

    glBindVertexArray(vao_[1]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();
  }

  int ScrollBar::GetSpace ()
  {
    int space = 0;

    if (orientation() == Horizontal) {
      space = size().width() - slide_.size().width();
    } else {
      space = size().height() - slide_.size().height();
    }

    return space;
  }

  int ScrollBar::GetSlidePosition ()
  {
    int pos = 0;

    if (orientation() == Horizontal) {
      pos = (value() - minimum()) * (size().width() - slide_.size().width())
          / (maximum() - minimum());
    } else {
      pos = (value() - minimum()) * (size().height() - slide_.size().height())
          / (maximum() - minimum());
      pos = size().height() - slide_.size().height() - pos;
    }

    return pos;
  }

  bool ScrollBar::CursorOnSlideIcon (const Point& local_cursor)
  {
    int slide_pos = GetSlidePosition();

    int xmin, ymin, xmax, ymax;

    if (orientation() == Horizontal) {
      xmin = slide_pos;
      ymin = (size().height() - slide_.size().height()) / 2;
      xmax = xmin + slide_.size().width();
      ymax = ymin + slide_.size().height();
    } else {
      xmin = (size().width() - slide_.size().width()) / 2;
      ymin = slide_pos;
      xmax = xmin + slide_.size().width();
      ymax = ymin + slide_.size().height();
    }

    if (local_cursor.x() < xmin || local_cursor.y() < ymin
        || local_cursor.x() > xmax || local_cursor.y() > ymax) {
      return false;
    }

    glm::vec2 center;
    glm::vec2 cursor_pos(local_cursor.x(), local_cursor.y());
    float distance = 0.f;

    if (orientation() == Horizontal) {

      if (local_cursor.x() < (xmin + slide_.radius())) {

        center.x = xmin + slide_.radius();
        center.y = (ymax - ymin) / 2 + ymin;

        distance = glm::distance(center, cursor_pos);

        return distance <= slide_.radius() ? true : false;

      } else if (local_cursor.x() > (xmax - slide_.radius())) {

        center.x = xmax - slide_.radius();
        center.y = (ymax - ymin) / 2 + ymin;

        distance = glm::distance(center, cursor_pos);

        return distance <= slide_.radius() ? true : false;

      } else {
        return true;
      }

    } else {

      if (local_cursor.y() < (ymin + slide_.radius())) {

        center.x = (xmax - xmin) / 2 + xmin;
        center.y = ymin + slide_.radius();

        distance = glm::distance(center, cursor_pos);

        return distance <= slide_.radius() ? true : false;

      } else if (local_cursor.y() > (ymax - slide_.radius())) {

        center.x = (xmax - xmin) / 2 + xmin;
        center.y = ymax - slide_.radius();

        distance = glm::distance(center, cursor_pos);

        return distance <= slide_.radius() ? true : false;

      } else {
        return true;
      }

    }

    return false;
  }

  bool ScrollBar::GetNewValue (const Point& cursor, int* vout)
  {
    bool result = false;

    int offset = 0;
    int move_space = GetSpace();

    if (move_space == 0)
      return false;

    if (orientation() == Horizontal) {
      offset = cursor.x() - last_cursor_position_.x();
    } else {
      offset = last_cursor_position_.y() - cursor.y();
    }

    int val = last_value_ + (offset * (maximum() - minimum())) / move_space;

    if (val > maximum()) {
      *vout = maximum();
    } else if (val < minimum()) {
      *vout = minimum();
    } else {
      *vout = val;
      result = true;
    }

    return result;
  }

}
