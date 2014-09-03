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
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		GLfloat verts [] = {
				-(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f,
				(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f
		};

		if(axis_x_) {
			axis_x_->bind();
			axis_x_->set_data(sizeof(verts), verts);
		}

		if(axis_y_) {
			verts [0] = 0.f;
			verts[3] = 0.f;
			verts[1] = - (GLfloat)(lines_ / 2) * scale_;
			verts[4] = (GLfloat)(lines_ / 2) * scale_;

			axis_y_->bind();
			axis_y_->set_data(sizeof(verts), verts);
		}

		if(axis_z_) {
			verts [1] = 0.f;
			verts[4] = 0.f;
			verts[2] = - (GLfloat)(lines_ / 2) * scale_;
			verts[5] = (GLfloat)(lines_ / 2) * scale_;

			axis_y_->bind();
			axis_y_->set_data(sizeof(verts), verts);
		}

		GLArrayBuffer::reset();
	}

	void GridFloor::SetScale (float scale)
	{
		scale_ = scale;

		std::vector<GLfloat> vertices;
		GenerateGripFloorVertices(vertices);
		buffer_->bind();
		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		GLfloat verts [] = {
				-(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f,
				(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f
		};

		if(axis_x_) {
			axis_x_->bind();
			axis_x_->set_data(sizeof(verts), verts);
		}

		if(axis_y_) {
			verts [0] = 0.f;
			verts[3] = 0.f;
			verts[1] = - (GLfloat)(lines_ / 2) * scale_;
			verts[4] = (GLfloat)(lines_ / 2) * scale_;

			axis_y_->bind();
			axis_y_->set_data(sizeof(verts), verts);
		}

		if(axis_z_) {
			verts [1] = 0.f;
			verts[4] = 0.f;
			verts[2] = - (GLfloat)(lines_ / 2) * scale_;
			verts[5] = (GLfloat)(lines_ / 2) * scale_;

			axis_y_->bind();
			axis_y_->set_data(sizeof(verts), verts);
		}

		GLArrayBuffer::reset();
	}

	void GridFloor::Render (const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
	{
		int max = lines_ / 2;
		int total_count = max * 2 * 2 * 2;

		RefPtr<GLSLProgram> program = Shaders::instance->primitive_program();
		program->Use();

		glUniformMatrix4fv(Shaders::instance->location(Stock::PRIMITIVE_PROJECTION), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		glUniformMatrix4fv(Shaders::instance->location(Stock::PRIMITIVE_VIEW), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(Shaders::instance->location(Stock::PRIMITIVE_MODEL), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0)));

		if(vaos_[0] != 0) {
			glVertexAttrib4f(Shaders::instance->location(Stock::PRIMITIVE_COLOR), 0.35f, 0.35f, 0.35f, 1.f);
			glBindVertexArray(vaos_[0]);
			glDrawArrays(GL_LINES, 0, total_count);
		}

		if(vaos_[1] != 0) {	// show x axis
			glVertexAttrib4f(Shaders::instance->location(Stock::PRIMITIVE_COLOR), 1.f, 0.f, 0.f, 0.5f);
			glBindVertexArray(vaos_[1]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if(vaos_[2] != 0) {	// show y axis
			glVertexAttrib4f(Shaders::instance->location(Stock::PRIMITIVE_COLOR), 0.f, 1.f, 0.f, 0.5f);
			glBindVertexArray(vaos_[2]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if(vaos_[3] != 0) {	// show z axis
			glVertexAttrib4f(Shaders::instance->location(Stock::PRIMITIVE_COLOR), 0.f, 0.f, 1.f, 0.5f);
			glBindVertexArray(vaos_[3]);
			glDrawArrays(GL_LINES, 0, 2);
		}

		glBindVertexArray(0);

		program->reset();
	}

	GridFloor::~GridFloor ()
	{
		glDeleteVertexArrays(4, vaos_);
	}

	void GridFloor::InitializeGrid()
	{
		std::vector<GLfloat> vertices;

		GenerateGripFloorVertices(vertices);

		memset(vaos_, 0, 4);
		glGenVertexArrays(3, vaos_);
		glBindVertexArray(vaos_[0]);

		buffer_.reset(new GLArrayBuffer);
		buffer_->generate();
		buffer_->bind();

		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);

		GLfloat verts [] = {
				-(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f,
				(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f
		};

		axis_x_.reset(new GLArrayBuffer);
		axis_x_->generate();
		axis_x_->bind();
		axis_x_->set_data(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[2]);

		verts [0] = 0.f;
		verts[3] = 0.f;
		verts[1] = - (GLfloat)(lines_ / 2) * scale_;
		verts[4] = (GLfloat)(lines_ / 2) * scale_;

		axis_y_.reset(new GLArrayBuffer);
		axis_y_->generate();
		axis_y_->bind();
		axis_y_->set_data(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		/*
		glBindVertexArray(vaos_[3]);

		verts [1] = 0.f;
		verts[4] = 0.f;
		verts[2] = - (GLfloat)(lines_ / 2) * scale_;
		verts[5] = (GLfloat)(lines_ / 2) * scale_;

		axis_y_.reset(new GLArrayBuffer);
		axis_y_->generate();
		axis_y_->bind();
		axis_y_->set_data(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);
		*/

		glBindVertexArray(0);

		GLArrayBuffer::reset();
		GLElementArrayBuffer::reset();
	}

	void GridFloor::SetAxis (const char* str)
	{
		if(str == 0) return;

		int flag = 0;
		const char *p = str;
		while (*p != '\0') {
			switch(*p) {
				case 'x':
				case 'X': {
					flag |= (0x1 << 0);
					break;
				}

				case 'y':
				case 'Y': {
					flag |= (0x1 << 1);
					break;
				}

				case 'z':
				case 'Z': {
					flag |= (0x1 << 2);
					break;
				}

				default:
					break;
			}

			p++;
		}

		GLfloat verts [] = {
				-(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f,
				(GLfloat)(lines_ / 2) * scale_, 0.f, 0.f
		};

		if(flag & 0x1) {

			if(vaos_[1] == 0) {
				glGenVertexArrays(1, &vaos_[1]);
			}
			glBindVertexArray(vaos_[1]);

			if(!axis_x_) {
				axis_x_.reset(new GLArrayBuffer);
				axis_x_->generate();
			}
			axis_x_->bind();
			axis_x_->set_data(sizeof(verts), verts);
			glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
			glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		} else {
			glDeleteVertexArrays(1, &vaos_[1]);
			vaos_[1] = 0;
			axis_x_.destroy();
		}

		if(flag & (0x1 << 1)) {
			verts [0] = 0.f;
			verts[3] = 0.f;
			verts[1] = - (GLfloat)(lines_ / 2) * scale_;
			verts[4] = (GLfloat)(lines_ / 2) * scale_;

			if(vaos_[2] == 0) {
				glGenVertexArrays(1, &vaos_[2]);
			}
			glBindVertexArray(vaos_[2]);

			if(!axis_y_) {
				axis_y_.reset(new GLArrayBuffer);
				axis_y_->generate();
			}
			axis_y_->bind();
			axis_y_->set_data(sizeof(verts), verts);
			glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
			glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		} else {
			glDeleteVertexArrays(1, &vaos_[2]);
			vaos_[2] = 0;
			axis_y_.destroy();
		}

		if(flag & (0x1 << 2)) {

			verts [1] = 0.f;
			verts[4] = 0.f;
			verts[2] = - (GLfloat)(lines_ / 2) * scale_;
			verts[5] = (GLfloat)(lines_ / 2) * scale_;

			if(vaos_[3] == 0) {
				glGenVertexArrays(1, &vaos_[3]);
			}

			glBindVertexArray(vaos_[3]);

			if(!axis_z_) {
				axis_z_.reset(new GLArrayBuffer);
				axis_z_->generate();
			}

			axis_z_->bind();
			axis_z_->set_data(sizeof(verts), verts);
			glEnableVertexAttribArray(Shaders::instance->location(Stock::PRIMITIVE_COORD));
			glVertexAttribPointer(Shaders::instance->location(Stock::PRIMITIVE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		} else {
			glDeleteVertexArrays(1, &vaos_[3]);
			vaos_[3] = 0;
			axis_z_.destroy();
		}

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	void GridFloor::GenerateGripFloorVertices (std::vector<GLfloat>& vertices)
	{
		int max = lines_ / 2;

		unsigned int total_count = 3 * max * 2 * 2 * 2;
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

//		vertices[count + 0] = (GLfloat)(-max) * scale_;
//		vertices[count + 1] = 0.f;
//		vertices[count + 2] = 0.f;
//
//		vertices[count + 3] = (GLfloat)(max) * scale_;
//		vertices[count + 4] = 0.f;
//		vertices[count + 5] = 0.f;
//
//		count += 6;

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

//		vertices[count + 0] = 0.f;
//		vertices[count + 1] = (GLfloat)(-max) * scale_;
//		vertices[count + 2] = 0.f;
//
//		vertices[count + 3] = 0.f;
//		vertices[count + 4] = (GLfloat)(max) * scale_;
//		vertices[count + 5] = 0.f;
//
//		count += 6;

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
