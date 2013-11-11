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

#include <BlendInt/Shadow.hpp>

#include <iostream>

namespace BlendInt {

	Shadow::Shadow()
	: AbstractRoundBox(),
	  m_offset_x(0),
	  m_offset_y(0),
	  m_direction(ShadowAll),
	  m_blur_rad(5.0)
	{

	}

	Shadow::~Shadow()
	{

	}

	void Shadow::update (int type, const void* data)
	{

	}

	void Shadow::render ()
	{

	}

	int Shadow::generate_shadow_vertices (
			const Size* size,
			float rad,
			float step,
			float vert[WIDGET_SIZE_MAX][2])
	{
#ifdef DEBUG
		static bool debug_print = true;
#endif

		float vec[WIDGET_CURVE_RESOLU][2];
		float minx, miny, maxx, maxy;
		int i, tot = 0;

		rad += step;

		if (2.0f * rad > size->height())
			rad = 0.5f * size->height();

		minx = 0.0f - step;
		miny = 0.0f - step;
		maxx = size->width() + step;
		maxy = size->height() + step;

		/* mult */
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		/* start with left-top, anti clockwise */
		if (round_type() & RoundTopLeft) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx + rad - vec[i][0];
				vert[tot][1] = maxy - vec[i][1];
			}
		} else {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = maxy;
			}
		}

		if (round_type() & RoundBottomLeft) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx + vec[i][1];
				vert[tot][1] = miny + rad - vec[i][0];
			}
		} else {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = miny;
			}
		}

		if (round_type() & RoundBottomRight) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx - rad + vec[i][0];
				vert[tot][1] = miny + vec[i][1];
			}
		} else {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = miny;
			}
		}

		if (round_type() & RoundTopRight) {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx - vec[i][1];
				vert[tot][1] = maxy - rad + vec[i][0];
			}
		} else {
			for (i = 0; i < WIDGET_CURVE_RESOLU; i++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = maxy;
			}
		}

#ifdef DEBUG
		if(debug_print) {

			int count = 0;

			std::cout << "-----------------------------------------------------------" << std::endl;

			std::cout << "Top Left:" << std::endl;

			for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
			{
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
			}

			std::cout << "Bottom Left:" << std::endl;

			for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
			{
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
			}

			std::cout << "Bottom Right:" << std::endl;

			for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
			{
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
			}

			std::cout << "Top Right:" << std::endl;

			for(int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++)
			{
				std::cout << "    " << vert[count][0] << " " << vert[count][1] << std::endl;
			}

//			debug_print = false;
		}
#endif

		return tot;
	}
}
