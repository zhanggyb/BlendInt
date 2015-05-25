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

#include <blendint/gui/tool-button.hpp>
#include <blendint/gui/abstract-window.hpp>

#include <blendint/core/image.hpp>

namespace BlendInt {

  ToolButton::ToolButton ()
  : AbstractButton(),
    hover_(false)
  {
    set_round_type(RoundAll);
    set_size(48, 48);

    InitializeToolButton();
  }

  ToolButton::~ToolButton ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void ToolButton::PerformSizeUpdate (const AbstractView* source,
                                      const AbstractView* target,
                                      int width,
                                      int height)
  {
    if (target == this) {

      set_size(width, height);

      std::vector<GLfloat> inner_verts;
      std::vector<GLfloat> outer_verts;

      if (theme()->tool().shaded) {
        GenerateRoundedVertices(Vertical,
                                theme()->tool().shadedown,
                                theme()->tool().shadetop,
                                &inner_verts, &outer_verts);
      } else {
        GenerateRoundedVertices(&inner_verts, &outer_verts);
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

  void ToolButton::PerformRoundTypeUpdate (int round_type)
  {
    set_round_type(round_type);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (theme()->tool().shaded) {
      GenerateRoundedVertices(Vertical,
                              theme()->tool().shadedown,
                              theme()->tool().shadetop,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    GLArrayBuffer::reset();

    RequestRedraw();
  }

  void ToolButton::PerformRoundRadiusUpdate (float radius)
  {
    set_round_radius(radius);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (theme()->tool().shaded) {
      GenerateRoundedVertices(Vertical,
                              theme()->tool().shadedown,
                              theme()->tool().shadetop,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    GLArrayBuffer::reset();

    RequestRedraw();
  }

  void ToolButton::PerformHoverIn (AbstractWindow* context)
  {
    if (!hover_) {
      hover_ = true;
      RequestRedraw();
    }
    return AbstractButton::PerformHoverIn(context);
  }

  void ToolButton::PerformHoverOut (AbstractWindow* context)
  {
    if (hover_) {
      hover_ = false;
      RequestRedraw();
    }

    return AbstractButton::PerformHoverOut(context);
  }

  Response ToolButton::Draw (AbstractWindow* context)
  {
    shaders()->widget_inner_program()->use();

    if (is_down()) {

      glUniform1i(
          shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
      glUniform1i(
          shaders()->location(Shaders::WIDGET_INNER_SHADED),
          theme()->tool().shaded);
      glUniform4fv(
          shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
          theme()->tool().inner_sel.data());

      glBindVertexArray(vao_[0]);
      glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

      shaders()->widget_outer_program()->use();

      glUniform2f(
          shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
          0.f, 0.f);
      glUniform4fv(
          shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
          theme()->tool().outline.data());

      glBindVertexArray(vao_[1]);
      glDrawArrays(GL_TRIANGLE_STRIP, 0,
                   outline_vertex_count(round_type()) * 2 + 2);

      if (emboss()) {
        glUniform4f(
            shaders()->location(Shaders::WIDGET_OUTER_COLOR),
            1.0f, 1.0f, 1.0f, 0.16f);
        glUniform2f(
            shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
            0.f, pixel_size(-1.f));
        glDrawArrays(GL_TRIANGLE_STRIP, 0,
                     emboss_vertex_count(round_type()) * 2);
      }

    } else {

      if (hover_) {

        shaders()->widget_outer_program()->use();

        glUniform2f(
            shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
            0.f, 0.f);
        glUniform4fv(
            shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
            theme()->tool().outline.data());

        glBindVertexArray(vao_[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0,
                     outline_vertex_count(round_type()) * 2 + 2);

        if (emboss()) {
          glUniform4f(
              shaders()->location(Shaders::WIDGET_OUTER_COLOR),
              1.0f, 1.0f, 1.0f, 0.16f);
          glUniform2f(
              shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, pixel_size(-1.f));
          glDrawArrays(GL_TRIANGLE_STRIP, 0,
                       emboss_vertex_count(round_type()) * 2);
        }

      }

    }

    DrawAction();

    return Finish;
  }

  void ToolButton::SetAction (const String& text)
  {
    RefPtr<Action> action(new Action(text));

    action_ = action;
  }

  void ToolButton::SetAction (const String& text, const String& shortcut)
  {
    RefPtr<Action> action(new Action(text, shortcut));

    action_ = action;
  }

  void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon,
                              const String& text)
  {
    RefPtr<Action> action(new Action(icon, text));

    action_ = action;

    set_icon(icon);
    set_text(text);
  }

  void ToolButton::SetAction (const RefPtr<AbstractIcon>& icon,
                              const String& text,
                              const String& shortcut)
  {
    RefPtr<Action> action(new Action(icon, text, shortcut));

    action_ = action;

    set_icon(icon);
    set_text(text);
  }

  void ToolButton::SetAction (const RefPtr<Action>& item)
  {
    action_ = item;
  }

  Size ToolButton::GetPreferredSize () const
  {
    return Size(48, 48);
  }

  void ToolButton::InitializeToolButton ()
  {
    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    if (theme()->tool().shaded) {
      GenerateRoundedVertices(Vertical,
                              theme()->tool().shadedown,
                              theme()->tool().shadetop,
                              &inner_verts, &outer_verts);
    } else {
      GenerateRoundedVertices(&inner_verts, &outer_verts);
    }

    glGenVertexArrays(2, vao_);
    vbo_.generate();

    glBindVertexArray(vao_[0]);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 3,
                          GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(vao_[1]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2,
                          GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();
  }

  void ToolButton::DrawAction ()
  {
    if (!action_) return;

    Rect rect(kPadding.left(),
              kPadding.bottom(),
              size().width() - kPadding.hsum(),
              size().height() - kPadding.vsum());

    if (action_->icon()) {
      if (action_->icon()->size().height() <= rect.height()) {
        if (action_->icon()->size().width() <= rect.width()) {

          int align = AlignCenter;
//          if (action_->text()) {
//            align |= AlignLeft;
//          } else {
//            align |= AlignHorizontalCenter;
//          }

          action_->icon()->DrawInRect(rect, align);
          rect.cut_left(action_->icon()->size().width() + kIconTextSpace);
        }
      }
    }

//    if (action_->text()) {
//      if (action_->text()->size().height() <= rect.height()) {
//        action_->text()->DrawInRect(
//            rect, AlignHorizontalCenter | AlignJustify | AlignBaseline);
//      }
//    }
  }

}
