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

#include <stdlib.h>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	namespace Stock {

		Icons* Icons::instance = 0;

		bool Icons::Initialize ()
		{
			if (!instance)
				instance = new Icons;

			return true;
		}

		void Icons::Release ()
		{
			if (instance) {
				delete instance;
				instance = 0;
			}
		}

		Icons::Icons ()
		{
			CreateIcons();
		}

		Icons::~Icons ()
		{
		}

		void Icons::CreateIcons ()
		{
			float vec[16][2];

			m_icon_menu.reset(new VertexIcon);
			DBG_SET_NAME(m_icon_menu, "Menu Icon");

			for (size_t i = 0; i < 6; i++) {
				vec[i][0] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][0];
				vec[i][1] = 0.5 * 16 * VertexIcon::menu_tria_vert[i][1];
			}

			m_icon_menu->Load(vec, 6, VertexIcon::menu_tria_face, 2);

			m_icon_circle.reset(new VertexIcon);
			DBG_SET_NAME(m_icon_circle, "Circle Icon");

			for (size_t i = 0; i < 16; i++) {
				vec[i][0] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][0];
				vec[i][1] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][1];
			}

			m_icon_circle->Load(vec, 16, VertexIcon::scroll_circle_face, 14);

			m_icon_check.reset(new VertexIcon);
			DBG_SET_NAME(m_icon_check, "Checkbox Icon");

			for (size_t i = 0; i < 6; i++) {
				vec[i][0] = 0.5 * 14 * VertexIcon::check_tria_vert[i][0];
				vec[i][1] = 0.5 * 14 * VertexIcon::check_tria_vert[i][1];
			}

			m_icon_check->Load(vec, 6, VertexIcon::check_tria_face, 4);

			m_icon_num.reset(new VertexIcon);
			DBG_SET_NAME(m_icon_num, "Number Slider Icon");

			for (size_t i = 0; i < 3; i++) {
				vec[i][0] = 0.5 * 10 * VertexIcon::num_tria_vert[i][0];
				vec[i][1] = 0.5 * 10 * VertexIcon::num_tria_vert[i][1];
			}

			m_icon_num->Load(vec, 3, VertexIcon::num_tria_face, 1);
		}

	}

}
