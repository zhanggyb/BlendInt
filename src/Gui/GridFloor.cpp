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

#include <math.h>
#include <assert.h>

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

// vec3, vec4, ivec4, mat4
#include <glm/glm.hpp>
// translate, rotate, scale, perspective
#include <glm/gtc/matrix_transform.hpp>
// value_ptr
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/OpenGL/GLSLProgram.hpp>
#include <BlendInt/Gui/GridFloor.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	GridFloor::GridFloor ()
	: AbstractPrimitive(),
	  vao_(0),
	  lines_(16),
	  scale_(1.f),
	  subdivisions_(10)
	{
		InitializeGrid();
	}

	void GridFloor::SetLines (int lines)
	{
		lines_ = lines;

		std::vector<GLfloat> vertices;
		GenerateGripFloorVertices(vertices);
		buffer_->Bind();
		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->Reset();
	}

	void GridFloor::SetScale (float scale)
	{
		scale_ = scale;

		std::vector<GLfloat> vertices;
		GenerateGripFloorVertices(vertices);
		buffer_->Bind();
		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		buffer_->Reset();
	}

	void GridFloor::Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
	{
		int max = lines_ / 2;
		int total_count = max * 2 * 2 * 2 + 2 * 2;

		RefPtr<GLSLProgram> program = Shaders::instance->primitive_program();
		program->Use();

		glUniformMatrix4fv(Shaders::instance->primitive_uniform_projection(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniformMatrix4fv(Shaders::instance->primitive_uniform_view(), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(Shaders::instance->primitive_uniform_model(), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

		glVertexAttrib3f(Shaders::instance->primitive_attrib_color_v3(), 0.35f, 0.35f, 0.35f);

		/* Draw the grid using the indices to our vertices using our vertex buffer objects */
		glBindVertexArray(vao_);
		glDrawArrays(GL_LINES, 0, total_count);
		glBindVertexArray(0);

		program->Reset();
	}

	GridFloor::~GridFloor ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void GridFloor::InitializeGrid()
	{
		std::vector<GLfloat> vertices;

		GenerateGripFloorVertices(vertices);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.reset(new GLArrayBuffer);
		buffer_->Generate();
		buffer_->Bind();

		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		/* Draw the grid using the indices to our vertices using our vertex buffer objects */
		glEnableVertexAttribArray(Shaders::instance->primitive_attrib_coord_v3());
		glVertexAttribPointer(Shaders::instance->primitive_attrib_coord_v3(), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		GLArrayBuffer::Reset();
		GLElementArrayBuffer::Reset();
	}

	void GridFloor::GenerateGripFloorVertices (std::vector<GLfloat>& vertices)
	{
		int max = lines_ / 2;

		unsigned int total_count = 3 * max * 2 * 2 * 2 + 3 * 2 * 2;
				//		  ~ 3  (x,y,z) coord of 1 point
				//			max * 2, horizontal lines
				//			max * 2 * 2, horizontal + vertical lines

		if(vertices.size() != total_count) {
			vertices.resize(total_count);
		}

		unsigned int count = 0;
		for(int i = 0; i < max; i++)
		{
			vertices[count + 0] = (GLfloat)(-max) * scale_;
			vertices[count + 1] = (GLfloat)(-max + i) * scale_;
			vertices[count + 2] = 0.f;

			vertices[count + 3] = (GLfloat)(max) * scale_;
			vertices[count + 4] = (GLfloat)(-max + i) * scale_;
			vertices[count + 5] = 0.f;

			count += 6;
		}

		vertices[count + 0] = (GLfloat)(-max) * scale_;
		vertices[count + 1] = 0.f;
		vertices[count + 2] = 0.f;

		vertices[count + 3] = (GLfloat)(max) * scale_;
		vertices[count + 4] = 0.f;
		vertices[count + 5] = 0.f;

		count += 6;

		for(int i = 0; i < max; i++)
		{
			vertices[count + 0] = (GLfloat)(-max) * scale_;
			vertices[count + 1] = (GLfloat)(i + 1) * scale_;
			vertices[count + 2] = 0.f;

			vertices[count + 3] = (GLfloat)(max) * scale_;
			vertices[count + 4] = (GLfloat)(i + 1) * scale_;
			vertices[count + 5] = 0.f;

			count += 6;
		}

		for(int i = 0; i < max; i++)
		{
			vertices[count + 0] = (GLfloat)(-max + i) * scale_;
			vertices[count + 1] = (GLfloat)(-max) * scale_;
			vertices[count + 2] = 0.f;

			vertices[count + 3] = (GLfloat)(-max + i) * scale_;
			vertices[count + 4] = (GLfloat)(max) * scale_;
			vertices[count + 5] = 0.f;

			count += 6;
		}

		vertices[count + 0] = 0.f;
		vertices[count + 1] = (GLfloat)(-max) * scale_;
		vertices[count + 2] = 0.f;

		vertices[count + 3] = 0.f;
		vertices[count + 4] = (GLfloat)(max) * scale_;
		vertices[count + 5] = 0.f;

		count += 6;

		for(int i = 0; i < max; i++)
		{
			vertices[count + 0] = (GLfloat)(i + 1) * scale_;
			vertices[count + 1] = (GLfloat)(-max) * scale_;
			vertices[count + 2] = 0.f;

			vertices[count + 3] = (GLfloat)(i + 1) * scale_;
			vertices[count + 4] = (GLfloat)(max) * scale_;
			vertices[count + 5] = 0.f;

			count += 6;
		}

		assert(count == total_count);
	}

}
