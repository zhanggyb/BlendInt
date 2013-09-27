/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_VERTEXICON_HPP_
#define _BILO_VERTEXICON_HPP_

namespace BILO {

	/**
	 * Icon displayed with vertexes
	 */
	class VertexIcon
	{
	public:

		VertexIcon ();

		~VertexIcon ();

		void display ();

	private:

		static const float num_tria_vert[3][2];

		static const unsigned int num_tria_face[1][3];

		static const float scroll_circle_vert[16][2];

		static const unsigned int scroll_circle_face[14][3];

		static const float menu_tria_vert[6][2];

		static const unsigned int menu_tria_face[2][3];

		static const float check_tria_vert[6][2];

		static const unsigned int check_tria_face[4][3];

	};
}

#endif /* VERTEXICON_HPP_ */
