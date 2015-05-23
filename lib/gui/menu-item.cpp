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

#include <blendint/gui/menu-item.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

Margin MenuItem::kPadding = Margin(2, 2, 2, 2);

int MenuItem::kSpace = 2;

MenuItem::MenuItem (const String& text)
    : AbstractMenuItem(),
      hovered_(false)
{
  action_ = Action::Create(text);

  InitializeMenuItem();
}

MenuItem::MenuItem(const RefPtr<Action>& action)
    : AbstractMenuItem(),
      hovered_(false)
{
#ifdef DEBUG
  DBG_ASSERT(action);
#endif

  action_ = action;
  InitializeMenuItem();
}

MenuItem::MenuItem (const String& text, const String& shortcut)
    : AbstractMenuItem(),
      hovered_(false)
{
  action_.reset(new Action(text, shortcut));
  InitializeMenuItem();
}

MenuItem::MenuItem (const RefPtr<AbstractIcon>& icon, const String& text)
    : AbstractMenuItem(),
      hovered_(false)
{
  action_.reset(new Action(icon, text));
  InitializeMenuItem();
}

MenuItem::MenuItem (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut)
    : AbstractMenuItem(),
      hovered_(false)
{
  action_ = Action::Create(icon, text, shortcut);
  InitializeMenuItem();
}

MenuItem::~MenuItem()
{
  glDeleteVertexArrays(1, &vao_);
}

bool MenuItem::IsExpandX () const
{
  return true;
}

Size MenuItem::GetPreferredSize () const
{
  Font font;

  int w = kSpace * 2;
  int h = font.height();

  if(action_->icon()) {
    w += std::max(action_->icon()->size().width(), h);
    h = std::max(h, action_->icon()->size().height());
  } else {
    w += h;
  }

  if(action_->text()) {
    w += action_->text()->size().width();
    h = std::max(h, action_->text()->size().height());
  }

  if(action_->shortcut()) {
    w += action_->shortcut()->size().width();
    h = std::max(h, action_->shortcut()->size().height());
  }

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  return Size(w, h);
}

void MenuItem::PerformHoverIn(AbstractWindow* context)
{
  hovered_ = true;
  RequestRedraw();
}

void MenuItem::PerformHoverOut(AbstractWindow* context)
{
  hovered_ = false;
  RequestRedraw();
}

Response MenuItem::Draw(AbstractWindow* context)
{
  if (hovered_) {

    shaders()->widget_inner_program()->use();

    glUniform1i(
        shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
    glUniform1i(
        shaders()->location(Shaders::WIDGET_INNER_SHADED),
        theme()->menu_item().shaded);
    glUniform4fv(
        shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
        theme()->menu_item().inner_sel.data());

    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  }

  Rect rect(pixel_size(kPadding.left()),
            pixel_size(kPadding.bottom()),
            size().width() - pixel_size(kPadding.hsum()),
            size().height() - pixel_size(kPadding.vsum()));

  if(action_->icon()) {
    action_->icon()->DrawInRect(rect, AlignLeft | AlignVerticalCenter);
    rect.cut_left(std::max(action_->icon()->size().width(), rect.height()));
  } else {
    rect.cut_left(rect.height());
  }

  rect.cut_left(kSpace);

  const float* color_v = 0;
  if(hovered_) {
    color_v = theme()->menu_item().text_sel.data();
  } else {
    color_v = theme()->menu_item().text.data();
  }

  if(action_->text()) {
    action_->text()->DrawInRect(rect,
                                AlignLeft | AlignJustify | AlignBaseline,
                                color_v);
  }

  if(action_->shortcut()) {
    action_->shortcut()->DrawInRect(rect,
                                    AlignRight | AlignJustify | AlignBaseline,
                                    color_v);
  }

  return Finish;
}

void MenuItem::PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    if (theme()->menu_item().shaded) {
      GenerateVertices(size(), 0.f, RoundNone, 0.f,
                       Vertical,
                       theme()->menu_item().shadetop,
                       theme()->menu_item().shadedown,
                       &inner_verts, 0);
    } else {
      GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);
    }

    vbo_.bind();
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                      &inner_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void MenuItem::InitializeMenuItem()
{
  Font font;

  int w = kSpace * 2;
  int h = font.height();

  if(action_->icon()) {
    w += std::max(action_->icon()->size().width(), h);
    h = std::max(h, action_->icon()->size().height());
  } else {
    w += h;
  }

  if(action_->text()) {
    w += action_->text()->size().width();
    h = std::max(h, action_->text()->size().height());
  }

  if(action_->shortcut()) {
    w += action_->shortcut()->size().width();
    h = std::max(h, action_->shortcut()->size().height());
  }

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);

  glGenVertexArrays(1, &vao_);

  std::vector<GLfloat> inner_verts;

  if (theme()->menu_item().shaded) {
    GenerateVertices(size(), 0.f, RoundNone, 0.f,
                     Vertical,
                     theme()->menu_item().shadetop,
                     theme()->menu_item().shadedown,
                     &inner_verts, 0);
  } else {
    GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);
  }

  glBindVertexArray(vao_);
  vbo_.generate();
  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

}
