/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <algorithm>

#include <BIL/Widget.hpp>

namespace BIL {

	GLubyte const checker_stipple_sml[32 * 32 / 8] =
		{
			255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
			255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
			0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
			0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
			255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
			255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
			0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
			0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
		};

	static const float jit[WIDGET_AA_JITTER][2] = {
		{ 0.468813, -0.481430}, {-0.155755, -0.352820},
		{ 0.219306, -0.238501}, {-0.393286, -0.110949},
		{-0.024699,  0.013908}, { 0.343805,  0.147431},
		{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	Widget::Widget (Traceable *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub

		// Set the default padding and margin
		set_padding(Vec4i(1, 1, 1, 1));
		set_margin(Vec4i(1, 1, 1, 1));
	}

	Widget::~Widget ()
	{
		// TODO Auto-generated destructor stub
	}

	void Widget::DrawTrias (const Trias *tria)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, tria->vec);
		glDrawElements(GL_TRIANGLES,
					   tria->tot * 3,
					   GL_UNSIGNED_INT,
					   tria->index);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Widget::DrawWidgetBase (const Base* wtb, const WidgetColors* wcol)
	{
#ifdef LALALALA
		int j, a;
	
		glEnable(GL_BLEND);

		/* backdrop non AA */
		if (wtb->inner) {
			if (wcol->shaded == 0) {
				if (wcol->alpha_check) {
					float inner_v_half[WIDGET_SIZE_MAX][2];
					float x_mid = 0.0f; /* used for dumb clamping of values */

					/* dark checkers */
					glColor4ub(UI_TRANSP_DARK, UI_TRANSP_DARK, UI_TRANSP_DARK, 255);
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, wtb->inner_v);
					glDrawArrays(GL_POLYGON, 0, wtb->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					/* light checkers */
					glEnable(GL_POLYGON_STIPPLE);
					glColor4ub(UI_TRANSP_LIGHT, UI_TRANSP_LIGHT, UI_TRANSP_LIGHT, 255);
					glPolygonStipple(checker_stipple_sml);

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, wtb->inner_v);
					glDrawArrays(GL_POLYGON, 0, wtb->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					glDisable(GL_POLYGON_STIPPLE);

					/* alpha fill */
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glColor4ubv((unsigned char *)wcol->inner.data);
					glEnableClientState(GL_VERTEX_ARRAY);

					for (a = 0; a < wtb->totvert; a++) {
						x_mid += wtb->inner_v[a][0];
					}
					x_mid /= wtb->totvert;

					glVertexPointer(2, GL_FLOAT, 0, wtb->inner_v);
					glDrawArrays(GL_POLYGON, 0, wtb->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					/* 1/2 solid color */
					glColor4ub(wcol->inner.data[0], wcol->inner.data[1],
											wcol->inner.data[2], 255);

					for (a = 0; a < wtb->totvert; a++) {
						inner_v_half[a][0] = std::min(wtb->inner_v[a][0], x_mid);
						inner_v_half[a][1] = wtb->inner_v[a][1];
					}

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, inner_v_half);
					glDrawArrays(GL_POLYGON, 0, wtb->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);
				}
				else {
					/* simple fill */
					glColor4ubv((unsigned char *)wcol->inner.data);

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, wtb->inner_v);
					glDrawArrays(GL_POLYGON, 0, wtb->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);
				}
			}
			else {
				char col1[4], col2[4];
				unsigned char col_array[WIDGET_SIZE_MAX * 4];
				unsigned char *col_pt = col_array;
			
				prepare_shade_colors(col1,
									 col2,
									 wcol->inner.data,
									 wcol->shadetop,
									 wcol->shadedown);
			
				glShadeModel(GL_SMOOTH);
				for (a = 0; a < wtb->totvert; a++, col_pt += 4) {
					prepare_round_box_shade_colors
						(col_pt, col1, col2, wtb->inner_uv[a][wtb->shadedir]);
				}

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glVertexPointer(2, GL_FLOAT, 0, wtb->inner_v);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, col_array);
				glDrawArrays(GL_POLYGON, 0, wtb->totvert);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);

				glShadeModel(GL_FLAT);
			}
		}
	
		/* for each AA step */
		if (wtb->outline) {
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
			float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

			const unsigned char tcol[4] = {wcol->outline.data[0],
										   wcol->outline.data[1],
										   wcol->outline.data[2],
										   wcol->outline.data[3] / WIDGET_AA_JITTER};

			widget_verts_to_quad_strip(wtb, wtb->totvert, quad_strip);

			if (wtb->emboss) {
				widget_verts_to_quad_strip_open(wtb, wtb->halfwayvert, quad_strip_emboss);
			}

			glEnableClientState(GL_VERTEX_ARRAY);

			for (j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);
			
				/* outline */
				glColor4ubv(tcol);

				glVertexPointer(2, GL_FLOAT, 0, quad_strip);
				glDrawArrays(GL_QUAD_STRIP, 0, wtb->totvert * 2 + 2);
		
				/* emboss bottom shadow */
				if (wtb->emboss) {
					glColor4f(1.0f, 1.0f, 1.0f, 0.02f);

					glVertexPointer(2, GL_FLOAT, 0, quad_strip_emboss);
					glDrawArrays(GL_QUAD_STRIP, 0, wtb->halfwayvert * 2);
				}
			
				glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
		}
	
		/* decoration */
		if (wtb->tria1.tot || wtb->tria2.tot) {
			const unsigned char tcol[4] = {wcol->item[0],
										   wcol->item[1],
										   wcol->item[2],
										   (unsigned char)((float)wcol->item[3] / WIDGET_AA_JITTER)};
			/* for each AA step */
			for (j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);

				if (wtb->tria1.tot) {
					glColor4ubv(tcol);
					DrawTrias(&wtb->tria1);
				}
				if (wtb->tria2.tot) {
					glColor4ubv(tcol);
					DrawTrias(&wtb->tria2);
				}
		
				glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
			}
		}

		glDisable(GL_BLEND);
#endif	// LALALALA
	}

	void Widget::KeyPressEvent (KeyEvent* event)
	{
		event->Accept();
	}

	void Widget::ContextMenuPressEvent (ContextMenuEvent* event)
	{
		event->Accept();
	}

	void Widget::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
		event->Accept();
	}

	void Widget::MousePressEvent (MouseEvent* event)
	{
		event->Accept();
	}

	void Widget::MouseReleaseEvent (MouseEvent* event)
	{
		event->Accept();
	}

	void Widget::MouseMoveEvent (MouseEvent* event)
	{
		event->Accept();
	}

	void Widget::InputMethodEvent (unsigned int character)
	{
	}

	void Widget::CursorEnterEvent (int entered)
	{
	}

	void Widget::Render (void)
	{

	}

} /* namespace BIL */
