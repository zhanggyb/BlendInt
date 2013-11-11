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

#include <BlendInt/Shadow.hpp>
#include <BlendInt/Theme.hpp>

#include <math.h>

#include <iostream>

namespace BlendInt {

	Shadow::Shadow()
	: AbstractRoundBox(),
	  m_offset_x(0),
	  m_offset_y(0),
	  m_direction(ShadowAll),
	  m_blur_rad(20.0)
	{
		set_round_type(RoundAll);
//		m_offset_x = 5;
//		m_offset_y = 5;
	}

	Shadow::~Shadow()
	{

	}

	void Shadow::update (int type, const void* data)
	{
		switch (type) {

			case FormSize: {

				Size shadow_size = *(static_cast<const Size*>(data));

				int step, totvert;

				float inner_v[WIDGET_SIZE_MAX][2];
				float outer_v[WIDGET_SIZE_MAX][2];

				float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];

				// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
//				const float radout = themes()->menu_shadow_width * 1.0;

				/* prevent tooltips to not show round shadow */
//						if (radout > 0.2f * size().height())
//							shadow_size.add_height(-0.2f * size().height());
//						else
//							shadow_size.add_height(-radout);
				totvert = generate_shadow_vertices(&shadow_size, radius(), 0.0f,
				        inner_v);

				for (step = 1; step <= (int) m_blur_rad; step++) {
					generate_shadow_vertices(&shadow_size, radius(), (float) step,
					        outer_v);
					verts_to_quad_strip(inner_v, outer_v, totvert, quad_strip);

					m_gl_buffer.create(step);
					m_gl_buffer.select(step);
					m_gl_buffer.set_property(totvert * 2 + 2,
					        sizeof(quad_strip[0]), GL_ARRAY_BUFFER,
					        GL_STATIC_DRAW);

					m_gl_buffer.bind();
					m_gl_buffer.upload(quad_strip);
					m_gl_buffer.unbind();
				}

				break;
			}

			default:
				break;
		}
	}

	void Shadow::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x() + m_offset_x,
					 position().y() + m_offset_y, 0.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float alphastep;
		int step;

		// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
//		const float radout = themes()->menu_shadow_width * 1.0;
//		alphastep = 3.0f * themes()->menu_shadow_fac / radout;
		alphastep = 3.0f * themes()->menu_shadow_fac / m_blur_rad;

		float expfac = 0.0;

		for (step = 1; step <= (int)m_blur_rad; step++) {
			expfac = sqrt(step / m_blur_rad);

			glColor4f(0.0f, 0.0f, 0.0f, alphastep * (1.0f - expfac));

			m_gl_buffer.select(step);
			m_gl_buffer.bind();
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);
			glDrawArrays(GL_QUAD_STRIP, 0, m_gl_buffer.vertices());
			glDisableClientState(GL_VERTEX_ARRAY);
			m_gl_buffer.unbind();
		}

		glDisable(GL_BLEND);
		glPopMatrix();
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


	void Shadow::draw(const float radin)
	{
		float alphastep;
		int step, totvert;

		float inner_v[WIDGET_SIZE_MAX][2];
		float outer_v[WIDGET_SIZE_MAX][2];

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];

		// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
		const float radout = themes()->menu_shadow_width * 1.0;

		Size shadow_size = size();

		/* disabled shadow */
		if (radout == 0.0f)
			return;

		/* prevent tooltips to not show round shadow */
		if (radout > 0.2f * size().height())
			shadow_size.add_height(-0.2f * size().height());
		else
			shadow_size.add_height(-radout);

		/* inner part */
		//totvert = round_box_shadow_edges(wtb.inner_v, &rect1, radin, roundboxalign & (UI_CNR_BOTTOM_RIGHT | UI_CNR_BOTTOM_LEFT), 0.0f);
		totvert = generate_shadow_vertices(&shadow_size, radin, 0.0f, inner_v);

		/* we draw a number of increasing size alpha quad strips */
		// alphastep = 3.0f * btheme->tui.menu_shadow_fac / radout;
		alphastep = 3.0f * themes()->menu_shadow_fac / radout;

		glEnableClientState(GL_VERTEX_ARRAY);

		for (step = 1; step <= (int)radout; step++) {
			float expfac = sqrt(step / radout);

			//round_box_shadow_edges(wtb.outer_v, &rect1, radin, UI_CNR_ALL, (float)step);
			generate_shadow_vertices(&shadow_size, radin, (float)step, outer_v);

//#ifdef DEBUG
//			glColor4f(0.9f, 0.0f, 0.0f, alphastep * (1.0f - expfac));
//#else
			glColor4f(0.0f, 0.0f, 0.0f, alphastep * (1.0f - expfac));
//#endif

			//widget_verts_to_quad_strip(&wtb, totvert, quad_strip);
			verts_to_quad_strip(inner_v, outer_v, totvert, quad_strip);

			glVertexPointer(2, GL_FLOAT, 0, quad_strip);
			glDrawArrays(GL_QUAD_STRIP, 0, totvert * 2); /* add + 2 for getting a complete soft rect. Now it skips top edge to allow transparent menus */
		}

		glDisableClientState(GL_VERTEX_ARRAY);

	}

}
