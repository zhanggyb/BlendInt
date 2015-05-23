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

#include <blendint/gui/check-button.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

CheckButton::CheckButton()
    : AbstractButton()
{
  set_checkable(true);

  Font font;	// default font
  int w = 80;
  int h = font.height();

  set_size(w + pixel_size(kPadding.hsum()),
           h + pixel_size(kPadding.vsum()));
}

CheckButton::CheckButton (const String& text)
    : AbstractButton(text)
{
  set_checkable(true);

  int w = this->text()->size().width();
  int h = this->text()->font().height();
  if(w < 80) w = 80;

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);
}

CheckButton::~CheckButton ()
{
}

Size CheckButton::GetPreferredSize () const
{
  int w = 0;
  int h = 0;

  const RefPtr<CheckIcon>& icon = icons()->check();

  w = icon->size().width();
  h = icon->size().height();

  w += kIconTextSpace;

  Font font;  // default font
  if (text()) {
    font = text()->font();
    w += text()->size().width();
  }

  h = std::max(h, font.height());

  if (w == kIconTextSpace) {
    w = h;
  }

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  return Size(w, h);
}

Response CheckButton::Draw (AbstractWindow* context)
{
  Rect rect(pixel_size(kPadding.left()),
            pixel_size(kPadding.bottom()),
            size().width() - pixel_size(kPadding.hsum()),
            size().height() - pixel_size(kPadding.vsum()));

  const RefPtr<CheckIcon>& icon = icons()->check();

  if (icon->size().height() <= rect.height()) {
    if (icon->size().width() <= rect.width()) {

      int align = AlignVerticalCenter;
      if (text()) {
        align |= AlignLeft;
      } else {
        align |= AlignHorizontalCenter;
      }

      icon->DrawInRect(rect, align);
      if(is_checked()) {
        icons()->hook()->DrawInRect(rect, align,
                                             Color(Palette::White).data());
      }

      rect.cut_left(icon->size().width() + kIconTextSpace);
    }
  }

  if (text()) {
    if (text()->size().height() <= rect.height()) {
      text()->DrawInRect(rect,
                         AlignHorizontalCenter | AlignJustify | AlignBaseline);
    }
  }

  return Finish;
}

}
