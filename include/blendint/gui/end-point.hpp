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

#include <blendint/core/object.hpp>
#include <blendint/core/point.hpp>
#include <blendint/core/size.hpp>
#include <blendint/core/color.hpp>

#include <blendint/gui/abstract-node.hpp>

namespace BlendInt {

/**
 * @brief The end point of node
 *
 * @ingroup blendint_gui_nodes
 */
class EndPoint: public Object
{

public:

  EndPoint ();

  virtual EndPoint ();

  void Draw ();

private:

  AbstractNode* node_;

  EndPoint* upstream_;

  EndPoint* downstream_;

  EndPoint* previous_;

  EndPoint* next_;

  Point position_;

  Color color_;

  static Size kSize;

};

}
