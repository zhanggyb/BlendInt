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
 * License along with BlendInt.	 If not, see
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
#endif  // __UNIX__

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexIcon.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#define WIDGET_AA_JITTER 8

namespace BlendInt {

	using Stock::Shaders;

	const float VertexIcon::num_tria_vert[3][2] ={
		{ -0.352077, 0.532607 },
		{ -0.352077, -0.549313 },
		{ 0.330000, -0.008353 }
	};

	const unsigned int VertexIcon::num_tria_face[1][3] = {
		{ 0, 1, 2 }
	};

	const float VertexIcon::scroll_circle_vert[16][2] = {
		{ 0.382684, 0.923879 }, { 0.000001, 1.000000 },
		{ -0.382683, 0.923880 }, { -0.707107, 0.707107 },
		{ -0.923879, 0.382684 }, { -1.000000, 0.000000 },
		{ -0.923880, -0.382684 }, { -0.707107, -0.707107 },
		{ -0.382683, -0.923880 }, { 0.000000, -1.000000 },
		{ 0.382684, -0.923880 }, { 0.707107, -0.707107 },
		{ 0.923880, -0.382684 }, { 1.000000, -0.000000 },
		{ 0.923880, 0.382683 }, { 0.707107, 0.707107 }
	};

	const unsigned int VertexIcon::scroll_circle_face[14][3] = {
		{ 0, 1, 2 },
		{2, 0, 3 },
		{ 3, 0, 15 },
		{ 3, 15, 4 },
		{ 4, 15, 14 },
		{ 4, 14, 5 },
		{ 5, 14, 13 },
		{ 5, 13, 6 },
		{ 6, 13, 12 },
		{ 6, 12, 7 },
		{ 7, 12, 11 },
		{ 7, 11, 8 },
		{ 8, 11, 10 },
		{ 8, 10, 9 }
	};

	const float VertexIcon::menu_tria_vert[6][2] = {
		{ -0.33, 0.16 }, { 0.33, 0.16 }, { 0, 0.82 },
		{ 0, -0.82 }, {-0.33, -0.16 }, { 0.33, -0.16 }
	};

	const unsigned int VertexIcon::menu_tria_face[2][3] = {
		{ 2, 0, 1 }, { 3, 5, 4 }
	};

	const float VertexIcon::check_tria_vert[6][2] = {
		{ -0.578579, 0.253369 }, {-0.392773, 0.412794 },
		{ -0.004241, -0.328551 }, { -0.003001, 0.034320 },
		{ 1.055313, 0.864744 }, { 0.866408, 1.026895 }
	};

	const unsigned int VertexIcon::check_tria_face[4][3] = {
		{ 3, 2, 4 }, { 3, 4, 5 }, { 1, 0, 3 }, { 0, 2, 3 }
	};

	VertexIcon::VertexIcon (int width, int height)
	: Icon(width, height),
	  vertex_buffer_(0),
	  element_buffer_(0),
	  vao_(0)
	{
		vertex_buffer_.reset(new GLArrayBuffer);
		element_buffer_.reset(new GLElementArrayBuffer);

		glGenVertexArrays(1, &vao_);
	}

	VertexIcon::~VertexIcon ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void VertexIcon::Load (const float (*vertex_array)[2], size_t array_size,
						   const unsigned int (*vertex_indices)[3], size_t indeces_size)
	{
		glBindVertexArray(vao_);

		vertex_buffer_->Generate();
		vertex_buffer_->Bind();
		vertex_buffer_->SetData(array_size * sizeof(vertex_array[0]), vertex_array[0]);

		element_buffer_->Generate();
		element_buffer_->Bind();
		element_buffer_->SetData(indeces_size, sizeof(vertex_indices[0]), vertex_indices[0]);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		vertex_buffer_->Reset();
		element_buffer_->Reset();
	}

	void VertexIcon::Draw(const glm::vec3& pos, short gamma) const
	{
		Color color (0.1f, 0.1f, 0.1f, 0.125f);

		Draw(pos, color, gamma);
	}

	void VertexIcon::Draw(const glm::vec3& pos, const Color& color, short gamma) const
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3fv(Shaders::instance->triangle_uniform_position(), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), gamma);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(), color.data());

		glBindVertexArray(vao_);

		vertex_buffer_->Bind();	// bind ARRAY BUFFER
		element_buffer_->Bind();	// bind ELEMENT ARRAY BUFFER

		glDrawElements(GL_TRIANGLES, element_buffer_->vertices() * 3,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		element_buffer_->Reset();
		vertex_buffer_->Reset();

		program->Reset();
	}

	void VertexIcon::Draw(const glm::vec3& pos, float angle, float scale, const Color& color, short gamma) const
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3fv(Shaders::instance->triangle_uniform_position(), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), gamma);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);

		glUniform1f(Shaders::instance->triangle_uniform_rotation(), angle);
		glUniform2f(Shaders::instance->triangle_uniform_scale(), scale, scale);
		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(), color.data());

		glBindVertexArray(vao_);

		vertex_buffer_->Bind();	// bind ARRAY BUFFER
		element_buffer_->Bind();	// bind ELEMENT ARRAY BUFFER

		glDrawElements(GL_TRIANGLES, element_buffer_->vertices() * 3,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glBindVertexArray(0);

		element_buffer_->Reset();
		vertex_buffer_->Reset();

		glUniform1f(Shaders::instance->triangle_uniform_rotation(), 0.f);
		glUniform2f(Shaders::instance->triangle_uniform_scale(), 1.f, 1.f);

		program->Reset();
	}

}

