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

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/mesh.hpp>

namespace BlendInt {

  const char* Mesh::vertex_shader =
      "#version 330\n"
      "layout (location = 0) in vec4 VertexPosition;"
      "layout (location = 1) in vec3 VertexNormal;"
      "out vec3 LightIntensity;"
      "uniform vec4 LightPosition;"	// Light position in eye coords.
      "uniform vec3 Kd;"// Diffuse reflectivity
      "uniform vec3 Ld;"// Light source intensity
      "uniform mat4 ModelViewMatrix;"
      "uniform mat3 NormalMatrix;"
      "uniform mat4 ProjectionMatrix;"
      "uniform mat4 MVP;"// Projection * ModelView
      ""
      "void main() {"
      "	vec3 tnorm = normalize( NormalMatrix * VertexNormal);"// Convert normal and position to eye coords
      "	vec4 eyeCoords = ModelViewMatrix * VertexPosition;"
      "	vec3 s = normalize(vec3(LightPosition - eyeCoords));"
      "	LightIntensity = Ld * Kd * max( dot( s, tnorm ), 0.0 );"// The diffuse shading equation
      "	gl_Position = MVP * VertexPosition;"// Convert position to clip coordinates and pass along
      "}";

  const char* Mesh::fragment_shader =
      "#version 330\n"
      "in vec3 LightIntensity;"
      "layout( location = 0 ) out vec4 FragColor;"
      ""
      "void main() {"
      "	FragColor = vec4(LightIntensity, 1.0);"
      "}";

  Mesh::Mesh ()
  : AbstractPrimitive(), vao_(0)
  {
    InitializeMesh();
  }

  Mesh::~Mesh ()
  {
    glDeleteVertexArrays(1, &vao_);
  }

  bool Mesh::Load (const char* filename)
  {
    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLushort> elements;

    if (!LoadObj(filename, vertices, normals, elements)) {
      return false;
    }

    glBindVertexArray(vao_);

    vertex_buffer_->bind();
    vertex_buffer_->set_data(vertices.size() * sizeof(vertices[0]),
                             &vertices[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    normal_buffer_->bind();
    normal_buffer_->set_data(normals.size() * sizeof(normals[0]), &normals[0]);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    index_buffer_->bind();
    index_buffer_->set_data(elements.size() * sizeof(GLushort), &elements[0]);

    glBindVertexArray(0);

    GLArrayBuffer::reset();
    GLElementArrayBuffer::reset();

    return true;
  }

  void Mesh::Render (const glm::mat4& projection_matrix,
                     const glm::mat4& view_matrix)
  {
    glBindVertexArray(vao_);

    glm::mat4 mv = view_matrix * model_matrix_;

    program_->use();
    program_->SetUniformMatrix4fv(
        "MVP", 1, GL_FALSE, glm::value_ptr(projection_matrix * view_matrix));
    program_->SetUniform3f("Kd", 0.9f, 0.9f, 0.9f);
    program_->SetUniform3f("Ld", 1.0f, 1.0f, 1.0f);
    program_->SetUniformMatrix4fv(
        "LightPosition", 1, GL_FALSE,
        glm::value_ptr(view_matrix * glm::vec4(8.0f, 10.0f, 16.0f, 1.0f)));
    program_->SetUniformMatrix4fv("ModelViewMatrix", 1, GL_FALSE,
                                  glm::value_ptr(mv));
    program_->SetUniformMatrix3fv(
        "NormalMatrix",
        1,
        GL_FALSE,
        glm::value_ptr(
            glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]))));
    index_buffer_->bind();

    int size;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    program_->reset();
  }

  bool Mesh::LoadObj (const char* filename,
                      std::vector<glm::vec4>& vertices,
                      std::vector<glm::vec3>& normals,
                      std::vector<GLushort>& elements)
  {
    using namespace std;

    ifstream in(filename, ios::in);
    if (!in) {
      cerr << "Cannot open " << filename << endl;
      return false;
    }

    string line;
    while (getline(in, line)) {
      if (line.substr(0, 2) == "v ") {
        istringstream s(line.substr(2));
        glm::vec4 v;
        s >> v.x;
        s >> v.y;
        s >> v.z;
        v.w = 1.0f;
        vertices.push_back(v);
      } else if (line.substr(0, 2) == "f ") {
        istringstream s(line.substr(2));
        GLushort a, b, c;
        s >> a;
        s >> b;
        s >> c;
        a--;
        b--;
        c--;
        elements.push_back(a);
        elements.push_back(b);
        elements.push_back(c);
      } else if (line[0] == '#') { /* ignoring this line */
      } else { /* ignoring this line */
      }
    }

    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (unsigned int i = 0; i < elements.size(); i += 3) {
      GLushort ia = elements[i];
      GLushort ib = elements[i + 1];
      GLushort ic = elements[i + 2];
      glm::vec3 normal = glm::normalize(
          glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
                     glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
      normals[ia] = normals[ib] = normals[ic] = normal;
    }

    return true;
  }

  void Mesh::InitializeMesh ()
  {
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    vertex_buffer_.reset(new GLArrayBuffer);
    vertex_buffer_->generate();

    normal_buffer_.reset(new GLArrayBuffer);
    normal_buffer_->generate();

    index_buffer_.reset(new GLElementArrayBuffer);
    index_buffer_->generate();

    glBindVertexArray(0);

    program_.reset(new GLSLProgram);
    program_->Create();

    program_->AttachShader(vertex_shader, GL_VERTEX_SHADER);
    program_->AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
    if (!program_->Link()) {
      DBG_PRINT_MSG("Fail to link the text program: %d", program_->id());
      exit(1);
    }

    model_matrix_ = glm::mat4(1.0f);
    model_matrix_ *= glm::rotate(model_matrix_,
                                 (glm::mediump_float) (M_PI * -35.0f),
                                 glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix_ *= glm::rotate(model_matrix_,
                                 (glm::mediump_float) (M_PI * 35.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
  }

}
