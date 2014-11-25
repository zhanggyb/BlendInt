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

#include <BlendInt/Gui/VectorIcon.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#ifndef WIDGET_AA_JITTER
#define WIDGET_AA_JITTER 8
#endif

namespace BlendInt {

	using Stock::Shaders;

	const float VectorIcon::num_tria_vert[3][2] ={
		{ -0.352077, 0.532607 },
		{ -0.352077, -0.549313 },
		{ 0.330000, -0.008353 }
	};

	const unsigned int VectorIcon::num_tria_face[1][3] = {
		{ 0, 1, 2 }
	};

	const float VectorIcon::scroll_circle_vert[16][2] = {
		{ 0.382684, 0.923879 }, { 0.000001, 1.000000 },
		{ -0.382683, 0.923880 }, { -0.707107, 0.707107 },
		{ -0.923879, 0.382684 }, { -1.000000, 0.000000 },
		{ -0.923880, -0.382684 }, { -0.707107, -0.707107 },
		{ -0.382683, -0.923880 }, { 0.000000, -1.000000 },
		{ 0.382684, -0.923880 }, { 0.707107, -0.707107 },
		{ 0.923880, -0.382684 }, { 1.000000, -0.000000 },
		{ 0.923880, 0.382683 }, { 0.707107, 0.707107 }
	};

	const unsigned int VectorIcon::scroll_circle_face[14][3] = {
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

	const float VectorIcon::menu_tria_vert[6][2] = {
		{ -0.33, 0.16 }, { 0.33, 0.16 }, { 0, 0.82 },
		{ 0, -0.82 }, {-0.33, -0.16 }, { 0.33, -0.16 }
	};

	const unsigned int VectorIcon::menu_tria_face[2][3] = {
		{ 2, 0, 1 }, { 3, 5, 4 }
	};

	const float VectorIcon::check_tria_vert[6][2] = {
		{ -0.578579, 0.253369 }, {-0.392773, 0.412794 },
		{ -0.004241, -0.328551 }, { -0.003001, 0.034320 },
		{ 1.055313, 0.864744 }, { 0.866408, 1.026895 }
	};

	const unsigned int VectorIcon::check_tria_face[4][3] = {
		{ 3, 2, 4 }, { 3, 4, 5 }, { 1, 0, 3 }, { 0, 2, 3 }
	};

	VectorIcon::VectorIcon (int width, int height)
	: AbstractIcon(width, height),
	  vao_(0),
	  elements_(0)
	{
		glGenVertexArrays(1, &vao_);
	}

	VectorIcon::~VectorIcon ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void VectorIcon::Load (const float (*vertex_array)[2], size_t array_size,
						   const unsigned int (*vertex_indices)[3], size_t indeces_size)
	{
		glBindVertexArray(vao_);

		vertex_buffer_.generate();
		vertex_buffer_.bind();
		vertex_buffer_.set_data(array_size * sizeof(vertex_array[0]), vertex_array[0]);

		element_buffer_.generate();
		element_buffer_.bind();
		element_buffer_.set_data(indeces_size * sizeof(vertex_indices[0]), vertex_indices[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		vertex_buffer_.reset();
		element_buffer_.reset();

		elements_ = indeces_size * 3;
	}

	void VectorIcon::Draw(float x, float y, short gamma) const
	{
		Color color (0.1f, 0.1f, 0.1f, 0.125f);

		Draw(glm::vec2(x, y), color, gamma);
	}

	void VectorIcon::Draw (float x, float y, const Color& color, short gamma) const
	{
		Draw(glm::vec2(x, y), color, gamma);
	}

	void VectorIcon::Draw(const glm::vec2& pos, const Color& color, short gamma) const
	{
		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), pos.x, pos.y);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), gamma);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 1);

		glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), color.data());

		glBindVertexArray(vao_);

		vertex_buffer_.bind();	// bind ARRAY BUFFER
		element_buffer_.bind();	// bind ELEMENT ARRAY BUFFER

		glDrawElements(GL_TRIANGLES, elements_,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		element_buffer_.reset();
		vertex_buffer_.reset();

		program->reset();
	}

	void VectorIcon::Draw(const glm::vec2& pos, float angle, float scale, const Color& color, short gamma) const
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), pos.x, pos.y);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), gamma);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 1);

		glUniform1f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ROTATION), angle);
		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_SCALE), scale, scale);
		glVertexAttrib4fv(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), color.data());

		glBindVertexArray(vao_);

		vertex_buffer_.bind();	// bind ARRAY BUFFER
		element_buffer_.bind();	// bind ELEMENT ARRAY BUFFER

		glDrawElements(GL_TRIANGLES, elements_,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		glBindVertexArray(0);

		element_buffer_.reset();
		vertex_buffer_.reset();

		glUniform1f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ROTATION), 0.f);
		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_SCALE), 1.f, 1.f);

		program->reset();
	}

	void VectorIcon::PerformSizeUpdate(const Size& size)
	{
		set_size(size);
	}

}
