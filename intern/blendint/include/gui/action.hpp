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

#pragma once

#include <core/object.hpp>

#include <gui/font.hpp>
#include <gui/abstract-icon.hpp>
#include <gui/text.hpp>

namespace BlendInt {

  /**
   * @brief Items used in Menu, List, Toolbox
   */
  class Action: public Object
  {
  public:

    static RefPtr<Action> Create ();

    static RefPtr<Action> Create (const String& text);

    static RefPtr<Action> Create (const String& text, const String& shortcut);

    static RefPtr<Action> Create (const RefPtr<AbstractIcon>& icon,
        const String& text);

    static RefPtr<Action> Create (const RefPtr<AbstractIcon>& icon,
        const String& text, const String& shortcut);

    Action ();

    Action (const String& text);

    Action (const String& text, const String& shortcut);

    Action (const RefPtr<AbstractIcon>& icon, const String& text);

    Action (const RefPtr<AbstractIcon>& icon, const String& text,
        const String& shortcut);

    virtual ~Action ();

    const RefPtr<AbstractIcon>& icon () const
    {
      return icon_;
    }

    void set_icon (const RefPtr<AbstractIcon>& icon)
    {
      icon_ = icon;
    }

    const RefPtr<Text>& text () const
    {
      return text_;
    }

    void set_text (const String& text)
    {
      if(text_) {
        text_->SetText(text);
      } else {
        text_.reset(new Text(text));
      }
    }

    const RefPtr<Text>& shortcut () const
    {
      return shortcut_;
    }

    void set_shortcut (const String& shortcut)
    {
      if(shortcut_) {
        shortcut_->SetText(shortcut);
      } else {
        shortcut_.reset(new Text(shortcut));
      }
    }

  private:

    RefPtr<AbstractIcon> icon_;

    RefPtr<Text> text_;

    RefPtr<Text> shortcut_;
  };

}
