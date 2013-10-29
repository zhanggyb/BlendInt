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
#include <assert.h>

#include <BlendInt/Widget.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>
#include <BlendInt/Vector.hpp>

#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	const float Widget::cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	const float Widget::jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	Widget::Widget ()
	: AbstractForm(), m_border_width(1.0), m_emboss(true)
	{

	}

	Widget::Widget (AbstractForm* parent)
			: AbstractForm(parent), m_border_width(1.0), m_emboss(true)
	{
		// TODO Auto-generated constructor stub
	}

	Widget::~Widget ()
	{
		// TODO Auto-generated destructor stub
	}

	const Padding& Widget::padding () const
	{
		return m_padding;
	}

	void Widget::set_padding (const Padding& padding)
	{
		Padding new_padding = padding;

		if(update(WidgetPropertyPadding, &new_padding)) m_padding = new_padding;
	}

	void Widget::set_padding (int l, int r, int t, int b)
	{
		Padding new_padding (l, r, t, b);
		if(update(WidgetPropertyPadding, &new_padding)) m_padding = new_padding;
	}


	void Widget::press_key (KeyEvent* event)
	{
	}

	void Widget::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::press_mouse (MouseEvent* event)
	{
	}

	void Widget::release_mouse (MouseEvent* event)
	{
	}

	void Widget::move_mouse (MouseEvent* event)
	{
	}

	void Widget::update ()
	{
		// the basic widget don't use shaded color

		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_vertices(size(), inner_v, outer_v);

		if(m_emboss) {
			if(m_buffer.size() != 3)
				m_buffer.generate(3);
		}	else {
			if(m_buffer.size() != 2)
					m_buffer.generate(2);
		}

		m_buffer.set_index(0);
		m_buffer.set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.bind();
		m_buffer.upload(inner_v);
		m_buffer.unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		m_buffer.set_index(1);
		m_buffer.set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_buffer.bind();
		m_buffer.upload(quad_strip);
		m_buffer.unbind();

		if (m_emboss) {
			float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip_emboss);

			m_buffer.set_index(2);
			m_buffer.set_property(vert_sum.half * 2, sizeof(quad_strip_emboss[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			m_buffer.bind();
			m_buffer.upload(quad_strip_emboss);
			m_buffer.unbind();
		}
	}

	bool Widget::update (int type, const void* property)
	{
		switch(type)
		{
			case FormPropertySize:
				update_shape(static_cast<const Size*>(property));
				break;
			case FormPropertyRoundCorner:
				break;
			default:
				break;
		}

		return true;
	}

	void Widget::render ()
	{
#ifdef DEBUG
		std::set<AbstractForm*>::const_iterator it;
		AbstractForm *item = 0;
		for (it = m_children.begin(); it != m_children.end(); it++) {
			item = *it;
			Interface::instance()->dispatch_render_event(item);
		}
#endif

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// draw inner, simple fill
		glColor4ub(themes()->regular.inner.r(),
		        themes()->regular.inner.g(),
		        themes()->regular.inner.b(),
		        themes()->regular.inner.a());
		m_buffer.set_index(0);
		m_buffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_POLYGON, 0, m_buffer.vertices());
		glDisableClientState(GL_VERTEX_ARRAY);
		m_buffer.unbind();

		// draw outline
		m_buffer.set_index(1);
		unsigned char tcol[4] = { themes()->regular.outline.r(),
		        themes()->regular.outline.g(),
		        themes()->regular.outline.b(),
		        themes()->regular.outline.a()};
		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		m_buffer.bind();
		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		m_buffer.unbind();

		if(m_emboss) {
			m_buffer.set_index(2);
			m_buffer.bind();
			glEnableClientState(GL_VERTEX_ARRAY);
			for (int j = 0; j < WIDGET_AA_JITTER; j++) {
				glTranslatef(jit[j][0], jit[j][1], 0.0f);
				glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
				glVertexPointer(2, GL_FLOAT, 0, 0);
				glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
				glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
			}
			glDisableClientState(GL_VERTEX_ARRAY);
			m_buffer.unbind();
		}

		glDisable(GL_BLEND);
		glPopMatrix();
	}

	void Widget::update_shape(const Size* size)
	{
		// the basic widget don't use shaded color

		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		VerticesSum vert_sum;

		vert_sum = generate_vertices(size, inner_v, outer_v);

		if(m_emboss) {
			if(m_buffer.size() != 3)
				m_buffer.generate(3);
		}	else {
			if(m_buffer.size() != 2)
					m_buffer.generate(2);
		}

		m_buffer.set_index(0);
		m_buffer.set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.bind();
		m_buffer.upload(inner_v);
		m_buffer.unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		m_buffer.set_index(1);
		m_buffer.set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_buffer.bind();
		m_buffer.upload(quad_strip);
		m_buffer.unbind();

		if (m_emboss) {
			float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

			verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip_emboss);

			m_buffer.set_index(2);
			m_buffer.set_property(vert_sum.half * 2, sizeof(quad_strip_emboss[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

			m_buffer.bind();
			m_buffer.upload(quad_strip_emboss);
			m_buffer.unbind();
		}
	}

	bool Widget::contain_no_padding (const Coord2d& cursor)
	{
		if (cursor.x() < (m_pos.x() + m_padding.left()) ||
				cursor.y() < (m_pos.y() + m_padding.bottom()) ||
				cursor.x() > (m_pos.x() + m_size.width() - m_padding.right()) ||
				cursor.y() > (m_pos.y() + m_size.height() - m_padding.top())) {
			return false;
		}

		return true;
	}

	Widget::VerticesSum Widget::generate_vertices(const Size* size, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2])
	{
		float rad = m_corner_radius;
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
		const int hnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerTopRight)) == (RoundCornerTopLeft | RoundCornerTopRight) ||
		                  (m_roundcorner & (RoundCornerBottomRight | RoundCornerBottomLeft)) == (RoundCornerBottomRight | RoundCornerBottomLeft)) ? 1 : 2;
		const int vnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerBottomLeft)) == (RoundCornerTopLeft | RoundCornerBottomLeft) ||
		                  (m_roundcorner & (RoundCornerTopRight | RoundCornerBottomRight)) == (RoundCornerTopRight | RoundCornerBottomRight)) ? 1 : 2;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * m_corner_radius > minsize)
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
		if (m_roundcorner & RoundCornerBottomLeft) {
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
		if (m_roundcorner & RoundCornerBottomRight) {
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
		if (m_roundcorner & RoundCornerTopRight) {
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
		if (m_roundcorner & RoundCornerTopLeft) {
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

#ifdef DEBUG

#endif

		sum.total = count;
		return sum;
	}

	Widget::VerticesSum Widget::generate_vertices(const Size& size, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2])
	{
		float rad = m_corner_radius;
		float radi = rad - m_border_width;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + m_border_width;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - m_border_width; 	// U.pixelsize;
		float minyi = miny + m_border_width;		// U.pixelsize;
		float maxyi = maxy - m_border_width;		// U.pixelsize;

		VerticesSum sum;

		int count = 0;

		int minsize = 0;
		const int hnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerTopRight)) == (RoundCornerTopLeft | RoundCornerTopRight) ||
		                  (m_roundcorner & (RoundCornerBottomRight | RoundCornerBottomLeft)) == (RoundCornerBottomRight | RoundCornerBottomLeft)) ? 1 : 2;
		const int vnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerBottomLeft)) == (RoundCornerTopLeft | RoundCornerBottomLeft) ||
		                  (m_roundcorner & (RoundCornerTopRight | RoundCornerBottomRight)) == (RoundCornerTopRight | RoundCornerBottomRight)) ? 1 : 2;

		minsize = std::min(size.width() * hnum,
		                 size.height() * vnum);

		if (2.0f * m_corner_radius > minsize)
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
		if (m_roundcorner & RoundCornerBottomLeft) {
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
		if (m_roundcorner & RoundCornerBottomRight) {
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
		if (m_roundcorner & RoundCornerTopRight) {
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
		if (m_roundcorner & RoundCornerTopLeft) {
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

#ifdef DEBUG

#endif

		sum.total = count;
		return sum;
	}

	Widget::VerticesSum Widget::generate_vertices (const Size* size,
			const WidgetTheme* theme,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = m_corner_radius;
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
		const int hnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerTopRight)) == (RoundCornerTopLeft | RoundCornerTopRight) ||
		                  (m_roundcorner & (RoundCornerBottomRight | RoundCornerBottomLeft)) == (RoundCornerBottomRight | RoundCornerBottomLeft)) ? 1 : 2;
		const int vnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerBottomLeft)) == (RoundCornerTopLeft | RoundCornerBottomLeft) ||
		                  (m_roundcorner & (RoundCornerTopRight | RoundCornerBottomRight)) == (RoundCornerTopRight | RoundCornerBottomRight)) ? 1 : 2;

		Color color_top = theme->inner + theme->shadetop;
		Color color_down = theme->inner + theme->shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * m_corner_radius > minsize)
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
		if (m_roundcorner & RoundCornerBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + veci[i][1];
				inner[count][1] = minyi + radi - veci[i][0];

				outer[count][0] = minx + vec[i][1];
				outer[count][1] = miny + rad - vec[i][0];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - radi + veci[i][0];
				inner[count][1] = minyi + veci[i][1];

				outer[count][0] = maxx - rad + vec[i][0];
				outer[count][1] = miny + vec[i][1];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - veci[i][1];
				inner[count][1] = maxyi - radi + veci[i][0];

				outer[count][0] = maxx - vec[i][1];
				outer[count][1] = maxy - rad + vec[i][0];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + radi - veci[i][0];
				inner[count][1] = maxyi - veci[i][1];

				outer[count][0] = minx + rad - vec[i][0];
				outer[count][1] = maxy - vec[i][1];

				if (shadedir) {
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

			if (shadedir) {
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

	Widget::VerticesSum Widget::generate_vertices (const Size* size,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = m_corner_radius;
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
		const int hnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerTopRight)) == (RoundCornerTopLeft | RoundCornerTopRight) ||
		                  (m_roundcorner & (RoundCornerBottomRight | RoundCornerBottomLeft)) == (RoundCornerBottomRight | RoundCornerBottomLeft)) ? 1 : 2;
		const int vnum = ((m_roundcorner & (RoundCornerTopLeft | RoundCornerBottomLeft)) == (RoundCornerTopLeft | RoundCornerBottomLeft) ||
		                  (m_roundcorner & (RoundCornerTopRight | RoundCornerBottomRight)) == (RoundCornerTopRight | RoundCornerBottomRight)) ? 1 : 2;

		Color color_top = color + shadetop;
		Color color_down = color + shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * m_corner_radius > minsize)
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
		if (m_roundcorner & RoundCornerBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + veci[i][1];
				inner[count][1] = minyi + radi - veci[i][0];

				outer[count][0] = minx + vec[i][1];
				outer[count][1] = miny + rad - vec[i][0];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - radi + veci[i][0];
				inner[count][1] = minyi + veci[i][1];

				outer[count][0] = maxx - rad + vec[i][0];
				outer[count][1] = miny + vec[i][1];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = maxxi - veci[i][1];
				inner[count][1] = maxyi - radi + veci[i][0];

				outer[count][0] = maxx - vec[i][1];
				outer[count][1] = maxy - rad + vec[i][0];

				if (shadedir) {
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

			if (shadedir) {
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
		if (m_roundcorner & RoundCornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner[count][0] = minxi + radi - veci[i][0];
				inner[count][1] = maxyi - veci[i][1];

				outer[count][0] = minx + rad - vec[i][0];
				outer[count][1] = maxy - vec[i][1];

				if (shadedir) {
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

			if (shadedir) {
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

	void Widget::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][2],
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

	void Widget::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][6],
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

	void Widget::verts_to_quad_strip_open (
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

} /* namespace BlendInt */

