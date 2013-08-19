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
#include <BIL/Types.hpp>
#include <BIL/Coord.hpp>
#include <BIL/Rect.hpp>

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BIL {

	/* *********************** draw data ************************** */

	static const float cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	const int WIDGET_AA_JITTER = 8;

	static const float jit[WIDGET_AA_JITTER][2] = {
		{ 0.468813, -0.481430}, {-0.155755, -0.352820},
		{ 0.219306, -0.238501}, {-0.393286, -0.110949},
		{-0.024699,  0.013908}, { 0.343805,  0.147431},
		{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

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

	/* helper call, makes shadow rect, with 'sun' above menu, so only shadow to left/right/bottom */
	/* return tot */
	static int round_box_shadow_edges(float (*vert)[2],
			const Recti& rect, float rad, int roundboxalign, float step)
	{
		if(!rect.valid()) return 0;

		float vec[WIDGET_CURVE_RESOLU][2];
		float minx, miny, maxx, maxy;
		int a, tot = 0;

		rad += step;

		if (2.0f * rad > rect.height())
			rad = 0.5f * rect.height();

		minx = rect.x() - step;
		miny = rect.y() - step;
		maxx = rect.x() + rect.width() + step;
		maxy = rect.y() + rect.height() + step;

		/* mult */
		for (a = 0; a < WIDGET_CURVE_RESOLU; a++) {
			vec[a][0] = rad * cornervec[a][0];
			vec[a][1] = rad * cornervec[a][1];
		}

		/* start with left-top, anti clockwise */
		if (roundboxalign & Drawable::RoundBoxTopLeft) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx + rad - vec[a][0];
				vert[tot][1] = maxy - vec[a][1];
			}
		}
		else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = maxy;
			}
		}

		if (roundboxalign & Drawable::RoundBoxBottomLeft) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx + vec[a][1];
				vert[tot][1] = miny + rad - vec[a][0];
			}
		}
		else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = miny;
			}
		}

		if (roundboxalign & Drawable::RoundBoxBottomRight) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx - rad + vec[a][0];
				vert[tot][1] = miny + vec[a][1];
			}
		}
		else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = miny;
			}
		}

		if (roundboxalign & Drawable::RoundBoxTopRight) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx - vec[a][1];
				vert[tot][1] = maxy - rad + vec[a][0];
			}
		}
		else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = maxy;
			}
		}
		return tot;
	}


	Widget::Trias::Trias ()
	: tot(0)
	{

	}

	Widget::Base::Base()
	: totvert(0), halfwayvert(0), inner(true),
	outline(true), emboss(true), shadedir(true)
	{
	}

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

	void Widget::DrawAntiTriangle (float x1,
							   float y1,
							   float x2,
							   float y2,
							   float x3,
							   float y3)
	{
		float tri_arr[3][2] = {{x1, y1}, {x2, y2}, {x3, y3}};
		float color[4];
		int j;
	
		glEnable(GL_BLEND);
		glGetFloatv(GL_CURRENT_COLOR, color);
		color[3] *= 0.125f;
		glColor4fv(color);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, tri_arr);

		/* for each AA step */
		for (j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_BLEND);
	}

	void Widget::DrawAntiRoundbox(int mode, float minx, float miny, float maxx, float maxy, float rad, bool use_alpha)
	{
		float color[4];
		int j;

		glEnable(GL_BLEND);
		glGetFloatv(GL_CURRENT_COLOR, color);
		if (use_alpha) {
			color[3] = 0.5f;
		}
		color[3] *= 0.125f;
		glColor4fv(color);

		for (j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);
			DrawBox(mode, minx, miny, maxx, maxy, rad);
			glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
		}

		glDisable(GL_BLEND);
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

	void Widget::DrawWidgetBaseOutline (const Base* wtb)
	{
		/* + 2 because the last pair is wrapped */
		/*
		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; 
		widget_verts_to_quad_strip(wtb, wtb->totvert, quad_strip);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, quad_strip);
		glDrawArrays(GL_QUAD_STRIP, 0, wtb->totvert * 2 + 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		*/
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
