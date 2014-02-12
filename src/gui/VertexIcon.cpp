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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/VertexIcon.hpp>

#define WIDGET_AA_JITTER 8

namespace BlendInt {

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

	VertexIcon::VertexIcon ()
		: Icon(), m_array_buffer(0), m_index_buffer(0)
	{
		set_size(16, 16);
		m_array_buffer = new GLArrayBuffer;
		Retain(m_array_buffer);

		m_index_buffer = new GLElementArrayBuffer;
		Retain(m_index_buffer);
	}

	VertexIcon::~VertexIcon ()
	{
		Destroy(m_array_buffer);
		Destroy(m_index_buffer);
	}

	void VertexIcon::load (const float (*vertex_array)[2], size_t array_size,
						   const unsigned int (*vertex_indices)[3], size_t indeces_size)
	{
		m_array_buffer->Generate();
		m_array_buffer->Bind();
		m_array_buffer->SetData(array_size, sizeof(vertex_array[0]), vertex_array[0]);
		m_array_buffer->ResetID();

		m_index_buffer->Generate();
		m_index_buffer->Bind();
		m_index_buffer->SetData(indeces_size, sizeof(vertex_indices[0]), vertex_indices[0]);
		m_index_buffer->ResetID();
	}

	void VertexIcon::Update (int type, const void* data)
	{
		// TODO: update shape
	}

	void VertexIcon::Draw()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(size().width()/2.0, size().height()/2.0, 0.0);

		if (m_array_buffer && m_index_buffer) {

			m_array_buffer->Bind();	// bind ARRAY BUFFER

			m_index_buffer->Bind();	// bind ELEMENT ARRAY BUFFER

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

			/* for each AA step */
			for (int i = 0; i < WIDGET_AA_JITTER; i++) {
				glTranslatef(jit[i][0], jit[i][1], 0.0f);
				glDrawElements(GL_TRIANGLES, m_index_buffer->vertices() * 3,
							   GL_UNSIGNED_INT, BUFFER_OFFSET(0));
				glTranslatef(-jit[i][0], -jit[i][1], 0.0f);
			}

			glDisableClientState(GL_VERTEX_ARRAY);

			m_index_buffer->ResetID();
			m_array_buffer->ResetID();
		}

		glPopMatrix();

		//Icon::Draw();
	}

}
