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

#include <GL/glew.h>
#include <assert.h>
#include <algorithm>

#include <iostream>
#include <assert.h>

#include <BILO/Widget.hpp>
#include <BILO/Types.hpp>
#include <BILO/Coord.hpp>

#include <BILO/Vector.hpp>

#include <BILO/Utilities-inl.hpp>

#include <BILO/Interface.hpp>

namespace BILO {

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
	: Drawable(), m_border_width(1.0)
	{

	}

	Widget::Widget (Drawable* parent)
			: Drawable(parent), m_border_width(1.0)
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

	bool Widget::update (int type, const void* property)
	{
		switch(type)
		{
			case WidgetPropertySize:
				update_shape(static_cast<const Size*>(property));
				break;
			case WidgetPropertyRoundCorner:
				break;
			default:
				break;
		}

		return true;
	}

	void Widget::render ()
	{
#ifdef DEBUG
		std::set<Drawable*>::const_iterator it;
		Drawable *item = 0;
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

		Theme* tm = Theme::instance();

		// draw inner, simple fill
		glColor4ub(tm->themeUI()->regular.inner.r(),
		        tm->themeUI()->regular.inner.g(),
		        tm->themeUI()->regular.inner.b(),
		        tm->themeUI()->regular.inner.a());

		m_buffer.set_index(0);

		m_buffer.bind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_POLYGON, 0, m_buffer.vertices());

		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

		// draw outline
		m_buffer.set_index(1);
		unsigned char tcol[4] = { tm->themeUI()->regular.outline.r(),
		        tm->themeUI()->regular.outline.g(),
		        tm->themeUI()->regular.outline.b(),
		        tm->themeUI()->regular.outline.a()};

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

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void Widget::update_shape(const Size* size)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][2];	// vertices for drawing inner

		int total_num = round_box_edges(size, inner_v, outer_v);

		m_buffer.generate(2);

		m_buffer.set_index(0);
		m_buffer.set_property(total_num, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.bind();
		m_buffer.upload(inner_v);
		m_buffer.unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
		//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip (inner_v, outer_v, total_num, quad_strip);

		m_buffer.set_index(1);
		m_buffer.set_property(total_num * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_buffer.bind();
		m_buffer.upload(quad_strip);
		m_buffer.unbind();
	}

	int Widget::round_box_edges(const Size* size, float inner_v[WIDGET_SIZE_MAX][2], float outer_v[WIDGET_SIZE_MAX][2])
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

		int total_num = 0;
		int halfwayvert = 0;
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
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, total_num++) {
				inner_v[total_num][0] = minxi + veci[i][1];
				inner_v[total_num][1] = minyi + radi - veci[i][0];

				outer_v[total_num][0] = minx + vec[i][1];
				outer_v[total_num][1] = miny + rad - vec[i][0];
			}
		}
		else {
			inner_v[total_num][0] = minxi;
			inner_v[total_num][1] = minyi;

			outer_v[total_num][0] = minx;
			outer_v[total_num][1] = miny;

			total_num++;
		}

		// corner right-bottom
		if (m_roundcorner & RoundCornerBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, total_num++) {
				inner_v[total_num][0] = maxxi - radi + veci[i][0];
				inner_v[total_num][1] = minyi + veci[i][1];

				outer_v[total_num][0] = maxx - rad + vec[i][0];
				outer_v[total_num][1] = miny + vec[i][1];
			}
		}
		else {
			inner_v[total_num][0] = maxxi;
			inner_v[total_num][1] = minyi;

			outer_v[total_num][0] = maxx;
			outer_v[total_num][1] = miny;
			total_num++;
		}

		halfwayvert = total_num;	// TODO: check how to use halfwayvert

		// corner right-top
		if (m_roundcorner & RoundCornerTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, total_num++) {
				inner_v[total_num][0] = maxxi - veci[i][1];
				inner_v[total_num][1] = maxyi - radi + veci[i][0];

				outer_v[total_num][0] = maxx - vec[i][1];
				outer_v[total_num][1] = maxy - rad + vec[i][0];
			}
		}
		else {
			inner_v[total_num][0] = maxxi;
			inner_v[total_num][1] = maxyi;

			outer_v[total_num][0] = maxx;
			outer_v[total_num][1] = maxy;
			total_num++;
		}

		// corner left-top
		if (m_roundcorner & RoundCornerTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, total_num++) {
				inner_v[total_num][0] = minxi + radi - veci[i][0];
				inner_v[total_num][1] = maxyi - veci[i][1];

				outer_v[total_num][0] = minx + rad - vec[i][0];
				outer_v[total_num][1] = maxy - vec[i][1];
			}

		}
		else {

			inner_v[total_num][0] = minxi;
			inner_v[total_num][1] = maxyi;

			outer_v[total_num][0] = minx;
			outer_v[total_num][1] = maxy;

			total_num++;
		}

		assert(total_num <= WIDGET_SIZE_MAX);

		return total_num;
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

	void Widget::shadecolors4(const Color& color, short shadetop, short shadedown, char coltop[4], char coldown[4])
	{
		coltop[0] = correct_in_scope(color.r() + shadetop, 0, 255);
		coltop[1] = correct_in_scope(color.g() + shadetop, 0, 255);
		coltop[2] = correct_in_scope(color.b() + shadetop, 0, 255);
		coltop[3] = color.a();

		coldown[0] = correct_in_scope(color.r() + shadedown, 0, 255);
		coldown[1] = correct_in_scope(color.g() + shadedown, 0, 255);
		coldown[2] = correct_in_scope(color.b() + shadedown, 0, 255);
		coldown[3] = color.a();
	}

	void Widget::round_box_shade_col4_r(const char color1[4], const char color2[4], const float fac, unsigned char color_out[4])
	{
		const int faci = convert_color_from_float(fac);
		const int facm = 255 - faci;

		color_out[0] = (faci * color1[0] + facm * color2[0]) >> 8;
		color_out[1] = (faci * color1[1] + facm * color2[1]) >> 8;
		color_out[2] = (faci * color1[2] + facm * color2[2]) >> 8;
		color_out[3] = (faci * color1[3] + facm * color2[3]) >> 8;
	}

} /* namespace BILO */

