/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BlendInt/VertexIcon.hpp>

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

	}

	VertexIcon::~VertexIcon ()
	{

	}

	void VertexIcon::display()
	{
		float num_vert[3][2];
		for (int i = 0; i < 3; i++)
		{
			num_vert[i][0] = num_tria_vert[i][0] * 20;
			num_vert[i][1] = num_tria_vert[i][1] * 20;
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, num_vert);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, num_tria_face);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

}
