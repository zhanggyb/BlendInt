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

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Cube.hpp>

#include <BlendInt/ShaderManager.hpp>

namespace BlendInt {

	Cube::Cube()
	: AbstractPrimitive(), vbo_cube_vertices(0),
	vbo_cube_colors(0), ibo_cube_elements(0), program(0),
	attribute_coord3d (0), attribute_v_color(0), uniform_mvp(0)
	{

	}

	void Cube::Render (const glm::mat4& mvp)
	{
		//glClearColor(1.0, 1.0, 1.0, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);
		glEnableVertexAttribArray(attribute_coord3d);
		// Describe our vertices array to OpenGL (it can't guess its format automatically)
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glVertexAttribPointer(attribute_coord3d, // attribute
		        3,                // number of elements per vertex, here (x,y,z)
		        GL_FLOAT,          // the type of each element
		        GL_FALSE,          // take our values as-is
		        0,                 // no extra data between each position
		        0                  // offset of first element
		        );

		glEnableVertexAttribArray(attribute_v_color);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glVertexAttribPointer(attribute_v_color, // attribute
		        3,                // number of elements per vertex, here (R,G,B)
		        GL_FLOAT,          // the type of each element
		        GL_FALSE,          // take our values as-is
		        0,                 // no extra data between each position
		        0                  // offset of first element
		        );

		/* Push each element in buffer_vertices to the vertex shader */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);

		/*
		int size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT,
		        0);
		 */
		//std::cout << "size: " << size << std::endl;
		glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_SHORT, 0);


		glDisableVertexAttribArray(attribute_v_color);
		glDisableVertexAttribArray(attribute_coord3d);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
	}

	int Cube::InitOnce()
	{
		GLfloat cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0,
				1.0, -1.0,  1.0,
				1.0,  1.0,  1.0,
				-1.0,  1.0,  1.0,
				// back
				-1.0, -1.0, -1.0,
				1.0, -1.0, -1.0,
				1.0,  1.0, -1.0,
				-1.0,  1.0, -1.0,
		};

		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

		GLfloat cube_colors[] = {
				// front colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
				// back colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
		};

		glGenBuffers(1, &vbo_cube_colors);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);

		GLushort cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// top
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// bottom
				4, 0, 3,
				3, 7, 4,
				// left
				4, 5, 1,
				1, 0, 4,
				// right
				3, 2, 6,
				6, 7, 3,
		};

		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

		GLint link_ok = GL_FALSE;

		GLuint vs, fs;

		if ((vs = create_shader("cube.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
		if ((fs = create_shader("cube.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

		program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			fprintf(stderr, "glLinkProgram:");
			print_log(program);
			return 0;
		}

		const char* attribute_name;
		attribute_name = "coord3d";
		attribute_coord3d = glGetAttribLocation(program, attribute_name);
		if (attribute_coord3d == -1) {
			fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
			return 0;
		}

		attribute_name = "v_color";
		attribute_v_color = glGetAttribLocation(program, attribute_name);
		if (attribute_v_color == -1) {
			fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
			return 0;
		}

		const char* uniform_name;
		uniform_name = "mvp";
		uniform_mvp = glGetUniformLocation(program, uniform_name);
		if (uniform_mvp == -1) {
			fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
			return 0;
		}

		float angle = 0;  // 45бу per second
		glm::vec3 axis_y(0, 1, 0);
		glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*5/4, 0.1f, 10.0f);

		mvp = projection * view * model * anim;

		glUseProgram(program);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
		glUseProgram(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}

	Cube::~Cube()
	{
		glDeleteProgram(program);
		glDeleteBuffers(1, &vbo_cube_vertices);
		glDeleteBuffers(1, &vbo_cube_colors);
		glDeleteBuffers(1, &ibo_cube_elements);
	}

}
