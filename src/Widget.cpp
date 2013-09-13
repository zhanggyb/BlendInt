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
#include <assert.h>
#include <algorithm>

#include <BIL/Widget.hpp>
#include <BIL/Types.hpp>
#include <BIL/Coord.hpp>

#include <BIL/Vector.hpp>

#include <BIL/Utilities-inl.hpp>

#include <iostream>

namespace BIL {

	/* *********************** draw data ************************** */

	static const float cornervec[WIDGET_CURVE_RESOLU][2] = { { 0.0, 0.0 }, {
	        0.195, 0.02 }, { 0.383, 0.067 }, { 0.55, 0.169 }, { 0.707, 0.293 },
	        { 0.831, 0.45 }, { 0.924, 0.617 }, { 0.98, 0.805 }, { 1.0, 1.0 } };

	const int WIDGET_AA_JITTER = 8;

	static const float jit[WIDGET_AA_JITTER][2] = { { 0.468813, -0.481430 }, {
	        -0.155755, -0.352820 }, { 0.219306, -0.238501 }, { -0.393286,
	        -0.110949 }, { -0.024699, 0.013908 }, { 0.343805, 0.147431 }, {
	        -0.272855, 0.269918 }, { 0.095909, 0.388710 } };

	static const float num_tria_vert[3][2] = { { -0.352077, 0.532607 }, {
	        -0.352077, -0.549313 }, { 0.330000, -0.008353 } };

	static const unsigned int num_tria_face[1][3] = { { 0, 1, 2 } };

	static const float scroll_circle_vert[16][2] =
	        { { 0.382684, 0.923879 }, { 0.000001, 1.000000 }, { -0.382683,
	                0.923880 }, { -0.707107, 0.707107 },
	                { -0.923879, 0.382684 }, { -1.000000, 0.000000 }, {
	                        -0.923880, -0.382684 }, { -0.707107, -0.707107 }, {
	                        -0.382683, -0.923880 }, { 0.000000, -1.000000 }, {
	                        0.382684, -0.923880 }, { 0.707107, -0.707107 }, {
	                        0.923880, -0.382684 }, { 1.000000, -0.000000 }, {
	                        0.923880, 0.382683 }, { 0.707107, 0.707107 } };

	static const unsigned int scroll_circle_face[14][3] = { { 0, 1, 2 }, { 2, 0,
	        3 }, { 3, 0, 15 }, { 3, 15, 4 }, { 4, 15, 14 }, { 4, 14, 5 }, { 5,
	        14, 13 }, { 5, 13, 6 }, { 6, 13, 12 }, { 6, 12, 7 }, { 7, 12, 11 },
	        { 7, 11, 8 }, { 8, 11, 10 }, { 8, 10, 9 } };

	static const float menu_tria_vert[6][2] = { { -0.33, 0.16 }, { 0.33, 0.16 },
	        { 0, 0.82 }, { 0, -0.82 }, { -0.33, -0.16 }, { 0.33, -0.16 } };

	static const unsigned int menu_tria_face[2][3] =
	        { { 2, 0, 1 }, { 3, 5, 4 } };

	static const float check_tria_vert[6][2] = { { -0.578579, 0.253369 }, {
	        -0.392773, 0.412794 }, { -0.004241, -0.328551 }, { -0.003001,
	        0.034320 }, { 1.055313, 0.864744 }, { 0.866408, 1.026895 } };

	static const unsigned int check_tria_face[4][3] = { { 3, 2, 4 },
	        { 3, 4, 5 }, { 1, 0, 3 }, { 0, 2, 3 } };

	GLubyte const checker_stipple_sml[32 * 32 / 8] = { 255, 0, 255, 0, 255, 0,
	        255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
	        0, 255, 0, 255, 0, 255, 0, 255, 0, 0, 255, 0, 255, 0, 255, 0, 255,
	        0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
	        255, 0, 255, 0, 255, 0, 255, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
	        255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
	        0, 255, 0, 255, 0, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255,
	        0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0,
	        255, 0, 255, };

	DecorationVertexes::DecorationVertexes ()
			: tot(0)
	{

	}

	WidgetVertexes::WidgetVertexes ()
			: totvert(0), halfwayvert(0), inner(true), outline(true), emboss(
			        true), shadedir(true)
	{
	}

	Widget::Widget (Traceable *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub

		// Set the default padding and margin
		set_padding(Padding(5, 5, 5, 5));
		set_margin(1, 1, 1, 1);
	}

	Widget::~Widget ()
	{
		// TODO Auto-generated destructor stub
	}

	void Widget::DrawAntiTriangle (float x1, float y1, float x2, float y2,
	        float x3, float y3)
	{
		float tri_arr[3][2] = { { x1, y1 }, { x2, y2 }, { x3, y3 } };
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

	void Widget::DrawAntiRoundbox (int mode, float minx, float miny, float maxx,
	        float maxy, float rad, bool use_alpha)
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

	void Widget::DrawTrias (const DecorationVertexes *tria)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, tria->vec);
		glDrawElements(GL_TRIANGLES, tria->tot * 3,
		GL_UNSIGNED_INT, tria->index);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	/**
	 * @brief Draw widget appearance
	 * @param vertexes
	 *
	 * @note widgetbase_draw() in interface_widgets.c
	 */
	void Widget::DrawAppearance (WidgetVertexes* vertexes)
	{
		Theme *theme = Theme::instance();
		if (theme == NULL)
			return;

		const WidgetColors* wcol = &(theme->themeUI()->wcol_regular);

		int j, a;

		glEnable(GL_BLEND);

		/* backdrop non AA */
		if (vertexes->inner) {
			if (wcol->shaded == 0) {
				if (wcol->alpha_check) {
					float inner_v_half[WIDGET_SIZE_MAX][2];
					float x_mid = 0.0f; /* used for dumb clamping of values */

					/* dark checkers */
					glColor4ub(UI_TRANSP_DARK, UI_TRANSP_DARK, UI_TRANSP_DARK,
					        255);
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, vertexes->inner_v);
					glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					/* light checkers */
					glEnable(GL_POLYGON_STIPPLE);
					glColor4ub(UI_TRANSP_LIGHT, UI_TRANSP_LIGHT,
					        UI_TRANSP_LIGHT, 255);
					glPolygonStipple(checker_stipple_sml);

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, vertexes->inner_v);
					glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					glDisable(GL_POLYGON_STIPPLE);

					/* alpha fill */
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glColor4ub(wcol->inner.r(), wcol->inner.g(),
					        wcol->inner.b(), wcol->inner.a());
					glEnableClientState(GL_VERTEX_ARRAY);

					for (a = 0; a < vertexes->totvert; a++) {
						x_mid += vertexes->inner_v[a][0];
					}
					x_mid /= vertexes->totvert;

					glVertexPointer(2, GL_FLOAT, 0, vertexes->inner_v);
					glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);

					/* 1/2 solid color */
					glColor4ub(wcol->inner.r(), wcol->inner.g(),
					        wcol->inner.b(), 255);

					for (a = 0; a < vertexes->totvert; a++) {
						inner_v_half[a][0] = std::min(vertexes->inner_v[a][0],
						        x_mid);
						inner_v_half[a][1] = vertexes->inner_v[a][1];
					}

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, inner_v_half);
					glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);
				} else {
					/* simple fill */
					glColor4ub(wcol->inner.r(), wcol->inner.g(),
					        wcol->inner.b(), wcol->inner.a());

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(2, GL_FLOAT, 0, vertexes->inner_v);
					glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
					glDisableClientState(GL_VERTEX_ARRAY);
				}
			} else {
				//char col1[4], col2[4];
				Color col1, col2;
				unsigned char col_array[WIDGET_SIZE_MAX * 4];
				unsigned char *col_pt = col_array;

				Color::convert_shade_color(wcol->inner, wcol->shadetop,
				        wcol->shadedown, &col1, &col2);

				glShadeModel(GL_SMOOTH);
				for (a = 0; a < vertexes->totvert; a++, col_pt += 4) {
					Color::ConvertRoundBoxShadeColor(col1, col2,
					        vertexes->inner_uv[a][vertexes->shadedir], col_pt);
				}

				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glVertexPointer(2, GL_FLOAT, 0, vertexes->inner_v);
				glColorPointer(4, GL_UNSIGNED_BYTE, 0, col_array);
				glDrawArrays(GL_POLYGON, 0, vertexes->totvert);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);

				glShadeModel(GL_FLAT);
			}
		}

		/* for each AA step */
		if (vertexes->outline) {
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
			float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

			const unsigned char tcol[4] = { wcol->outline[0], wcol->outline[1],
			        wcol->outline[2], wcol->outline[3] / WIDGET_AA_JITTER };

			verts_to_quad_strip(vertexes->totvert, quad_strip, vertexes);

			if (vertexes->emboss) {
				verts_to_quad_strip_open(vertexes->halfwayvert,
				        quad_strip_emboss, vertexes);
			}

			glEnableClientState(GL_VERTEX_ARRAY);

			for (j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);

				/* outline */
				glColor4ubv(tcol);

				glVertexPointer(2, GL_FLOAT, 0, quad_strip);
				glDrawArrays(GL_QUAD_STRIP, 0, vertexes->totvert * 2 + 2);

				/* emboss bottom shadow */
				if (vertexes->emboss) {
					glColor4f(1.0f, 1.0f, 1.0f, 0.02f);

					glVertexPointer(2, GL_FLOAT, 0, quad_strip_emboss);
					glDrawArrays(GL_QUAD_STRIP, 0, vertexes->halfwayvert * 2);
				}

				glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
			}

			glDisableClientState(GL_VERTEX_ARRAY);
		}

		/* decoration */
		if (vertexes->tria1.tot || vertexes->tria2.tot) {
			const unsigned char tcol[4] = { wcol->item[0], wcol->item[1],
			        wcol->item[2], (unsigned char) ((float) wcol->item[3]
			                / WIDGET_AA_JITTER) };
			/* for each AA step */
			for (j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(1.0f * jit[j][0], 1.0f * jit[j][1], 0.0f);

				if (vertexes->tria1.tot) {
					glColor4ubv(tcol);
					DrawTrias(&vertexes->tria1);
				}
				if (vertexes->tria2.tot) {
					glColor4ubv(tcol);
					DrawTrias(&vertexes->tria2);
				}

				glTranslatef(-1.0f * jit[j][0], -1.0f * jit[j][1], 0.0f);
			}
		}

		glDisable(GL_BLEND);
	}

	void Widget::DrawWidgetBaseOutline (WidgetVertexes* wtb)
	{
		/* + 2 because the last pair is wrapped */
		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2];
		verts_to_quad_strip(wtb->totvert, quad_strip, wtb);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, quad_strip);
		glDrawArrays(GL_QUAD_STRIP, 0, wtb->totvert * 2 + 2);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Widget::keyPressEvent (KeyEvent* event)
	{
		event->accept();
	}

	void Widget::contextMenuPressEvent (ContextMenuEvent* event)
	{
		event->accept();
	}

	void Widget::contextMenuReleaseEvent (ContextMenuEvent* event)
	{
		event->accept();
	}

	void Widget::mousePressEvent (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::mouseReleaseEvent (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::mouseMoveEvent (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::update ()
	{
		if (!size_.is_valid())
			return;

		float rad;

		/* half rounded */
		// TODO: define widget_unit by user
		//rad = 0.2f * U.widget_unit;
		rad = 0.2f * 20;

		//round_box_edges(&wtb, roundboxalign, rect, rad);
		CalculateRoundBoxEdges(round_box_type_, Rect(0, 0, size_.width(), size_.height()), rad,
		        &m_appearance);
	}

	void Widget::render ()
	{
		DrawAppearance(&m_appearance);
	}

	/* helper call, makes shadow rect, with 'sun' above menu, so only shadow to left/right/bottom */
	/* return tot */
	int Widget::CalculateRoundBoxShadowEdges (float (*vert)[2],
	        const Rect& rect, float rad, int roundboxalign, float step)
	{
		if (!rect.IsValid())
			return 0;

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
		if (roundboxalign & RoundBoxTopLeft) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx + rad - vec[a][0];
				vert[tot][1] = maxy - vec[a][1];
			}
		} else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = maxy;
			}
		}

		if (roundboxalign & RoundBoxBottomLeft) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx + vec[a][1];
				vert[tot][1] = miny + rad - vec[a][0];
			}
		} else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = minx;
				vert[tot][1] = miny;
			}
		}

		if (roundboxalign & RoundBoxBottomRight) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx - rad + vec[a][0];
				vert[tot][1] = miny + vec[a][1];
			}
		} else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = miny;
			}
		}

		if (roundboxalign & RoundBoxTopRight) {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx - vec[a][1];
				vert[tot][1] = maxy - rad + vec[a][0];
			}
		} else {
			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				vert[tot][0] = maxx;
				vert[tot][1] = maxy;
			}
		}
		return tot;
	}

	void Widget::CalculateRoundBoxEdges (int roundboxalign, const Rect& rect,
	        float rad, float radi, WidgetVertexes* wt)
	{
		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];
		float minx = rect.x(), miny = rect.y(), maxx = rect.x() + rect.width(),
		        maxy = rect.y() + rect.height();

		// TODO pixelsize should be defined by user
		float pixelsize = 1.0;

		float minxi = minx + pixelsize; /* boundbox inner */
		float maxxi = maxx - pixelsize;
		float minyi = miny + pixelsize;
		float maxyi = maxy - pixelsize;
		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f; /* for uv, can divide by zero */
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;
		int a, tot = 0, minsize;
		const int hnum =
		        ((roundboxalign & (RoundBoxTopLeft | RoundBoxTopRight))
		                == (RoundBoxTopLeft | RoundBoxTopRight)
		                || (roundboxalign
		                        & (RoundBoxBottomRight | RoundBoxBottomLeft))
		                        == (RoundBoxBottomRight | RoundBoxBottomLeft)) ?
		                1 : 2;
		const int vnum =
		        ((roundboxalign & (RoundBoxTopLeft | RoundBoxBottomLeft))
		                == (RoundBoxTopLeft | RoundBoxBottomLeft)
		                || (roundboxalign
		                        & (RoundBoxTopRight | RoundBoxBottomRight))
		                        == (RoundBoxTopRight | RoundBoxBottomRight)) ?
		                1 : 2;

		minsize = std::min(rect.width() * hnum, rect.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - pixelsize;

		/* mult */
		for (a = 0; a < WIDGET_CURVE_RESOLU; a++) {
			veci[a][0] = radi * cornervec[a][0];
			veci[a][1] = radi * cornervec[a][1];
			vec[a][0] = rad * cornervec[a][0];
			vec[a][1] = rad * cornervec[a][1];
		}

		/* corner left-bottom */
		if (roundboxalign & RoundBoxBottomLeft) {

			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				wt->inner_v[tot][0] = minxi + veci[a][1];
				wt->inner_v[tot][1] = minyi + radi - veci[a][0];

				wt->outer_v[tot][0] = minx + vec[a][1];
				wt->outer_v[tot][1] = miny + rad - vec[a][0];

				wt->inner_uv[tot][0] = facxi * (wt->inner_v[tot][0] - minxi);
				wt->inner_uv[tot][1] = facyi * (wt->inner_v[tot][1] - minyi);
			}
		} else {
			wt->inner_v[tot][0] = minxi;
			wt->inner_v[tot][1] = minyi;

			wt->outer_v[tot][0] = minx;
			wt->outer_v[tot][1] = miny;

			wt->inner_uv[tot][0] = 0.0f;
			wt->inner_uv[tot][1] = 0.0f;

			tot++;
		}

		/* corner right-bottom */
		if (roundboxalign & RoundBoxBottomRight) {

			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				wt->inner_v[tot][0] = maxxi - radi + veci[a][0];
				wt->inner_v[tot][1] = minyi + veci[a][1];

				wt->outer_v[tot][0] = maxx - rad + vec[a][0];
				wt->outer_v[tot][1] = miny + vec[a][1];

				wt->inner_uv[tot][0] = facxi * (wt->inner_v[tot][0] - minxi);
				wt->inner_uv[tot][1] = facyi * (wt->inner_v[tot][1] - minyi);
			}
		} else {
			wt->inner_v[tot][0] = maxxi;
			wt->inner_v[tot][1] = minyi;

			wt->outer_v[tot][0] = maxx;
			wt->outer_v[tot][1] = miny;

			wt->inner_uv[tot][0] = 1.0f;
			wt->inner_uv[tot][1] = 0.0f;

			tot++;
		}

		wt->halfwayvert = tot;

		/* corner right-top */
		if (roundboxalign & RoundBoxTopRight) {

			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				wt->inner_v[tot][0] = maxxi - veci[a][1];
				wt->inner_v[tot][1] = maxyi - radi + veci[a][0];

				wt->outer_v[tot][0] = maxx - vec[a][1];
				wt->outer_v[tot][1] = maxy - rad + vec[a][0];

				wt->inner_uv[tot][0] = facxi * (wt->inner_v[tot][0] - minxi);
				wt->inner_uv[tot][1] = facyi * (wt->inner_v[tot][1] - minyi);
			}
		} else {
			wt->inner_v[tot][0] = maxxi;
			wt->inner_v[tot][1] = maxyi;

			wt->outer_v[tot][0] = maxx;
			wt->outer_v[tot][1] = maxy;

			wt->inner_uv[tot][0] = 1.0f;
			wt->inner_uv[tot][1] = 1.0f;

			tot++;
		}

		/* corner left-top */
		if (roundboxalign & RoundBoxTopLeft) {

			for (a = 0; a < WIDGET_CURVE_RESOLU; a++, tot++) {
				wt->inner_v[tot][0] = minxi + radi - veci[a][0];
				wt->inner_v[tot][1] = maxyi - veci[a][1];

				wt->outer_v[tot][0] = minx + rad - vec[a][0];
				wt->outer_v[tot][1] = maxy - vec[a][1];

				wt->inner_uv[tot][0] = facxi * (wt->inner_v[tot][0] - minxi);
				wt->inner_uv[tot][1] = facyi * (wt->inner_v[tot][1] - minyi);
			}

		} else {

			wt->inner_v[tot][0] = minxi;
			wt->inner_v[tot][1] = maxyi;

			wt->outer_v[tot][0] = minx;
			wt->outer_v[tot][1] = maxy;

			wt->inner_uv[tot][0] = 0.0f;
			wt->inner_uv[tot][1] = 1.0f;

			tot++;
		}

		assert(tot <= WIDGET_SIZE_MAX);

		wt->totvert = tot;
	}

	void Widget::CalculateRoundBoxEdges (int roundboxalign, const Rect& rect,
	        float rad, WidgetVertexes *wt)
	{
		// TODO: pixelsize should be defined by user
		float pixelsize = 1.0;
		CalculateRoundBoxEdges(roundboxalign, rect, rad, rad - pixelsize, wt);
	}

	void Widget::CalculateTriangleNumbers (const Rect& rect, float triasize,
	        char where, DecorationVertexes *tria)
	{
		float centx, centy, sizex, sizey, minsize;
		int a, i1 = 0, i2 = 1;

		minsize = std::min(rect.width(), rect.height());

		/* center position and size */
		centx = (float) rect.x() + 0.5f * minsize;
		centy = (float) rect.y() + 0.5f * minsize;
		sizex = sizey = -0.5f * triasize * minsize;

		if (where == 'r') {
			centx = (float) (rect.x() + rect.width()) - 0.5f * minsize;
			sizex = -sizex;
		} else if (where == 't') {
			centy = (float) (rect.y() + rect.height()) - 0.5f * minsize;
			sizey = -sizey;
			i2 = 0;
			i1 = 1;
		} else if (where == 'b') {
			sizex = -sizex;
			i2 = 0;
			i1 = 1;
		}

		for (a = 0; a < 3; a++) {
			tria->vec[a][0] = sizex * num_tria_vert[a][i1] + centx;
			tria->vec[a][1] = sizey * num_tria_vert[a][i2] + centy;
		}

		tria->tot = 1;
		tria->index = num_tria_face;
	}

	void Widget::CalculateScrollCircle (const Rect& rect, float triasize,
	        char where, DecorationVertexes *tria)
	{
		float centx, centy, sizex, sizey, minsize;
		int a, i1 = 0, i2 = 1;

		minsize = std::min(rect.width(), rect.height());

		/* center position and size */
		centx = (float) rect.x() + 0.5f * minsize;
		centy = (float) rect.y() + 0.5f * minsize;
		sizex = sizey = -0.5f * triasize * minsize;

		if (where == 'r') {
			centx = (float) (rect.x() + rect.width()) - 0.5f * minsize;
			sizex = -sizex;
		} else if (where == 't') {
			centy = (float) (rect.y() + rect.height()) - 0.5f * minsize;
			sizey = -sizey;
			i2 = 0;
			i1 = 1;
		} else if (where == 'b') {
			sizex = -sizex;
			i2 = 0;
			i1 = 1;
		}

		for (a = 0; a < 16; a++) {
			tria->vec[a][0] = sizex * scroll_circle_vert[a][i1] + centx;
			tria->vec[a][1] = sizey * scroll_circle_vert[a][i2] + centy;
		}

		tria->tot = 14;
		tria->index = scroll_circle_face;
	}

	void Widget::CalculateMenuTriangle (const Rect& rect,
	        DecorationVertexes *tria)
	{
		float centx, centy, size;
		int a;

		/* center position and size */
		centx = rect.x() + rect.width() - 0.32f * rect.height();
		centy = rect.y() + rect.height() + 0.50f * rect.height();
		size = 0.4f * (float) rect.height();

		for (a = 0; a < 6; a++) {
			tria->vec[a][0] = size * menu_tria_vert[a][0] + centx;
			tria->vec[a][1] = size * menu_tria_vert[a][1] + centy;
		}

		tria->tot = 2;
		tria->index = menu_tria_face;
	}

	void Widget::CalculateCheckTriangle (const Rect& rect,
	        DecorationVertexes *tria)
	{
		float centx, centy, size;
		int a;

		/* center position and size */
		centx = rect.x() + rect.width() + 0.5f * rect.height();
		centy = rect.y() + 0.5f * rect.height();
		size = 0.5f * rect.height();

		for (a = 0; a < 6; a++) {
			tria->vec[a][0] = size * check_tria_vert[a][0] + centx;
			tria->vec[a][1] = size * check_tria_vert[a][1] + centy;
		}

		tria->tot = 4;
		tria->index = check_tria_face;
	}

	void Widget::verts_to_quad_strip (const int totvert,
	        float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], WidgetVertexes *wtb)
	{
		int a;
		for (a = 0; a < totvert; a++) {
			copy_v2_v2(quad_strip[a * 2], wtb->outer_v[a]);
			copy_v2_v2(quad_strip[a * 2 + 1], wtb->inner_v[a]);
		}
		copy_v2_v2(quad_strip[a * 2], wtb->outer_v[0]);
		copy_v2_v2(quad_strip[a * 2 + 1], wtb->inner_v[0]);
	}

	void Widget::verts_to_quad_strip_open (const int totvert,
	        float quad_strip[WIDGET_SIZE_MAX * 2][2], WidgetVertexes *wtb)
	{
		int a;
		for (a = 0; a < totvert; a++) {
			quad_strip[a * 2][0] = wtb->outer_v[a][0];
			quad_strip[a * 2][1] = wtb->outer_v[a][1];
			quad_strip[a * 2 + 1][0] = wtb->outer_v[a][0];
			quad_strip[a * 2 + 1][1] = wtb->outer_v[a][1] - 1.0f;
		}
	}

} /* namespace BIL */

