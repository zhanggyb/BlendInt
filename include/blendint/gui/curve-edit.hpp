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

#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/abstract-widget.hpp>

namespace BlendInt {

/*
 * Currently for debuging shader
 */

/**
 * @brief A widget to show and edit curve
 *
 */
class CurveEdit: public AbstractWidget
{
 public:

  CurveEdit ();

  virtual ~CurveEdit ();

  virtual Size GetPreferredSize () const;
  
  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

 protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);
  
  virtual Response Draw (AbstractWindow* context);

 private:

  GLuint vao_[2];

  GLBuffer<ARRAY_BUFFER, 2> vbo_;
  
};

}
