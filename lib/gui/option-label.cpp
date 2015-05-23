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
#include <blendint/gui/option-label.hpp>

namespace BlendInt {

Margin OptionLabel::kPadding(2, 2, 2, 2);

OptionLabel::OptionLabel (const String& text, bool checkable)
    : AbstractWidget(),
      flags_(OptionLabelToggleMask)
{
  if (checkable) SETBIT(flags_, OptionLabelCheckableMask);

  text_.reset(new Text(text));

  int w = text_->size().width();
  int h = text_->font().height();

  w += h; // add space for num icon

  if (checkable) w += h + kSpace;  // add space for check icon

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  set_size(w, h);
}

OptionLabel::OptionLabel (const String& text,
                          const RefPtr<AbstractIcon>& icon,
                          bool checkable)
    : AbstractWidget(),
      flags_(OptionLabelToggleMask),
      display_icon_(icon)
{
  if (checkable) SETBIT(flags_, OptionLabelCheckableMask);

  text_.reset(new Text(text));

  int w = text_->size().width();
  int h = text_->font().height();

  w += h; // add space for num icon

  if (checkable) w += h + kSpace;  // add space for check icon

  if (icon) {
    w += icon->size().width();
    h = std::max (h, icon->size().height());
  }
  
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
  int w = text_->size().width();
  int h = text_->font().height();

  w += h;

  if (flags_ & OptionLabelCheckableMask) {
    w += h + kSpace;
  }

  if (display_icon_) {
    w += display_icon_->size().width();
    h = std::max (h, display_icon_->size().height());
  }

  w += pixel_size(kPadding.hsum());
  h += pixel_size(kPadding.vsum());

  return Size(w, h);
}

bool OptionLabel::IsExpandX () const
{
  return true;
}

void OptionLabel::PerformHoverIn (AbstractWindow* context)
{
}

void OptionLabel::PerformHoverOut (AbstractWindow* context)
{
}

Response OptionLabel::PerformMousePress (AbstractWindow* context)
{
  if (flags_ & OptionLabelCheckableMask) {

    Rect rect(pixel_size(kPadding.left()) + text_->font().height(),
              pixel_size(kPadding.bottom()),
              text_->font().height(),
              size().height() - pixel_size(kPadding.vsum()));

    if (rect.contains(context->local_cursor_position())) {
      // if in check icon
      return Finish;
    }

    rect.set_x(pixel_size(kPadding.left()));
    rect.set_width(rect.width() + text_->font().height() + text_->size().width());

    if (rect.contains(context->local_cursor_position())) {
      return Finish;
    } else {
      return Ignore;
    }

  } else {

    Rect rect(pixel_size(kPadding.left()),
              pixel_size(kPadding.bottom()),
              text_->font().height() + text_->size().width(),
              size().height() - pixel_size(kPadding.vsum()));

    if (rect.contains(context->local_cursor_position())) {
      return Finish;
    } else {
      return Ignore;
    }

  }

  return Ignore;
}

Response OptionLabel::PerformMouseRelease (AbstractWindow* context)
{
  return Ignore;
}

Response OptionLabel::Draw (AbstractWindow* context)
{
  Rect rect(pixel_size(kPadding.left()),
            pixel_size(kPadding.bottom()),
            size().height() - pixel_size(kPadding.vsum()),
            size().height() - pixel_size(kPadding.vsum()));
  int x = 0;
  float rotate = 0.f;
  if (flags_ & OptionLabelToggleMask) rotate = -90.f;
  
  icons()->num()->DrawInRect(rect, AlignCenter,
                                      Color(0x0F0F0FFF).data(), 0, rotate, true);
  x += rect.width();

  if (flags_ & OptionLabelCheckableMask) {
    rect.set_x(rect.x() + x);
    icons()->check()->DrawInRect(rect, AlignCenter,
                                          Color(0x0F0F0FFF).data(), 0, 0, true);
    x += rect.width() + kSpace;
  }

  if (text_->size().height() <= rect.height()) {
    rect.set_x(x);
    rect.set_width(size().width() - pixel_size(kPadding.hsum()) - x);
    text_->DrawInRect(
        rect, AlignLeft | AlignVerticalCenter | AlignJustify | AlignBaseline,
        theme()->regular().text.data());
  }

  if (display_icon_)
    display_icon_->DrawInRect(rect, AlignRight | AlignVerticalCenter);
  
  return Finish;
}

}  // namespace BlendInt
