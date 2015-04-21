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

#include <blendint/core/point.hpp>

namespace BlendInt {

  /**
   * @brief Size
   *
   * only used in widgets/windows to define its size
   *
   * @ingroup blendint_core
   */
  class Size
  {
  public:

    Size ()
    : width_(0), height_(0)
    {

    }

    Size (int width, int height)
    : width_(width), height_(height)
    {

    }

    Size (const Size& orig)
    : width_(orig.width_), height_(orig.height_)
    {

    }

    Size& operator = (const Size& orig)
    {
      width_ = orig.width_;
      height_ = orig.height_;
      return *this;
    }

    bool is_valid () const
    {
      return (width_ >= 0) && (height_ >= 0);
    }

    inline bool equal (const Size& size)
    {
      return (width_ == size.width()) && (height_ == size.height());
    }

    inline bool equal (int width, int height)
    {
      return (width_ == width && height_ == height);
    }

    void reset (int width, int height)
    {
      width_ = width;
      height_ = height;
    }

    int width () const
    {
      return width_;
    }

    void set_width (int width)
    {
      width_ = width;
    }

    int height () const
    {
      return height_;
    }

    void set_height (int height)
    {
      height_ = height;
    }

    void add_width (int width)
    {
      width_ = width_ + width;
    }

    void add_height (int height)
    {
      height_ = height_ + height;
    }

  private:

    int width_;
    int height_;
  };

  /**
   * @brief compare two size object and return true if they are equal
   * @param size1
   * @param size2
   * @return
   */
  extern inline bool operator == (const Size& size1, const Size& size2)
  {
    return ((size1.width() == size2.width())
        && (size1.height() == size2.height()));
  }
}
