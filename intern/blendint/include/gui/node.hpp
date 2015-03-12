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

#include <gui/abstract-layout.hpp>
#include <gui/abstract-node.hpp>
#include <gui/widget-shadow.hpp>

namespace BlendInt {

  /**
   * @brief A special view used to display and manage a node in NodeView
   *
   * @ingroup blendint_gui_nodes
   */
  class Node: public AbstractNode
  {
  public:

    Node (AbstractLayout* layout,
          unsigned int inner_color = 0x999999FF,
          unsigned int outer_color = 0x191919FF,
          bool shaded = false,
          short shadetop = 0,
          short shadedown = 0);

    virtual ~Node ();

    bool AddWidget (AbstractWidget* widget);

    bool InsertWidget (int index, AbstractWidget* widget);

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

    virtual Size GetPreferredSize () const;

    void SetInnerColor (unsigned int color);

    void SetOuterColor (unsigned int color);

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual void PerformRoundTypeUpdate (int round_type);

    virtual void PerformRoundRadiusUpdate (float radius);

    virtual Response Draw (AbstractWindow* context);

  private:

    unsigned int inner_color_;

    unsigned int outer_color_;

    bool shaded_;

    short shadetop_;

    short shadedown_;

    GLuint vao_[2];

    GLBuffer<ARRAY_BUFFER, 2> vbo_;

    AbstractLayout* layout_;

    RefPtr<WidgetShadow> shadow_;

  };

}
