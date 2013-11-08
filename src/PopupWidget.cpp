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
#include <math.h>

#include <BlendInt/PopupWidget.hpp>

#include <BlendInt/Theme.hpp>

namespace BlendInt {

	PopupWidget::PopupWidget()
	: Widget()
	{
		reset_z(1);	// TODO: define layer in enumeration
	}

	PopupWidget::~PopupWidget()
	{

	}

	void PopupWidget::update(int type, const void* data)
	{
		Widget::update(type, data);

		switch(type) {

			case FormPropertySize: {

				Size shadow_size = *(static_cast<const Size*>(data));

						int step, totvert;

						float inner_v[WIDGET_SIZE_MAX][2];
						float outer_v[WIDGET_SIZE_MAX][2];

						float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];

						// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
						const float radout = themes()->menu_shadow_width * 1.0;

						/* prevent tooltips to not show round shadow */
//						if (radout > 0.2f * size().height())
//							shadow_size.add_height(-0.2f * size().height());
//						else
//							shadow_size.add_height(-radout);

						totvert = generate_shadow_vertices(&shadow_size, 5.0, 0.0f, inner_v);

						for (step = 1; step <= (int)radout; step++) {
							generate_shadow_vertices(&shadow_size, 5.0, (float)step, outer_v);
							verts_to_quad_strip(inner_v, outer_v, totvert, quad_strip);

							glbuffer().create(ShadowBufferKeyBase + step);
							glbuffer().select(ShadowBufferKeyBase + step);
							glbuffer().set_property(totvert * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

							glbuffer().bind();
							glbuffer().upload(quad_strip);
							glbuffer().unbind();
						}


				break;
			}

			default:
				break;
		}

	}

	void PopupWidget::render ()
	{

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//		draw_shadow(5.0);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());
		draw_gl_buffer(FormBufferKeyInner);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_gl_buffer_anti_alias(FormBufferKeyOuter);

		if(emboss()) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			draw_gl_buffer_anti_alias(FormBufferKeyEmboss);
		}

		float alphastep;
		int step;

		// #define UI_DPI_FAC ((U.pixelsize * (float)U.dpi) / 72.0f)
		const float radout = themes()->menu_shadow_width * 1.0;
		alphastep = 3.0f * themes()->menu_shadow_fac / radout;

		float expfac = 0.0;

		for (step = 1; step <= (int)radout; step++) {
			expfac = sqrt(step / radout);

			glColor4f(0.0f, 0.0f, 0.0f, alphastep * (1.0f - expfac));

			draw_gl_buffer(ShadowBufferKeyBase + step, GL_QUAD_STRIP);
		}

		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void PopupWidget::draw_shadow(const float radin)
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
