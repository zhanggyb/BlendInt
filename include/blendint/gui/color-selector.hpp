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

#include <blendint/gui/radio-button.hpp>
#include <blendint/gui/block.hpp>
#include <blendint/gui/linear-layout.hpp>
#include <blendint/gui/frame-shadow.hpp>
#include <blendint/gui/stack.hpp>
#include <blendint/gui/button-group.hpp>
#include <blendint/gui/abstract-dialog.hpp>

namespace BlendInt {

/// @cond redundant
/**
 * @brief A special radio button expands along x
 */
class ColorModeButton: public RadioButton
{
public:

  using RadioButton::RadioButton;

  virtual ~ColorModeButton ()
  {
  }

  virtual bool IsExpandX () const override
  {
    return true;
  }

};
/// @endcond

/**
 * @brief A dialog to pick color
 *
 * @ingroup blendint_gui_frames
 */
class ColorSelector: public AbstractDialog
{
DISALLOW_COPY_AND_ASSIGN(ColorSelector);

public:

  ColorSelector ();

  virtual ~ColorSelector ();

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

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
