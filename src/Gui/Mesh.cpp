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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Gui/Mesh.hpp>

namespace BlendInt {

	const char* Mesh::vertex_shader =
			"#version 330\n"
			""
			"layout(location = 0) in vec4 v_coord;"
			"layout(location = 1) in vec3 v_normal;"
			"uniform mat4 MVP;"
			""
			"void main(void) {"
			"	gl_Position = MVP * v_coord;"
			"}";

	const char* Mesh::fragment_shader =
			"#version 330\n"
			""
			"out vec4 FragmentColor;"
			""
			"void main(void) {"
			"	FragmentColor = vec4(0.8, 0.8, 0.8, 1.0);"
			"}";

	Mesh::Mesh ()
	: AbstractPrimitive(),
	  m_vao(0)
	{
		InitializeMesh();
	}

	void Mesh::Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
	{
		glBindVertexArray(m_vao);

		m_program->Use();
		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(projection_matrix * view_matrix));

		glBindVertexArray(m_vao);

		m_index_buffer->Bind();

		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_program->Reset();
	}

	Mesh::~Mesh ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Mesh::LoadObj (const char* filename, std::vector<glm::vec4>& vertices,
	        std::vector<glm::vec3>& normals, std::vector<GLushort>& elements)
	{
		using namespace std;

		ifstream in(filename, ios::in);
		if (!in) {
			cerr << "Cannot open " << filename << endl;
			exit(1);
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
			        glm::cross(
			                glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			                glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
			normals[ia] = normals[ib] = normals[ic] = normal;
		}
	}

	void Mesh::InitializeMesh ()
	{
		std::vector<glm::vec4> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLushort> elements;

		LoadObj("test.obj", vertices, normals, elements);

		DBG_PRINT_MSG("vertex size: %ld", vertices.size());
		DBG_PRINT_MSG("normal size: %ld", normals.size());
		DBG_PRINT_MSG("elements size: %ld", elements.size());

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_vertex_buffer.reset(new GLArrayBuffer);
		m_vertex_buffer->Generate();
		m_vertex_buffer->Bind();
		m_vertex_buffer->SetData(vertices.size() * sizeof(GLfloat), &vertices[0]);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		m_normal_buffer.reset(new GLArrayBuffer);
		m_normal_buffer->Generate();
		m_normal_buffer->Bind();
		m_normal_buffer->SetData(normals.size() * sizeof(GLfloat), &normals[0]);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		m_index_buffer.reset(new GLElementArrayBuffer);
		m_index_buffer->Generate();
		m_index_buffer->Bind();
		m_index_buffer->SetData(elements.size() * sizeof(GLushort), &elements[0]);

		GLElementArrayBuffer::Reset();

		m_program.reset(new GLSLProgram);
		m_program->Create();

		m_program->AttachShader(vertex_shader, GL_VERTEX_SHADER);
		m_program->AttachShader(fragment_shader, GL_FRAGMENT_SHADER);
		if (!m_program->Link()) {
			DBG_PRINT_MSG("Fail to link the text program: %d",
					m_program->id());
			exit(1);
		}

	}

}
