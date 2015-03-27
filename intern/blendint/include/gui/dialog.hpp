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

#include <core/string.hpp>

#include <opengl/gl-buffer.hpp>

#include <gui/abstract-dialog.hpp>
#include <gui/frame-shadow.hpp>

namespace BlendInt {

// forward declare
class AbstractLayout;
class LinearLayout;

/**
 * @brief Common dialog with a customized layout
 *
 * @ingroup blendint_gui_frames
 */
class Dialog: public AbstractDialog
{

public:

  /**
   * @brief Constructor of Dialog
   * @param[in] title The title of this dialog
   * @param[in] layout The layout used for content
   * @param[in] flags Flag for this dialog
   *
   * @note Destroy the layout will destoy this dialog too.
   */
  Dialog (const String& title, AbstractLayout* layout = 0, int flags = 0);

  virtual ~Dialog ();

  AbstractWidget* AddWidget (AbstractWidget* widget);

  AbstractWidget* InsertWidget (int index, AbstractWidget* widget);

  virtual Size GetPreferredSize () const;

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual bool PreDraw (AbstractWindow* context);

  virtual Response Draw (AbstractWindow* context);

private:

  void OnCloseButtonClicked ();

  void OnApplyButtonClicked ();

  void OnOKButtonClicked ();

  // 0: inner
  // 1: outer
  GLuint vao_[2];

  GLBuffer<ARRAY_BUFFER, 2> vbo_;

  glm::mat4 projection_matrix_;

  glm::mat3 model_matrix_;

  RefPtr<FrameShadow> shadow_;

  AbstractLayout* content_layout_;

  LinearLayout* main_layout_;
};

}
