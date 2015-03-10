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

#include <gui/abstract-button.hpp>
#include <gui/button-group.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  Margin AbstractButton::kPadding = Margin(2, 2, 2, 2);

  int AbstractButton::kIconTextSpace = 2;

  AbstractButton::AbstractButton ()
  : AbstractRoundWidget(), button_flags_(0), group_(0)
  {

  }

  AbstractButton::AbstractButton (const String& text)
  : AbstractRoundWidget(), button_flags_(0), group_(0)
  {
    text_.reset(new Text(text));
  }

  AbstractButton::AbstractButton (const RefPtr<AbstractIcon>& icon)
  : AbstractRoundWidget(), button_flags_(0), group_(0)
  {
    icon_ = icon;
  }

  AbstractButton::AbstractButton (const RefPtr<AbstractIcon>& icon,
                                  const String& text)
  : AbstractRoundWidget(), button_flags_(0), group_(0)
  {
    icon_ = icon;
    text_.reset(new Text(text));
  }

  AbstractButton::~AbstractButton ()
  {

  }

  Size AbstractButton::GetPreferredSize () const
  {
    int w = 0;
    int h = 0;

    if (icon_) {
      w = icon_->size().width();
      h = icon_->size().height();
    }

    w += kIconTextSpace;

    Font font;	// default font
    if (text_) {
      font = text_->font();
      w += text_->size().width();
    }

    h = std::max(h, font.height());

    if (w == kIconTextSpace) {
      w = h;
    }

    w += pixel_size(kPadding.hsum());
    h += pixel_size(kPadding.vsum());

    return Size(w, h);
  }

  void AbstractButton::PerformHoverIn (AbstractWindow* context)
  {
    if (is_pressed()) {
      set_down(true);

      if (is_checkable()) {
        set_checked(!is_checked());
      }

      RequestRedraw();
    }
  }

  void AbstractButton::PerformHoverOut (AbstractWindow* context)
  {
    if (is_pressed()) {
      set_down(false);

      if (is_checkable()) {
        set_checked(!is_checked());
      }

      RequestRedraw();
    }
  }

  Response AbstractButton::PerformMousePress (AbstractWindow* context)
  {
    if (context->GetMouseButton() == MouseButtonLeft) {
      set_pressed(true);
      set_down(true);

      if (is_checkable()) {
        set_last_checked(is_checked());
        set_checked(!is_checked());
      }

      RequestRedraw();

      pressed_.fire(this);
    }

    return Finish;
  }

  Response AbstractButton::PerformMouseRelease (AbstractWindow* context)
  {
    if (context->GetMouseButton() == MouseButtonLeft) {
      int fire_event = 0;	// 0: no event, 1: click event, 2: toggled event

      if (is_checkable()) {
        if (is_pressed()) {
          fire_event = 2;
        }
      } else {
        if (is_pressed() && is_down()) {
          fire_event = 1;
        }
      }

      RequestRedraw();

      switch (fire_event) {

        case 0:
          break;

        case 1: {
          if (group_) {
            group_->Click(this);
          } else {
            clicked_.fire(this);
          }
          break;
        }

        case 2: {
          if (is_checked() != is_last_checked()) {

            if (group_) {
              group_->Toggle(this, is_checked());
            } else {
              toggled_.fire(this, is_checked());
            }
          }
          break;
        }

        default:
          break;
      }

      set_pressed(false);
      set_down(false);

      released_.fire(this);

      return Finish;
    }

    return Ignore;
  }

  Response AbstractButton::PerformMouseMove (AbstractWindow* context)
  {
    return Finish;
  }

  void AbstractButton::DrawIconText ()
  {
    Rect rect(pixel_size(kPadding.left()), pixel_size(kPadding.bottom()),
              size().width() - pixel_size(kPadding.hsum()),
              size().height() - pixel_size(kPadding.vsum()));

    if (icon_) {
      if (icon_->size().height() <= rect.height()) {
        if (icon_->size().width() <= rect.width()) {

          int align = AlignVerticalCenter;
          if (text_) {
            align |= AlignLeft;
          } else {
            align |= AlignHorizontalCenter;
          }

          icon_->DrawInRect(rect, align);
          rect.cut_left(icon_->size().width() + kIconTextSpace);
        }
      }
    }

    if (text_) {
      if (text_->size().height() <= rect.height()) {
        text_->DrawInRect(rect,
                          AlignHorizontalCenter | AlignJustify | AlignBaseline);
      }
    }
  }

  void AbstractButton::SetDown (bool down)
  {
    if (is_checkable()) {
      if (is_checked() != down) RequestRedraw();

      set_checked(down);

      if (group_) {
        group_->Toggle(this, is_checked());
      } else {
        toggled_.fire(this, is_checked());
      }

    } else {

      if (is_down() != down) RequestRedraw();

      set_down(down);
      if (group_) {
        group_->Click(this);
      } else {
        clicked_.fire(this);
      }
    }
  }

  void AbstractButton::SetCheckable (bool checkable)
  {
    if (!checkable) {
      set_checked(false);
    }

    set_checkable(checkable);
  }

  void AbstractButton::SetChecked (bool checked)
  {
    if (is_checkable()) {

      if (is_checked() == checked) return;

      set_checked(checked);
      RequestRedraw();

      if (group_) {
        group_->Toggle(this, is_checked());
      } else {
        toggled_.fire(this, is_checked());
      }
    }
  }

} /* namespace BlendInt */
