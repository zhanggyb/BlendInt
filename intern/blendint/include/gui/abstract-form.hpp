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

#include <vector>

#include <opengl/opengl.hpp>

#include <core/types.hpp>
#include <core/rect.hpp>
#include <core/color.hpp>
#include <core/object.hpp>

namespace BlendInt {

  /**
   * @brief Abstract form class
   *
   * @ingroup blendint_gui_forms
   */
  class AbstractForm: public Object
  {
  public:

    AbstractForm ()
        : Object()
    {
    }

    AbstractForm (int width, int height)
        : Object(), size_(width, height)
    {
    }

    AbstractForm (const Size& size)
        : Object(), size_(size)
    {
    }

    virtual ~AbstractForm ()
    {
    }

    AbstractForm (const AbstractForm& orig)
    {
      size_ = orig.size_;
    }

    AbstractForm& operator = (const AbstractForm& orig)
    {
      size_ = orig.size();
      return *this;
    }

    void Resize (int width, int height);

    void Resize (const Size& size);

    const Size& size () const
    {
      return size_;
    }

    virtual void Draw (int x,
                       int y,
                       const float* color_ptr = Color(Palette::Black).data(),
                       short gamma = 0,
                       float rotate = 0.f,
                       float scale_x = 1.f,
                       float scale_y = 1.f) const = 0;

    virtual void DrawInRect (const Rect& rect,
                             int align,
                             const float* color_ptr =
                                 Color(Palette::White).data(),
                             short gamma = 0,
                             float rotate = 0.f,
                             bool scale = false) const = 0;

    static void SetDefaultBorderWidth (int border);

    static inline float default_border_width ()
    {
      return kBorderWidth;
    }

    static int GetOutlineVertices (int round_type);

    static inline int outline_vertex_count(int round_type)
    {
      return kOutlineVertexTable[round_type & 0x0F];
    }

  protected:

    virtual void PerformSizeUpdate (int width, int height) = 0;

    /**
     * @brief preset the size of the form
     * @param width
     * @param height
     *
     * @note this function should be called only in the constructor of subclass to set
     * the size without through Update() for performance.
     */
    inline void set_size (int width, int height)
    {
      size_.set_width(width);
      size_.set_height(height);
    }

    /**
     * @brief preset the size of the form
     * @param size
     *
     * @note this function should be called only in the constructor of subclass to set
     * the size without through Update() for performance.
     */
    inline void set_size (const Size& size)
    {
      size_ = size;
    }

    static void GenerateVertices (float xmin,
                                  float ymin,
                                  float xmax,
                                  float ymax,
                                  float border,
                                  int round_type,
                                  float radius,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static void GenerateVertices (float xmin,
                                  float ymin,
                                  float xmax,
                                  float ymax,
                                  float border,
                                  int round_type,
                                  float radius,
                                  Orientation shadedir,
                                  short shadetop,
                                  short shadedown,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static void GenerateVertices (const Size& size,
                                  float border,
                                  int round_type,
                                  float radius,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static void GenerateVertices (const Size& size,
                                  float border,
                                  int round_type,
                                  float radius,
                                  Orientation shadedir,
                                  short shadetop,
                                  short shadedown,
                                  std::vector<GLfloat>* inner,
                                  std::vector<GLfloat>* outer);

    static const float cornervec[WIDGET_CURVE_RESOLU][2];

  private:

    static inline float make_shaded_offset (short shadetop,
                                            short shadedown,
                                            float fact);

    static void GenerateTriangleStripVertices (const std::vector<GLfloat>* inner,
                                               const std::vector<GLfloat>* edge,
                                               unsigned int num,
                                               std::vector<GLfloat>* strip);

    Size size_;

    static float kBorderWidth;

    static const int kOutlineVertexTable[16];

  };

}
