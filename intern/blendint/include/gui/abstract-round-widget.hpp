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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#pragma once

#include <gui/abstract-widget.hpp>

namespace BlendInt {

  /**
   * @brief A Normal widget
   *
   * @ingroup blendint_gui_widgets
   */
  class AbstractRoundWidget: public AbstractWidget
  {
  DISALLOW_COPY_AND_ASSIGN(AbstractRoundWidget);

  public:

    AbstractRoundWidget ();

    AbstractRoundWidget (int width, int height);

    virtual ~AbstractRoundWidget ();

    void SetRoundRadius (float radius);

    void SetRoundType (int type);

    void SetEmboss (bool emboss);

    inline uint32_t round_type () const
    {
      return widget_flag_ & 0x0F;
    }

    inline float round_radius () const
    {
      return round_radius_;
    }

    inline bool emboss () const
    {
      return widget_flag_ & WidgetEmboss;
    }

  protected:

    void GenerateRoundedVertices (std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    void GenerateRoundedVertices (Orientation shadedir,
                                  short shadetop,
                                  short shadedown,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    virtual void PerformRoundTypeUpdate (int round_type);

    virtual void PerformRoundRadiusUpdate (float radius);

    virtual void PerformEmbossUpdate (bool emboss);

    virtual Response Draw (AbstractWindow* context);

    inline void set_round_type (int type)
    {
      widget_flag_ = (widget_flag_ & 0xFFF0) + (type & 0x0F);
    }

    inline void set_round_radius (float radius)
    {
      round_radius_ = radius;
    }

    inline void set_emboss (bool emboss)
    {
      if (emboss) {
        SETBIT(widget_flag_, WidgetEmboss);
      } else {
        CLRBIT(widget_flag_, WidgetEmboss);
      }
    }

  private:

    enum WidgetFlagIndex
    {

      WidgetRoundTopLeft = (1 << 0),

      WidgetRoundTopRight = (1 << 1),

      WidgetRoundBottomRight = (1 << 2),

      WidgetRoundBottomLeft = (1 << 3),

      WidgetEmboss = (1 << 4),

    };

    uint32_t widget_flag_;

    float round_radius_;

  };

}
