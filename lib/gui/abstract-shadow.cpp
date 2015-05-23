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

#include <blendint/gui/abstract-shadow.hpp>
#include <blendint/gui/abstract-window.hpp>

#ifdef DEBUG
#include <cassert>
#endif

namespace BlendInt {

  AbstractShadow::AbstractShadow (short shadow_width)
  : AbstractRoundForm(), shadow_width_(shadow_width)
  {

  }

  AbstractShadow::~AbstractShadow ()
  {
  }

  void AbstractShadow::GenerateShadowVertices (std::vector<GLfloat>& vertices,
                                               std::vector<GLuint>& elements)
  {
    int width = shadow_width_;

    float rad = radius() * theme()->pixel();

    float minx = 0.0f;
    float miny = 0.0f;
    float maxx = size().width();
    float maxy = size().height();

    if (2.0f * rad > maxy) rad = 0.5f * maxy;

    maxy -= 2 * rad;

    float vec[WIDGET_CURVE_RESOLU][2];

    width *= theme()->pixel();

    int outline_vertex_count = GetOutlineVertexCount(round_type());
    unsigned int verts_num = (width + 1) * outline_vertex_count * 3;// 3 float for one vertex: 0, 1: coord, 2: shade

    if (vertices.size() != verts_num) {
      vertices.resize(verts_num);
    }

    float alpha = 1.f;
    int count = 0;
    for (int i = 0; i <= width; i++) {

      for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
        vec[j][0] = rad * cornervec[j][0];
        vec[j][1] = rad * cornervec[j][1];
      }

      //shade = 1.0 - std::sqrt(i * (1.0 / width));
      alpha = 1.0 - std::pow(i * (1.0 / width), 1.0 / 3);

      // for shadow, start from left-top

      // corner left-top
      if (round_type() & RoundTopLeft) {
        for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
          vertices[count + 0] = minx + rad - vec[j][0];
          vertices[count + 1] = maxy - vec[j][1];
          vertices[count + 2] = alpha;
          count += 3;
        }
      } else {
        vertices[count + 0] = minx;
        vertices[count + 1] = maxy;
        vertices[count + 2] = alpha;
        count += 3;
      }

      // corner left-bottom
      if (round_type() & RoundBottomLeft) {
        for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
          vertices[count + 0] = minx + vec[j][1];
          vertices[count + 1] = miny + rad - vec[j][0];
          vertices[count + 2] = alpha;
          count += 3;
        }
      } else {
        vertices[count + 0] = minx;
        vertices[count + 1] = miny;
        vertices[count + 2] = alpha;
        count += 3;
      }

      // corner right-bottom
      if (round_type() & RoundBottomRight) {
        for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
          vertices[count + 0] = maxx - rad + vec[j][0];
          vertices[count + 1] = miny + vec[j][1];
          vertices[count + 2] = alpha;
          count += 3;
        }
      } else {
        vertices[count + 0] = maxx;
        vertices[count + 1] = miny;
        vertices[count + 2] = alpha;
        count += 3;
      }

      // corner right-top
      if (round_type() & RoundTopRight) {
        for (int j = 0; j < WIDGET_CURVE_RESOLU; j++) {
          vertices[count + 0] = maxx - vec[j][1];
          vertices[count + 1] = maxy - rad + vec[j][0];
          vertices[count + 2] = alpha;
          count += 3;
        }
      } else {
        vertices[count + 0] = maxx;
        vertices[count + 1] = maxy;
        vertices[count + 2] = alpha;
        count += 3;
      }

      rad += 1.f;
      minx -= 1.f;
      miny -= 1.f;
      maxx += 1.f;
      maxy += 1.f;
    }

#ifdef DEBUG
    DBG_ASSERT(count == (int )verts_num);
#endif

    unsigned int elements_num = outline_vertex_count * 2 * width;

    if (elements.size() != elements_num) {
      elements.resize(elements_num);
    }

    count = 0;
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < (int) outline_vertex_count; j++) {
        elements[count + 0] = i * outline_vertex_count + j;
        elements[count + 1] = (i + 1) * outline_vertex_count + j;
        count += 2;
      }
    }

#ifdef DEBUG
    DBG_ASSERT(count == (int )elements_num);
#endif
  }

}
