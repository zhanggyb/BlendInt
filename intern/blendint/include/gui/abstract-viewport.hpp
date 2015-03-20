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

#include <gui/abstract-frame.hpp>

namespace BlendInt {

  /**
   * @brief Abstract class for 3D scene
   *
   * @ingroup blendint_gui_frames
   */
  class AbstractViewport: public AbstractFrame
  {
  DISALLOW_COPY_AND_ASSIGN(AbstractViewport);

  public:

    AbstractViewport (int width, int height);

    virtual ~AbstractViewport ();

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

  protected:

    virtual Response PerformKeyPress (AbstractWindow* context);

    virtual void PerformFocusOn (AbstractWindow* context);

    virtual void PerformFocusOff (AbstractWindow* context);

    virtual void PerformHoverIn (AbstractWindow* context);

    virtual void PerformHoverOut (AbstractWindow* context);

    virtual Response PerformMousePress (AbstractWindow* context);

    virtual Response PerformMouseRelease (AbstractWindow* context);

    virtual Response PerformMouseMove (AbstractWindow* context);

    virtual Response PerformMouseHover (AbstractWindow* context);

    virtual void RenderScene () = 0;

  private:

    virtual bool PreDraw (AbstractWindow* context);

    virtual Response Draw (AbstractWindow* context);

    virtual void PostDraw (AbstractWindow* context);
  };

}
