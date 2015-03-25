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

#include <gui/abstract-window.hpp>
#include <gui/option-label.hpp>

namespace BlendInt {

  OptionLabel::OptionLabel (const String& text, int flags)
  : toggle_(false),
    check_(false)
  {
    text_.reset(new Text(text));

    int w = text_->size().width();
    int h = text_->font().height();

    w += pixel_size(kPadding.hsum());
    h += pixel_size(kPadding.vsum());

    set_size(w, h);
  }

  OptionLabel::~OptionLabel ()
  {
  }

  void OptionLabel::SetText (const String& text)
  {
    text_->SetText(text);
    RequestRedraw();
  }

  void OptionLabel::SetFont (const Font& font)
  {
    text_->SetFont(font);
    RequestRedraw();
  }

  Size OptionLabel::GetPreferredSize () const
  {
    int h = text_->font().height();
    int w = text_->size().width();

    w += pixel_size(kPadding.hsum());
    h += pixel_size(kPadding.vsum());

    return Size(w, h);
  }

  bool OptionLabel::IsExpandX () const
  {
    return true;
  }

  Response OptionLabel::Draw (AbstractWindow* context)
  {
    Rect rect(pixel_size(kPadding.left()), pixel_size(kPadding.bottom()),
              size().width() - pixel_size(kPadding.hsum()),
              size().height() - pixel_size(kPadding.vsum()));

    if (text_->size().height() <= rect.height()) {
      text_->DrawInRect(rect, AlignCenter | AlignJustify | AlignBaseline,
                        Color(Color::White).data());
    }

    return Finish;
  }

}
