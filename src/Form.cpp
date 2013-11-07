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
#include <assert.h>
#include <algorithm>

#include <iostream>

#include <BlendInt/Form.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>

#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	const float Form::cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	const float Form::jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	Form::Form()
	: AbstractForm(),
	  m_round_type(CornerNone),
	  m_radius(5.0),
	  m_border_width(1.0),
	  m_emboss(true)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Form::Form(AbstractForm* parent)
	: AbstractForm(parent),
	  m_round_type(CornerNone),
	  m_radius(5.0),
	  m_border_width(1.0),
	  m_emboss(true)
	{
		set_minimal_size(0, 0);
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Form::~Form()
	{

	}

	void Form::set_round_type(int type)
	{
		if (m_round_type == type) return;

		update (FormPropertyCorner, &type);

		m_round_type = type;

		fire_property_changed_event(FormPropertyCorner);
	}

	void Form::set_radius(float radius)
	{
		if (m_radius == radius) return;

		update (FormPropertyCorner, &radius);

		m_radius = radius;

		fire_property_changed_event(FormPropertyCorner);
	}

	void Form::set_emboss(bool emboss)
	{
		m_emboss = emboss;

		// TODO: call update
	}

	void Form::set_border_width(float width)
	{
		if(width > 0.0)
			m_border_width = width;
		else
			return;

		// TODO: call update
	}


	void Form::update (int type, const void* data)
	{
		switch (type) {

			case FormPropertySize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			default:
				break;
		}
	}

	void Form::render()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());
		draw_gl_buffer(0);

		// draw outline
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_gl_buffer_anti_alias(1);

		if(m_emboss) {
			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			draw_gl_buffer_anti_alias(2);
		}

		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void Form::press_key (KeyEvent* event)
	{
	}

	void Form::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Form::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Form::press_mouse (MouseEvent* event)
	{
	}

	void Form::release_mouse (MouseEvent* event)
	{
	}

	void Form::move_mouse (MouseEvent* event)
	{
	}

	void Form::draw_gl_buffer(size_t index, int mode)
	{
		m_glbuffer.set_index(index);
		m_glbuffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(mode, 0, m_glbuffer.vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		m_glbuffer.unbind();
	}

	void Form::draw_shaded_gl_buffer(size_t index, int mode)
	{
		m_glbuffer.set_index(index);

		m_glbuffer.bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(mode, 0, glbuffer().vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		m_glbuffer.unbind();
	}

	void Form::draw_gl_buffer_anti_alias(size_t index, int mode)
	{
		m_glbuffer.set_index(index);
		m_glbuffer.bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(mode, 0, m_glbuffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		m_glbuffer.unbind();
	}

	Form::VerticesSum Form::generate_vertices(const Size* size, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2])
	{
		float rad = radius();
		float radi = rad - m_border_width;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + m_border_width;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - m_border_width; 	// U.pixelsize;
		float minyi = miny + m_border_width;		// U.pixelsize;
		float maxyi = maxy - m_border_width;		// U.pixelsize;

		VerticesSum sum;

		int count = 0;

		int minsize = 0;
		const int hnum = ((round_type() & (CornerTopLeft | CornerTopRight)) == (CornerTopLeft | CornerTopRight) ||
		                  (round_type() & (CornerBottomRight | CornerBottomLeft)) == (CornerBottomRight | CornerBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type() & (CornerTopLeft | CornerBottomLeft)) == (CornerTopLeft | CornerBottomLeft) ||
		                  (round_type() & (CornerTopRight | CornerBottomRight)) == (CornerTopRight | CornerBottomRight)) ? 1 : 2;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius() > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - m_border_width;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type() & CornerBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = minxi + veci[i][1];
				inner_v[count][1] = minyi + radi - veci[i][0];

				outer_v[count][0] = minx + vec[i][1];
				outer_v[count][1] = miny + rad - vec[i][0];
			}
		}
		else {
			inner_v[count][0] = minxi;
			inner_v[count][1] = minyi;

			outer_v[count][0] = minx;
			outer_v[count][1] = miny;
			count++;
		}

		// corner right-bottom
		if (round_type() & CornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = maxxi - radi + veci[i][0];
				inner_v[count][1] = minyi + veci[i][1];

				outer_v[count][0] = maxx - rad + vec[i][0];
				outer_v[count][1] = miny + vec[i][1];
			}
		}
		else {
			inner_v[count][0] = maxxi;
			inner_v[count][1] = minyi;

			outer_v[count][0] = maxx;
			outer_v[count][1] = miny;
			count++;
		}

		sum.half = count;

		// corner right-top
		if (round_type() & CornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = maxxi - veci[i][1];
				inner_v[count][1] = maxyi - radi + veci[i][0];

				outer_v[count][0] = maxx - vec[i][1];
				outer_v[count][1] = maxy - rad + vec[i][0];
			}
		}
		else {
			inner_v[count][0] = maxxi;
			inner_v[count][1] = maxyi;

			outer_v[count][0] = maxx;
			outer_v[count][1] = maxy;
			count++;
		}

		// corner left-top
		if (round_type() & CornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = minxi + radi - veci[i][0];
				inner_v[count][1] = maxyi - veci[i][1];

				outer_v[count][0] = minx + rad - vec[i][0];
				outer_v[count][1] = maxy - vec[i][1];
			}

		}
		else {

			inner_v[count][0] = minxi;
			inner_v[count][1] = maxyi;

			outer_v[count][0] = minx;
			outer_v[count][1] = maxy;
			count++;
		}

		assert(count <= WIDGET_SIZE_MAX);

		sum.total = count;
		return sum;
	}

	Form::VerticesSum Form::generate_vertices (const Size* size,
			const WidgetTheme* theme,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = radius();
		float radi = rad - m_border_width;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + m_border_width;
		float maxxi = maxx - m_border_width;
		float minyi = miny + m_border_width;
		float maxyi = maxy - m_border_width;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		VerticesSum sum;
		int count = 0;
		int minsize = 0;
		const int hnum = ((round_type() & (CornerTopLeft | CornerTopRight)) == (CornerTopLeft | CornerTopRight) ||
		                  (round_type() & (CornerBottomRight | CornerBottomLeft)) == (CornerBottomRight | CornerBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type() & (CornerTopLeft | CornerBottomLeft)) == (CornerTopLeft | CornerBottomLeft) ||
		                  (round_type() & (CornerTopRight | CornerBottomRight)) == (CornerTopRight | CornerBottomRight)) ? 1 : 2;

		Color color_top = theme->inner + theme->shadetop;
		Color color_down = theme->inner + theme->shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius() > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - m_border_width;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type() & CornerBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + veci[i][1];
				inner[count][1] = minyi + radi - veci[i][0];

				outer[count][0] = minx + vec[i][1];
				outer[count][1] = miny + rad - vec[i][0];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {
			inner[count][0] = minxi;
			inner[count][1] = minyi;

			outer[count][0] = minx;
			outer[count][1] = miny;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		// corner right-bottom
		if (round_type() & CornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - radi + veci[i][0];
				inner[count][1] = minyi + veci[i][1];

				outer[count][0] = maxx - rad + vec[i][0];
				outer[count][1] = miny + vec[i][1];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {
			inner[count][0] = maxxi;
			inner[count][1] = minyi;

			outer[count][0] = maxx;
			outer[count][1] = miny;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		sum.half = count;

		// corner right-top
		if (round_type() & CornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - veci[i][1];
				inner[count][1] = maxyi - radi + veci[i][0];

				outer[count][0] = maxx - vec[i][1];
				outer[count][1] = maxy - rad + vec[i][0];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;

			}
		}
		else {
			inner[count][0] = maxxi;
			inner[count][1] = maxyi;

			outer[count][0] = maxx;
			outer[count][1] = maxy;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		// corner left-top
		if (round_type() & CornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + radi - veci[i][0];
				inner[count][1] = maxyi - veci[i][1];

				outer[count][0] = minx + rad - vec[i][0];
				outer[count][1] = maxy - vec[i][1];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {

			inner[count][0] = minxi;
			inner[count][1] = maxyi;

			outer[count][0] = minx;
			outer[count][1] = maxy;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		assert(count <= WIDGET_SIZE_MAX);

		sum.total = count;
		return sum;
	}

	Form::VerticesSum Form::generate_vertices (const Size* size,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = radius();
		float radi = rad - m_border_width;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + m_border_width;
		float maxxi = maxx - m_border_width;
		float minyi = miny + m_border_width;
		float maxyi = maxy - m_border_width;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		VerticesSum sum;
		int count = 0;
		int minsize = 0;
		const int hnum = ((round_type() & (CornerTopLeft | CornerTopRight)) == (CornerTopLeft | CornerTopRight) ||
		                  (round_type() & (CornerBottomRight | CornerBottomLeft)) == (CornerBottomRight | CornerBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type() & (CornerTopLeft | CornerBottomLeft)) == (CornerTopLeft | CornerBottomLeft) ||
		                  (round_type() & (CornerTopRight | CornerBottomRight)) == (CornerTopRight | CornerBottomRight)) ? 1 : 2;

		Color color_top = color + shadetop;
		Color color_down = color + shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius() > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - m_border_width;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type() & CornerBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + veci[i][1];
				inner[count][1] = minyi + radi - veci[i][0];

				outer[count][0] = minx + vec[i][1];
				outer[count][1] = miny + rad - vec[i][0];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {
			inner[count][0] = minxi;
			inner[count][1] = minyi;

			outer[count][0] = minx;
			outer[count][1] = miny;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		// corner right-bottom
		if (round_type() & CornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - radi + veci[i][0];
				inner[count][1] = minyi + veci[i][1];

				outer[count][0] = maxx - rad + vec[i][0];
				outer[count][1] = miny + vec[i][1];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {
			inner[count][0] = maxxi;
			inner[count][1] = minyi;

			outer[count][0] = maxx;
			outer[count][1] = miny;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		sum.half = count;

		// corner right-top
		if (round_type() & CornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - veci[i][1];
				inner[count][1] = maxyi - radi + veci[i][0];

				outer[count][0] = maxx - vec[i][1];
				outer[count][1] = maxy - rad + vec[i][0];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {
			inner[count][0] = maxxi;
			inner[count][1] = maxyi;

			outer[count][0] = maxx;
			outer[count][1] = maxy;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		// corner left-top
		if (round_type() & CornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + radi - veci[i][0];
				inner[count][1] = maxyi - veci[i][1];

				outer[count][0] = minx + rad - vec[i][0];
				outer[count][1] = maxy - vec[i][1];

				if (shadedir == Vertical) {
					shaded_color = make_shade_color(color_top, color_down, facyi * (inner[count][1] - minyi));
				} else {
					shaded_color = make_shade_color(color_top, color_down, facxi * (inner[count][0] - minxi));
				}
				inner[count][2] = shaded_color[0] / 255.0;
				inner[count][3] = shaded_color[1] / 255.0;
				inner[count][4] = shaded_color[2] / 255.0;
				inner[count][5] = shaded_color[3] / 255.0;
			}
		}
		else {

			inner[count][0] = minxi;
			inner[count][1] = maxyi;

			outer[count][0] = minx;
			outer[count][1] = maxy;

			if (shadedir == Vertical) {
				shaded_color = make_shade_color(color_top, color_down, 1.0f);
			} else {
				shaded_color = make_shade_color(color_top, color_down, 0.0f);
			}
			inner[count][2] = shaded_color[0] / 255.0;
			inner[count][3] = shaded_color[1] / 255.0;
			inner[count][4] = shaded_color[2] / 255.0;
			inner[count][5] = shaded_color[3] / 255.0;

			count++;
		}

		assert(count <= WIDGET_SIZE_MAX);

		sum.total = count;
		return sum;
	}

	int Form::generate_shadow_vertices (
			const Size* size,
			float rad,
			float step,
			float vert[WIDGET_SIZE_MAX][2])
	{
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
		if (round_type() & CornerTopLeft) {
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

		if (round_type() & CornerBottomLeft) {
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

		if (round_type() & CornerBottomRight) {
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

		if (round_type() & CornerTopRight) {
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

		return tot;
	}

	void Form::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][2],
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int i;
		for (i = 0; i < totvert; i++) {
			copy_v2_v2(quad_strip[i * 2], outer_v[i]);
			copy_v2_v2(quad_strip[i * 2 + 1], inner_v[i]);
		}
		copy_v2_v2(quad_strip[i * 2], outer_v[0]);
		copy_v2_v2(quad_strip[i * 2 + 1], inner_v[0]);
	}

	void Form::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][6],
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int i;
		for (i = 0; i < totvert; i++) {
			copy_v2_v2(quad_strip[i * 2], outer_v[i]);
			copy_v2_v2(quad_strip[i * 2 + 1], inner_v[i]);
		}
		copy_v2_v2(quad_strip[i * 2], outer_v[0]);
		copy_v2_v2(quad_strip[i * 2 + 1], inner_v[0]);
	}

	void Form::verts_to_quad_strip_open (
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		for (int i = 0; i < totvert; i++) {
			quad_strip[i * 2][0] = outer_v[i][0];
			quad_strip[i * 2][1] = outer_v[i][1];
			quad_strip[i * 2 + 1][0] = outer_v[i][0];
			quad_strip[i * 2 + 1][1] = outer_v[i][1] - 1.0f;
		}
	}

	void Form::update_shape(const Size* size)
	{
		// the basic widget don't use shaded color

		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_vertices(size, inner_v, outer_v);

		if(m_emboss) {
			if(m_glbuffer.size() != 3)
				m_glbuffer.generate(3);
		}	else {
			if(m_glbuffer.size() != 2)
					m_glbuffer.generate(2);
		}

		m_glbuffer.set_index(0);
		m_glbuffer.set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_glbuffer.bind();
		m_glbuffer.upload(inner_v);
		m_glbuffer.unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		m_glbuffer.set_index(1);
		m_glbuffer.set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_glbuffer.bind();
		m_glbuffer.upload(quad_strip);
		m_glbuffer.unbind();

		if (m_emboss) {
			//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip);

			m_glbuffer.set_index(2);
			m_glbuffer.set_property(vert_sum.half * 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			m_glbuffer.bind();
			m_glbuffer.upload(quad_strip);
			m_glbuffer.unbind();
		}
	}

}
