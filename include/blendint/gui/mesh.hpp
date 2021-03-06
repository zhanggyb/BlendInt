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

#include <blendint/opengl/glarraybuffer.hpp>
#include <blendint/opengl/glsl-program.hpp>
#include <blendint/opengl/glelementarraybuffer.hpp>

#include <blendint/gui/abstract-primitive.hpp>

namespace BlendInt {

  class Mesh: public AbstractPrimitive
  {
  public:

    Mesh ();

    virtual ~Mesh ();

    bool Load (const char* filename);

    virtual void Render (const glm::mat4& projection_matrix,
                         const glm::mat4& view_matrix);

    static bool LoadObj (const char* filename,
                         std::vector<glm::vec4>& vertices,
                         std::vector<glm::vec3>& normals,
                         std::vector<GLushort>& elements);

  private:

    void InitializeMesh ();

    GLuint vao_;

    RefPtr<GLArrayBuffer> vertex_buffer_;
    RefPtr<GLArrayBuffer> normal_buffer_;

    RefPtr<GLElementArrayBuffer> index_buffer_;

    RefPtr<GLSLProgram> program_;

    glm::mat4 model_matrix_;

    static const char* vertex_shader;
    static const char* fragment_shader;
  };

}
