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

#include <core/margin.hpp>

#include <gui/text.hpp>
#include <gui/abstract-widget.hpp>

namespace BlendInt {

  /**
   * @brief A widget used in dialog/expander/node to show title text and fire toggle/check event
   */
  class OptionLabel: public AbstractWidget
  {
  public:

    enum Options {

      Toggle = 0x1 << 0,
      Check = 0x1 << 1,

    };

    OptionLabel (const String& text, int flags);

    virtual ~OptionLabel ();

    void SetText (const String& text);

    void SetFont (const Font& font);

    virtual Size GetPreferredSize () const;

    virtual bool IsExpandX () const;

  protected:

    virtual Response Draw (AbstractWindow* context) final;

  private:

    RefPtr<Text> text_;

    bool toggle_;

    bool check_;

    static Margin kPadding;

  };

}
