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

#include <opengl/opengl.hpp>

#include <gui/numerical-slider.hpp>
#include <gui/abstract-frame.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  Margin NumericalSlider::kPadding(2, 2, 2, 2);

  NumericalSlider::NumericalSlider (Orientation orientation)
  : AbstractSlider<double>(orientation),
    text_start_(0),
    cursor_index_(0),
    last_value_(0.0),
    hover_(false),
    pressed_(false),
    moved_(false),
    edit_mode_(false)
  {
    set_round_type(RoundAll);

    int h = font_.height();
    int w = h;
    w += h;

    set_size(w + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));
    set_round_radius(size().height() / 2);

    char buf[32];
    snprintf(buf, 32, "%.3f", value());
    value_text_.reset(new Text(buf));
    value_text_->SetFont(font_);

    InitializeNumericalSlider();
  }

  NumericalSlider::NumericalSlider (const String& title,
      Orientation orientation)
  : AbstractSlider<double>(orientation),
    text_start_(0),
    cursor_index_(0),
    last_value_(0.0),
    hover_(false),
    pressed_(false),
    moved_(false),
    edit_mode_(false)
  {
    set_round_type(RoundAll);

    title_text_.reset(new Text(title));
    title_text_->SetFont(font_);

    int h = font_.height();
    int w = std::max(title_text_->size().width(), h);
    w += h;

    set_size(w + pixel_size(kPadding.hsum()), h + pixel_size(kPadding.vsum()));
    set_round_radius(size().height() / 2);

    char buf[32];
    snprintf(buf, 32, "%.3f", value());
    value_text_.reset(new Text(buf));
    value_text_->SetFont(font_);

    InitializeNumericalSlider();
  }

  NumericalSlider::~NumericalSlider ()
  {
    vao_.clear();	// no need to call

    if(edit_mode_) {
      if(hover_) {
        AbstractWindow* win = AbstractWindow::GetWindow(this);
        if(win) {
          win->PopCursor();
        }
      }
    }
  }

  void NumericalSlider::SetTitle (const String& title)
  {
    if(title_text_) {
      title_text_->SetText(title);
    } else {
      title_text_.reset(new Text(title));
    }

    RequestRedraw();
  }

  bool NumericalSlider::IsExpandX () const
  {
    return true;
  }

  Size NumericalSlider::GetPreferredSize () const
  {
    int radius_plus = 0;

    if ((round_type() & RoundTopLeft) || (round_type() & RoundBottomLeft)) {
      radius_plus += round_radius();
    }

    if ((round_type() & RoundTopRight) || (round_type() & RoundBottomRight)) {
      radius_plus += round_radius();
    }

    int w = font_.height() + pixel_size(kPadding.hsum() + radius_plus);
    int h = font_.height() + pixel_size(kPadding.vsum());

    if(title_text_) {
      w += title_text_->size().width();
    }

    if(value_text_) {
      w += value_text_->size().width();
    }

    return Size(w, h);
  }

  void NumericalSlider::PerformOrientationUpdate (Orientation orientation)
  {
  }

  void NumericalSlider::PerformMinimumUpdate (double minimum)
  {
  }

  void NumericalSlider::PerformMaximumUpdate (double maximum)
  {
  }

  void NumericalSlider::PerformValueUpdate (double value)
  {
    char buf[32];
    snprintf(buf, 32, "%.3f", value);

    value_text_->SetText(buf);
    RequestRedraw();
  }

  void NumericalSlider::PerformStepUpdate (double step)
  {
  }

  void NumericalSlider::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      set_size(*request.size());

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      if (AbstractWindow::theme()->number_slider().shaded) {
        GenerateRoundedVertices(Vertical,
            AbstractWindow::theme()->number_slider().shadetop,
            AbstractWindow::theme()->number_slider().shadedown, &inner_verts,
            &outer_verts);
      } else {
        GenerateRoundedVertices(&inner_verts, &outer_verts);
      }

      vbo_.bind(0);
      vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
          &inner_verts[0]);
      vbo_.bind(1);
      vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
          &outer_verts[0]);

      vbo_.bind(2);
      GLfloat* buf_p = (GLfloat*) vbo_.map(GL_READ_WRITE);
      *(buf_p + 5) = (GLfloat) (request.size()->height()
          - vertical_space * 2 * AbstractWindow::theme()->pixel());
      *(buf_p + 7) = (GLfloat) (request.size()->height()
          - vertical_space * 2 * AbstractWindow::theme()->pixel());
      vbo_.unmap();
      vbo_.reset();

      RequestRedraw();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void NumericalSlider::PerformRoundTypeUpdate (int round_type)
  {
    set_round_type(round_type);
    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->number_slider().shaded) {
      GenerateRoundedVertices(Vertical,
          AbstractWindow::theme()->number_slider().shadetop,
          AbstractWindow::theme()->number_slider().shadedown, &inner_verts,
          &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    GLArrayBuffer::reset();

    RequestRedraw();
  }

  void NumericalSlider::PerformRoundRadiusUpdate (float radius)
  {
    set_round_radius(radius);
    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->number_slider().shaded) {
      GenerateRoundedVertices(Vertical,
          AbstractWindow::theme()->number_slider().shadetop,
          AbstractWindow::theme()->number_slider().shadedown, &inner_verts,
          &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    GLArrayBuffer::reset();

    RequestRedraw();
  }

  Response NumericalSlider::Draw (AbstractWindow* context)
  {
    if(edit_mode_) {
      DrawEditMode(context);
    } else {
      DrawSlideMode(context);
    }

    return Finish;
  }

  void NumericalSlider::PerformFocusOn (AbstractWindow* context)
  {
  }

  void NumericalSlider::PerformFocusOff (AbstractWindow* context)
  {
    if(edit_mode_) {
      edit_mode_ = false;
      RequestRedraw();
    }
  }

  void NumericalSlider::PerformHoverIn (AbstractWindow* context)
  {
    if(edit_mode_) {
      context->PushCursor();
      context->SetCursor(IBeamCursor);
    }

    hover_ = true;
  }

  void NumericalSlider::PerformHoverOut (AbstractWindow* context)
  {
    if(edit_mode_) {
      context->PopCursor();
    }

    hover_ = false;
  }

  Response NumericalSlider::PerformKeyPress (AbstractWindow* context)
  {
    if(edit_mode_) {

      int key = context->GetKeyInput();

      if((key == Key_Enter) || (key == Key_KP_Enter)) {

        std::string str = ConvertFromString(value_text_->text());
        double v;
        sscanf(str.c_str(), "%lf", &v);

        v = glm::clamp(v, minimum(), maximum());

        edit_mode_ = false;
        if(hover_) context->PopCursor();

        char buf[32];
        snprintf(buf, 32, "%.3f", v);
        value_text_->SetText(buf);

        RequestRedraw();

        if(v != value()) {
          set_value(v);
          fire_value_changed_event(value());
        }

      } else if(key == Key_Escape) {

        edit_mode_ = false;
        if(hover_) context->PopCursor();

        RequestRedraw();
      } else {
        return KeyPressInEditMode(context);
      }

    }
    return Finish;
  }

  Response NumericalSlider::PerformMousePress (AbstractWindow* context)
  {
    last_value_ = value();
    last_cursor_position_ = context->GetGlobalCursorPosition();
    pressed_ = true;
    moved_ = false;

    if(!edit_mode_) {
      fire_slider_pressed();
    }

    return Finish;
  }

  Response NumericalSlider::PerformMouseMove (AbstractWindow* context)
  {
    if(pressed_) {

      moved_ = true;

      double new_value = value();
      if(GetNewValue(context->GetGlobalCursorPosition(), &new_value)) {
        set_value(new_value);

        char buf[32];
        snprintf(buf, 32, "%.3f", new_value);

        value_text_->SetText(buf);

        RequestRedraw();
        fire_slider_moved_event(value());
      }

    }

    return Finish;
  }

  Response NumericalSlider::PerformMouseRelease (AbstractWindow* context)
  {
    if(pressed_) {
      pressed_ = false;

      if(moved_) {
        moved_ = false;
        fire_slider_released();
      } else {
        edit_mode_ = true;
        cursor_index_ = value_text_->length();
        RequestRedraw();
      }

    }

    return Finish;
  }

  void NumericalSlider::InitializeNumericalSlider ()
  {
    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (AbstractWindow::theme()->number_slider().shaded) {
      GenerateRoundedVertices(Vertical,
          AbstractWindow::theme()->number_slider().shadetop,
          AbstractWindow::theme()->number_slider().shadedown, &inner_verts,
          &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vao_.generate();
    vbo_.generate();

    // generate buffer for inner
    vao_.bind(0);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3,
    GL_FLOAT, GL_FALSE, 0, 0);

    // generate buffer for outer
    vao_.bind(1);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2,
    GL_FLOAT, GL_FALSE, 0, 0);

    // generate cursor vertices
    std::vector<GLfloat> cursor_vertices(8, 0.f);

    cursor_vertices[0] = 0.f;
    cursor_vertices[1] = (GLfloat) vertical_space
        * AbstractWindow::theme()->pixel();

    cursor_vertices[2] = 1.f;
    cursor_vertices[3] = (GLfloat) vertical_space
        * AbstractWindow::theme()->pixel();

    cursor_vertices[4] = 0.f;
    cursor_vertices[5] = (GLfloat) (size().height()
        - vertical_space * 2 * AbstractWindow::theme()->pixel());

    cursor_vertices[6] = 1.f;
    cursor_vertices[7] = (GLfloat) (size().height()
        - vertical_space * 2 * AbstractWindow::theme()->pixel());

    vao_.bind(2);
    vbo_.bind(2);
    vbo_.set_data(sizeof(GLfloat) * cursor_vertices.size(),
        &cursor_vertices[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    vao_.reset();
    vbo_.reset();
  }

  Response NumericalSlider::KeyPressInEditMode (AbstractWindow* context)
  {
    if (!context->GetTextInput().empty()) {

      int valid_width = size().width() - 2 * round_radius();

      if (value_text_->empty()) {

        value_text_->Add(context->GetTextInput());
        cursor_index_ += context->GetTextInput().length();

      } else {

        int prev_width = value_text_->size().width();
        value_text_->Insert(cursor_index_, context->GetTextInput());
        int new_width = value_text_->size().width();

        cursor_index_ += context->GetTextInput().length();

        if (new_width <= valid_width) {

          text_start_ = 0;

        } else {

          int diff = new_width - prev_width;
          DBG_ASSERT(diff > 0);

          size_t visible_width = value_text_->GetTextWidth(
              cursor_index_ - text_start_ + 1, text_start_, false);

          if (visible_width > (size_t) valid_width) {

            size_t num = 0;
            size_t char_width = 0;
            while (char_width < (size_t) diff) {
              num++;
              char_width = value_text_->GetTextWidth(num, text_start_, true);
            }
            text_start_ += num;
          }

        }

      }

      RequestRedraw();

      return Finish;

    } else {

      if (!value_text_)
        return Finish;

      switch (context->GetKeyInput()) {

        case Key_Backspace: {
          DisposeBackspacePress();
          break;
        }

        case Key_Delete: {
          DisposeDeletePress();
          break;
        }

        case Key_Left: {
          DisposeLeftPress();
          break;
        }

        case Key_Right: {
          DisposeRightPress();
          break;
        }

        case Key_Up: {
          break;
        }

        case Key_Down: {
          break;
        }

        default:
          break;
      }

      return Finish;
    }
  }

  void NumericalSlider::DisposeBackspacePress ()
  {
    if (value_text_->length() && cursor_index_ > 0) {

      int prev_width = value_text_->size().width();
      value_text_->Erase(cursor_index_ - 1, 1);
      int new_width = value_text_->size().width();

      cursor_index_--;

      if (text_start_ > 0) {

        int diff = prev_width - new_width;

        size_t num = 0;
        size_t char_width = 0;
        while ((char_width < (size_t) diff) && ((text_start_ - num) > 0)) {
          num++;
          char_width = value_text_->GetTextWidth(num, text_start_ - num, true);
        }
        text_start_ -= num;

      }

      RequestRedraw();
    }
  }

  void NumericalSlider::DisposeDeletePress ()
  {
    if (value_text_->length() && (cursor_index_ < value_text_->length())) {

      value_text_->Erase(cursor_index_, 1);

      RequestRedraw();
    }
  }

  void NumericalSlider::DisposeLeftPress ()
  {
    if (value_text_->length() && cursor_index_ > 0) {

      if (cursor_index_ == text_start_) {
        text_start_--;
      }

      cursor_index_--;
      RequestRedraw();
    }
  }

  void NumericalSlider::DisposeRightPress ()
  {
    if ((value_text_->length() > 0) && (cursor_index_ < value_text_->length())) {

      int valid_width = size().width() - pixel_size(kPadding.hsum());
      size_t visible_width = value_text_->GetTextWidth(
          cursor_index_ - text_start_ + 1, text_start_, false);

      cursor_index_++;
      if (visible_width > (size_t) valid_width) {
        text_start_ += 1;
      }

      RequestRedraw();
    }
  }

  float NumericalSlider::GetSlidePosition (float border, double v)
  {
    float minxi = 0.f;
    float maxxi = size().width();
    //float radi = (round_radius() - border) * AbstractWindow::theme()->pixel();

    return (maxxi - minxi) * v / (maximum() - minimum());
  }

  bool NumericalSlider::GetNewValue(const Point& cursor, double* out_v)
  {
    bool result = false;

    float space = 0.f;
    int offset = 0;

    if(orientation() == Horizontal) {
      space = size().width();
      offset = cursor.x() - last_cursor_position_.x();
    } else {
      space = size().height();
      offset = last_cursor_position_.y() - cursor.y();
    }

    double value = last_value_ + (offset * (maximum() - minimum())) / space;

    if (value > maximum()) {
      *out_v = maximum();
    } else if (value < minimum()) {
      *out_v = minimum();
    } else {
      *out_v = value;
      result = true;
    }

    return result;
  }

  void NumericalSlider::DrawSlideMode (AbstractWindow* context)
  {
    float x = (context->shaders()->widget_model_matrix()
        * glm::vec3(0.f, 0.f, 1.f)).x;

    if (context->active_frame()->has_view_buffer()) {
      x = x - context->viewport_origin().x();
    } else {
      x = context->active_frame()->position().x() + x
          - context->viewport_origin().x();
    }

    int outline_vertices = GetOutlineVertices(round_type());
    float len = GetSlidePosition(default_border_width(), value());

    AbstractWindow::shaders()->widget_split_inner_program()->use();

    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_SPLIT_INNER_GAMMA),
        0);

    glUniform1f(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SPLIT_INNER_PARTING),
        x + len);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_SPLIT_INNER_COLOR0),
        1, AbstractWindow::theme()->number_slider().inner_sel.data());
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_SPLIT_INNER_COLOR1),
        1, AbstractWindow::theme()->number_slider().inner.data());

    vao_.bind(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertices + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
        0.f, 0.f);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->number_slider().outline.data());

    vao_.bind(1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertices * 2 + 2);

    if (emboss()) {
      glUniform4f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
          1.0f, 1.0f, 1.0f, 0.16f);

      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
          0.f, -1.f);

      glDrawArrays(GL_TRIANGLE_STRIP, 0,
                   GetHalfOutlineVertices(round_type()) * 2);
    }

    Rect rect(0, 0, AbstractWindow::icons()->num()->size().width() * 2,
              size().height());

    AbstractWindow::icons()->num()->DrawInRect(rect, AlignCenter,
                                               Color(0x0F0F0FFF).data(), 0,
                                               180.f);

    rect.set_x(rect.width() + pixel_size(kPadding.left()));
    rect.set_width(
        size().width() - pixel_size(kPadding.hsum())
            - 4 * AbstractWindow::icons()->num()->size().width());

    if (title_text_) {
      title_text_->DrawInRect(
          rect, AlignLeft | AlignJustify | AlignVerticalCenter | AlignBaseline);
    }
    value_text_->DrawInRect(
        rect, AlignRight | AlignVerticalCenter | AlignJustify | AlignBaseline);

    rect.set_x(
        size().width() - AbstractWindow::icons()->num()->size().width() * 2);
    rect.set_width(AbstractWindow::icons()->num()->size().width());

    AbstractWindow::icons()->num()->DrawInRect(rect, AlignCenter,
                                               Color(0x0F0F0FFF).data(), 0,
                                               0.f);
  }

  void NumericalSlider::DrawEditMode (AbstractWindow* context)
  {
    AbstractWindow::shaders()->widget_inner_program()->use();

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
        1, AbstractWindow::theme()->text().inner.data());
    glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA),
        0);

    vao_.bind(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
        1, AbstractWindow::theme()->text().outline.data());
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION), 0.f,
        0.f);

    vao_.bind(1);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
        GetOutlineVertices(round_type()) * 2 + 2);

    if (emboss()) {
      glUniform4f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1.0f,
          1.0f, 1.0f, 0.16f);
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION),
          0.f, -1.f);
      glDrawArrays(GL_TRIANGLE_STRIP, 0,
          GetHalfOutlineVertices(round_type()) * 2);
    }

    int cursor_pos = 0;

    int x = round_radius();
    int y = (size().height() - font_.height()) / 2 - font_.descender();

    if (value_text_) {

      int w = size().width() - round_radius() * 2;
      int h = size().height() - pixel_size(kPadding.vsum());

      // A workaround for Adobe Source Han Sans
      int diff = font_.ascender() - font_.descender();
      if (diff < font_.height()) {
        y += (font_.height() - diff - 1) / 2;
      }

//            if((alignment_ == AlignVerticalCenter) || (alignment_ == AlignCenter)) {
//                x += (w - text_ext_->size().width()) / 2;
//            } else if (alignment_ == AlignRight) {
//                x = w - text_ext_->size().width();
//            }

      if (value_text_->size().height() <= h) {
        cursor_pos = value_text_->DrawWithCursor(x, y, cursor_index_,
            text_start_, w, AbstractWindow::theme()->text().text);
      }

    }

    x += cursor_pos;
    y = 0 + 1;

    AbstractWindow::shaders()->widget_triangle_program()->use();
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION), x,
        y);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS),
        0);
    glVertexAttrib4f(AttributeColor, 0.f, 0.f, 0.f, 1.f);
    // glVertexAttrib4f(AttributeColor, 0.f, 0.215f, 1.f, 0.75f);

    vao_.bind(2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

}
