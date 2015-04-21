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
#include <blendint/opengl/gl-texture2d.hpp>

#include <blendint/gui/abstract-form.hpp>

namespace BlendInt {

/**
 * @brief Form to draw a chess board
 *
 * A chess board is composed with a simple vbo to draw repeated texture.
 *
 * @ingroup blendint_gui_forms
 */
class ChessBoard: public AbstractForm
{
public:

  ChessBoard (int w = 20, int height = 20);

  virtual ~ChessBoard ();

  virtual void Draw (int x, // x coord
                     int y, // y coord
                     const float* color_ptr = Color(Palette::White).data(), // color
                     short gamma = 0,
                     float rotate = 0.f,
                     float scale_x = 1.f,
                     float scale_y = 1.f) const;

  virtual void DrawInRect (const Rect& rect,  // rectangel to draw
                           int align, // alignment
                           const float* color_ptr =
                               Color(Palette::White).data(), // color
                           short gamma = 0, // gamma
                           float rotate = 0.f,  // rotate
                           bool scale = false // scale
                           ) const;

protected:

  virtual void PerformSizeUpdate (int width, int height);

private:

  GLuint vao_;

  GLBuffer<ARRAY_BUFFER, 1> vbo_;

  RefPtr<GLTexture2D> texture_;

  static const int kCellWidth = 20;

  static const int kCellHeight = 20;
};

} // namespace BlendInt
