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

#include <opengl/gl-buffer.hpp>

#include <gui/abstract-frame.hpp>

namespace BlendInt {

class AbstractLayout;
// forward declare

/**
 * @brief A common frame with customized layout
 *
 * @ingroup blendint_gui_frames
 */
class Frame: public AbstractFrame
{
public:

  Frame (AbstractLayout* layout);

  Frame (int width, int height, AbstractLayout* layout);

  virtual ~Frame ();

  AbstractWidget* AddWidget (AbstractWidget* widget);

  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

  virtual Size GetPreferredSize () const;

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height) final;

  virtual bool PreDraw (AbstractWindow* context) final;

  virtual Response Draw (AbstractWindow* context) final;

  virtual void PostDraw (AbstractWindow* context) final;

  virtual void PerformFocusOn (AbstractWindow* context);

  virtual void PerformFocusOff (AbstractWindow* context);

  virtual void PerformHoverIn (AbstractWindow* context);

  virtual void PerformHoverOut (AbstractWindow* context);

  virtual Response PerformKeyPress (AbstractWindow* context);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

  virtual Response PerformMouseMove (AbstractWindow* context);

  virtual Response PerformMouseHover (AbstractWindow* context) final;

  inline AbstractLayout* layout () const
  {
    return layout_;
  }

private:

  void InitializeFrameOnce ();

  void SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context);

  void OnFocusedWidgetDestroyed (AbstractWidget* widget);

  void OnHoverWidgetDestroyed (AbstractWidget* widget);

  glm::mat4 projection_matrix_;

  glm::mat3 model_matrix_;

  AbstractWidget* focused_widget_;

  AbstractWidget* hovered_widget_;

  // 0 - for inner
  // 1 - for outer
  GLuint vao_[2];

  GLBuffer<ARRAY_BUFFER, 2> vbo_;

  int cursor_position_;

  AbstractLayout* layout_;

  bool focused_;

  bool hover_;

  bool pressed_;
};

}
