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
#include <blendint/gui/label.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

Margin Label::kPadding(2, 2, 2, 2);

Label::Label (const String& text, Alignment alignment)
    : AbstractWidget(), alignment_(alignment), vao_(0)
{
  text_.reset(new Text(text));

  int w = text_->size().width();
  int h = text_->font().height();

  w += kPadding.hsum();
  h += kPadding.vsum();

  set_size(w, h);

  foreground_ = Palette::Black;
  background_ = 0xFFFFFF00;

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  vbo_.generate();
  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3,
                        GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

Label::~Label ()
{
  glDeleteVertexArrays(1, &vao_);
}

void Label::SetText (const String& text)
{
  text_->SetText(text);
  RequestRedraw();
}

void Label::SetFont (const Font& font)
{
  text_->SetFont(font);
  RequestRedraw();
}

void Label::SetForeground (const Color& color)
{
  if (foreground_ != color) {
    foreground_ = color;
    RequestRedraw();
  }
}

void Label::SetBackground (const Color& color)
{
  if (background_ != color) {
    background_ = color;
    RequestRedraw();
  }
}

void Label::PerformSizeUpdate (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height)
{
  if (target == this) {
    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

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

Response Label::PerformMousePress (AbstractWindow* context)
{
  return Ignore;
}

Response Label::PerformMouseRelease (AbstractWindow* context)
{
  return Ignore;
}

Response Label::PerformMouseMove (AbstractWindow* context)
{
  return Ignore;
}

Response Label::Draw (AbstractWindow* context)
{
  shaders()->widget_inner_program()->use();

  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_SHADED), 0);
  glUniform4fv(shaders()->location(Shaders::WIDGET_INNER_COLOR), 1,
               Color(background_).data());

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  if (text_) {

    Rect rect(pixel_size(kPadding.left()), pixel_size(kPadding.bottom()),
              size().width() - pixel_size(kPadding.hsum()),
              size().height() - pixel_size(kPadding.vsum()));

    if (text_->size().height() <= rect.height()) {
      text_->DrawInRect(rect, alignment_ | AlignJustify | AlignBaseline,
                        Color(foreground_).data());
    }

  }

  return Finish;
}

Size Label::GetPreferredSize () const
{
  int h = text_->font().height();
  int w = text_->size().width();

  w += kPadding.hsum();
  h += kPadding.vsum();

  return Size(w, h);
}

bool Label::IsExpandX () const
{
  return true;
}

} /* namespace BlendInt */

