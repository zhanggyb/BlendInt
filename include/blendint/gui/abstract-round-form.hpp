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

#include <blendint/gui/abstract-form.hpp>

namespace BlendInt {

class AbstractRoundForm: public AbstractForm
{
 public:

  AbstractRoundForm ();

  virtual ~AbstractRoundForm ();

  void SetRoundType (int type);

  void SetRadius (float radius);

  int round_type () const
  {
    return round_type_;
  }

  float radius () const
  {
    return radius_;
  }

  virtual void DrawInRect (const Rect& rect,
                           int align,
                           const float* color_ptr =
                           Color(Palette::White).data(),
                           short gamma = 0,
                           float rotate = 0.f,
                           bool scale = false) const
  {
    // TODO: implement in sub classes.
  }

 protected:

  virtual void PerformRoundTypeUpdate (int round_type) = 0;

  virtual void PerformRoundRadiusUpdate (float radius) = 0;

  void set_round_type (int type)
  {
    round_type_ = type;
  }

  void set_radius (float radius)
  {
    radius_ = radius;
  }

  void GenerateRoundedVertices (std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  void GenerateRoundedVertices (Orientation shadedir,
                                short shadetop,
                                short shadedown,
                                std::vector<GLfloat>* inner,
                                std::vector<GLfloat>* outer);

  static int GetOutlineVertexCount (int round_type);

 private:

  int round_type_;

  float radius_;
};

}
