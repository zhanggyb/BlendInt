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

#include <gui/abstract-widget.hpp>

namespace BlendInt {

  /**
   * @brief A separator widget
   *
   * @ingroup blendint_gui_widgets
   */
  class Separator: public AbstractWidget
  {
  DISALLOW_COPY_AND_ASSIGN(Separator);

  public:

    Separator (bool expand_x = false, bool expand_y = false);

    virtual ~Separator ();

    void SetExpandX (bool expand);

    void SetExpandY (bool expand);

    void SetExpand (bool expand_x, bool expand_y);

    virtual Size GetPreferredSize () const;

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

  protected:

    virtual Response PerformMousePress (AbstractWindow* context);

    virtual Response PerformMouseRelease (AbstractWindow* context);

    virtual bool PreDraw (AbstractWindow* context);

    virtual Response Draw (AbstractWindow* context);

    virtual void PostDraw (AbstractWindow* context);

  private:

    bool expand_x_;
    bool expand_y_;

    Size preferred_size_;
  };

}
