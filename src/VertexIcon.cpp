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

#include <GL/glew.h>
#include <iostream>

#include <BlendInt/VertexIcon.hpp>

#define WIDGET_AA_JITTER 8

namespace BlendInt {

	static const float jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	const float VertexIcon::num_tria_vert[3][2] ={
			{ -0.352077, 0.532607 },
			{ -0.352077, -0.549313 },
			{ 0.330000, -0.008353 }
	};

	const unsigned int VertexIcon::num_tria_face[1][3] = {
			{ 0, 1, 2 }
	};

	const float VertexIcon::scroll_circle_vert[16][2] = {
	        { 0.382684, 0.923879 }, { 0.000001, 1.000000 }, { -0.382683,
	                0.923880 }, { -0.707107, 0.707107 },
	        { -0.923879, 0.382684 }, { -1.000000, 0.000000 }, { -0.923880,
	                -0.382684 }, { -0.707107, -0.707107 }, { -0.382683,
	                -0.923880 }, { 0.000000, -1.000000 },
	        { 0.382684, -0.923880 }, { 0.707107, -0.707107 }, { 0.923880,
	                -0.382684 }, { 1.000000, -0.000000 },
	        { 0.923880, 0.382683 }, { 0.707107, 0.707107 } };

	const unsigned int VertexIcon::scroll_circle_face[14][3] = { { 0, 1, 2 }, {
	        2, 0, 3 }, { 3, 0, 15 }, { 3, 15, 4 }, { 4, 15, 14 }, { 4, 14, 5 },
	        { 5, 14, 13 }, { 5, 13, 6 }, { 6, 13, 12 }, { 6, 12, 7 }, { 7, 12,
	                11 }, { 7, 11, 8 }, { 8, 11, 10 }, { 8, 10, 9 } };

	const float VertexIcon::menu_tria_vert[6][2] =
	        { { -0.33, 0.16 }, { 0.33, 0.16 }, { 0, 0.82 }, { 0, -0.82 }, {
	                -0.33, -0.16 }, { 0.33, -0.16 } };

	const unsigned int VertexIcon::menu_tria_face[2][3] = { { 2, 0, 1 }, { 3, 5,
	        4 } };

	const float VertexIcon::check_tria_vert[6][2] = { { -0.578579, 0.253369 }, {
	        -0.392773, 0.412794 }, { -0.004241, -0.328551 }, { -0.003001,
	        0.034320 }, { 1.055313, 0.864744 }, { 0.866408, 1.026895 } };

	const unsigned int VertexIcon::check_tria_face[4][3] = { { 3, 2, 4 }, { 3,
	        4, 5 }, { 1, 0, 3 }, { 0, 2, 3 } };

	VertexIcon::VertexIcon ()
	{
		m_size.set_width(16);
		m_size.set_height(16);
	}

	VertexIcon::~VertexIcon ()
	{

	}

	void VertexIcon::demo_init ()
	{
		float menu[6][2];

		for(size_t i = 0; i < 6; i++)
		{
			menu[i][0] = m_size.width() * menu_tria_vert[i][0];
			menu[i][1] = m_size.height() * menu_tria_vert[i][1];
		}

		load (menu, 6, menu_tria_face, 2);
	}

	void VertexIcon::load (const float (*vertex_array)[2], size_t array_size,
			const unsigned int (*vertex_indices)[3], size_t indeces_size)
	{
		m_gl_buffer.generate(2);

		m_gl_buffer.set_index(0);	// 0 for ARRAY BUFFER
		m_gl_buffer.set_property(array_size, sizeof(vertex_array[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_gl_buffer.bind();
		m_gl_buffer.upload(vertex_array);
		m_gl_buffer.unbind();

		m_gl_buffer.set_index(1);	// 1 for ELEMENT ARRAY BUFFER
		m_gl_buffer.set_property(indeces_size, sizeof(vertex_indices[0]), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_gl_buffer.bind();
		m_gl_buffer.upload(vertex_indices);
		m_gl_buffer.unbind();
	}

	void VertexIcon::display()
	{
		m_gl_buffer.set_index(0);
		m_gl_buffer.bind();

		glVertexPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);

		m_gl_buffer.set_index(1);
		m_gl_buffer.bind();

		/* for each AA step */
		for (int i = 0; i < WIDGET_AA_JITTER; i++) {
			glTranslatef(jit[i][0], jit[i][1], 0.0f);
			glDrawElements(GL_TRIANGLES, m_gl_buffer.vertices() * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
			glTranslatef(-jit[i][0], -jit[i][1], 0.0f);
		}

		glDisableClientState(GL_VERTEX_ARRAY);

		m_gl_buffer.unbind(0);
		m_gl_buffer.unbind(1);
	}

	void VertexIcon::display (float x, float y)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(x, y, 0);

		display();

		glPopMatrix();
	}

	void VertexIcon::resize (int w, int h)
	{
		m_size.set_width(w);
		m_size.set_height(h);
	}

	void VertexIcon::resize (const Size& size)
	{
		m_size = size;
	}

	void VertexIcon::reload ()
	{

	}

}
