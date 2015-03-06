/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#pragma once

#include <gui/block.hpp>
#include <gui/stack-layout.hpp>
#include <gui/linear-layout.hpp>
#include <gui/frame-shadow.hpp>

#include <gui/button-group.hpp>

#include <gui/abstract-dialog.hpp>
#include <gui/stack.hpp>

namespace BlendInt {

  class ColorSelector: public AbstractDialog
  {
  DISALLOW_COPY_AND_ASSIGN(ColorSelector);

  public:

    ColorSelector ();

    virtual ~ColorSelector ();

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual bool PreDraw (AbstractWindow* context);

    virtual Response Draw (AbstractWindow* context);

  private:

    void OnButtonToggled (int index, bool toggled);

    Block* CreateRGBBlock ();

    Block* CreateHSVBlock ();

    LinearLayout* CreateHexBlock ();

    Stack* CreateBlockStack ();

    GLuint vao_[2];

    GLBuffer<ARRAY_BUFFER, 2> vbo_;

    RefPtr<FrameShadow> shadow_;

    ButtonGroup radio_group_;

    Stack* stack_;

    glm::mat4 projection_matrix_;

    glm::mat3 model_matrix_;

  };
}
