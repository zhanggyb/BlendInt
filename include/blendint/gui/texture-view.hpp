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

#include <blendint/core/image.hpp>

#include <blendint/opengl/gl-texture2d.hpp>
#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/chess-board.hpp>
#include <blendint/gui/abstract-scrollable.hpp>

namespace BlendInt {

class TextureView: public AbstractScrollable
{
public:

  TextureView ();

  virtual ~TextureView ();

  bool OpenFile (const char* filename);

  void LoadImage (const RefPtr<Image>& image);

  void SetTexture (const RefPtr<GLTexture2D>& texture);

  void Clear ();

  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

  virtual Size GetPreferredSize () const;

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual bool PreDraw (AbstractWindow* context);

  virtual Response Draw (AbstractWindow* context);

  virtual void PostDraw (AbstractWindow* context);

private:

  void InitializeImageView ();

  // void AdjustImageArea (const Size& size);

  Size image_size_;

  /**
   * @brief Vertex Array Objects
   *
   * 0 - for background
   * 1 - for plane to display image texture
   */

  GLuint vao_[2];
  GLBuffer<ARRAY_BUFFER, 2> vbo_;

  RefPtr<GLTexture2D> texture_;

  RefPtr<ChessBoard> chessboard_;
};

}
