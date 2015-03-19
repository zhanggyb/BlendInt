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

#include <opengl/glarraybuffer.hpp>
#include <opengl/glelementarraybuffer.hpp>
#include <gui/abstract-primitive.hpp>

namespace BlendInt {

  class GridFloor: public AbstractPrimitive
  {
  public:

    GridFloor ();

    virtual ~GridFloor ();

    void SetLines (int lines);

    void SetScale (float scale);

    void SetAxis (const char* str);

    virtual void Render (const glm::mat4& projection_matrix,
                         const glm::mat4& view_matrix);

  private:

    void InitializeGrid ();

    void GenerateGripFloorVertices (std::vector<GLfloat>& vertices);

    /**
     * @brief Vertex array objects
     *
     * - 0: grid
     * - 1: x axis
     * - 2: y axis
     * - 3: z axis
     */
    GLuint vaos_[4];

    int lines_;
    float scale_;

    int subdivisions_;	// default is 10

    RefPtr<GLArrayBuffer> buffer_;

    RefPtr<GLArrayBuffer> axis_x_;
    RefPtr<GLArrayBuffer> axis_y_;
    RefPtr<GLArrayBuffer> axis_z_;
  };

}
