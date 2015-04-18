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

#include <gui/linear-layout.hpp>
#include <gui/button-group.hpp>
#include <gui/stack.hpp>
#include <gui/abstract-frame.hpp>
#include <gui/frame-shadow.hpp>

namespace BlendInt {

  class ToolBar: public AbstractFrame
  {
  public:

    ToolBar (unsigned int color = 0x999999FF,
             bool shaded = false,
             short shadetop = 15,
             short shadedown = 0);

    virtual ~ToolBar ();

    virtual Size GetPreferredSize () const override;

    void LoadTools ();

    CppEvent::EventRef<int, bool> toggled()
    {
      return toggled_;
    }

  protected:

    virtual bool SizeUpdateTest (const AbstractView* source, const AbstractView* target, int width, int height) final;

    virtual void PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height) final;

    virtual bool PreDraw (AbstractWindow* context) final;

    virtual Response Draw (AbstractWindow* context) final;

    virtual void PostDraw (AbstractWindow* context) final;

    virtual Response PerformMouseHover (AbstractWindow* context) final;

    virtual void PerformFocusOn (AbstractWindow* context);

    virtual void PerformFocusOff (AbstractWindow* context);

    virtual void PerformHoverIn (AbstractWindow* context);

    virtual void PerformHoverOut (AbstractWindow* context);

    virtual Response PerformKeyPress (AbstractWindow* context);

    virtual Response PerformMousePress (AbstractWindow* context);

    virtual Response PerformMouseMove (AbstractWindow* context);

    virtual Response PerformMouseRelease (AbstractWindow* context);

  private:

    void InitializeToolBar ();

    LinearLayout* CreateRadioLayout ();

    Stack* CreateStack ();

    void SetFocusedWidget (AbstractWidget* widget, AbstractWindow* context);

    void OnFocusedWidgetDestroyed (AbstractWidget* widget);

    void OnHoverWidgetDestroyed (AbstractWidget* widget);

    void OnToggleStack (int index, bool toggled);

    glm::mat4 projection_matrix_;

    glm::mat3 model_matrix_;

    RefPtr<FrameShadow> shadow_;

    LinearLayout* layout_;

    AbstractWidget* focused_widget_;

    AbstractWidget* hovered_widget_;

    // 0 - for inner
    GLuint vao_;

    GLBuffer<ARRAY_BUFFER, 1> vbo_;

    Color color_;

    bool shaded_;

    short shadetop_;

    short shadedown_;

    bool focused_;

    bool hover_;

    bool pressed_;

    int cursor_position_;

    ButtonGroup radio_group_;

    Stack* stack_;

    CppEvent::Event<int, bool> toggled_;
  };

}
