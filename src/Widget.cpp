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

	static const float cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	static const float jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	const float Widget::quarter_corner_vertexes[9][2] =
	{
			{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
			{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
			{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	static void widget_verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][2],
			const float outer_v[WIDGET_SIZE_MAX][2],
			const int totvert,
			float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
	{
		int a;
		for (a = 0; a < totvert; a++) {
			copy_v2_v2(quad_strip[a * 2], outer_v[a]);
			copy_v2_v2(quad_strip[a * 2 + 1], inner_v[a]);
		}
		copy_v2_v2(quad_strip[a * 2], outer_v[0]);
		copy_v2_v2(quad_strip[a * 2 + 1], inner_v[0]);
	}

	Widget::Widget ()
	: Drawable(), m_vertex_num(0), m_halfwayvert(0)
	{

	}

	Widget::Widget (Drawable* parent)
			: Drawable(parent), m_vertex_num(0), m_halfwayvert(0)
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

		draw_inner();

		draw_outline();

		/*
		if (m_buffer.is_buffer(0)) {


			Theme* tm = Theme::instance();

			glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
			        tm->themeUI()->wcol_regular.inner.g(),
			        tm->themeUI()->wcol_regular.inner.b(),
			        tm->themeUI()->wcol_regular.inner.a() * 0.5f);

			m_buffer.rebind();
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawArrays(GL_POLYGON, 0, 4);

			glColor4ub(tm->themeUI()->wcol_regular.outline.r(),
			        tm->themeUI()->wcol_regular.outline.g(),
			        tm->themeUI()->wcol_regular.outline.b(),
			        tm->themeUI()->wcol_regular.outline.a());

			glDrawArrays(GL_LINE_LOOP, 0, 4);

			glDisableClientState(GL_VERTEX_ARRAY);

			m_buffer.unbind();

		}
		 */

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void Widget::update_shape(const Size* size)
	{
		float rad = m_corner_radius;
		float radi = rad - 1.0;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + 1.0;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - 1.0; 	// U.pixelsize;
		float minyi = miny + 1.0;		// U.pixelsize;
		float maxyi = maxy - 1.0;		// U.pixelsize;

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
			radi = 0.5f * minsize - 1.0;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (m_roundcorner & RoundCornerBottomLeft) {

			for (int a = 0; a < WIDGET_CURVE_RESOLU; a++, total_num++) {
				m_inner_v[total_num][0] = minxi + veci[a][1];
				m_inner_v[total_num][1] = minyi + radi - veci[a][0];

				m_outer_v[total_num][0] = minx + vec[a][1];
				m_outer_v[total_num][1] = miny + rad - vec[a][0];
			}
		}
		else {
			m_inner_v[total_num][0] = minxi;
			m_inner_v[total_num][1] = minyi;

			m_outer_v[total_num][0] = minx;
			m_outer_v[total_num][1] = miny;

			total_num++;
		}

		// corner right-bottom
		if (m_roundcorner & RoundCornerBottomRight) {

			for (int a = 0; a < WIDGET_CURVE_RESOLU; a++, total_num++) {
				m_inner_v[total_num][0] = maxxi - radi + veci[a][0];
				m_inner_v[total_num][1] = minyi + veci[a][1];

				m_outer_v[total_num][0] = maxx - rad + vec[a][0];
				m_outer_v[total_num][1] = miny + vec[a][1];
			}
		}
		else {
			m_inner_v[total_num][0] = maxxi;
			m_inner_v[total_num][1] = minyi;

			m_outer_v[total_num][0] = maxx;
			m_outer_v[total_num][1] = miny;
			total_num++;
		}

		halfwayvert = total_num;

		// corner right-top
		if (m_roundcorner & RoundCornerTopRight) {

			for (int a = 0; a < WIDGET_CURVE_RESOLU; a++, total_num++) {
				m_inner_v[total_num][0] = maxxi - veci[a][1];
				m_inner_v[total_num][1] = maxyi - radi + veci[a][0];

				m_outer_v[total_num][0] = maxx - vec[a][1];
				m_outer_v[total_num][1] = maxy - rad + vec[a][0];
			}
		}
		else {
			m_inner_v[total_num][0] = maxxi;
			m_inner_v[total_num][1] = maxyi;

			m_outer_v[total_num][0] = maxx;
			m_outer_v[total_num][1] = maxy;
			total_num++;
		}

		// corner left-top
		if (m_roundcorner & RoundCornerTopLeft) {

			for (int a = 0; a < WIDGET_CURVE_RESOLU; a++, total_num++) {
				m_inner_v[total_num][0] = minxi + radi - veci[a][0];
				m_inner_v[total_num][1] = maxyi - veci[a][1];

				m_outer_v[total_num][0] = minx + rad - vec[a][0];
				m_outer_v[total_num][1] = maxy - vec[a][1];
			}

		}
		else {

			m_inner_v[total_num][0] = minxi;
			m_inner_v[total_num][1] = maxyi;

			m_outer_v[total_num][0] = minx;
			m_outer_v[total_num][1] = maxy;

			total_num++;
		}

		assert(total_num <= WIDGET_SIZE_MAX);

		//wt->totvert = total_num;

		m_buffer.generate(2);

		m_buffer.set_index(0);
		m_buffer.bind(GL_ARRAY_BUFFER);
		m_buffer.upload(sizeof(m_inner_v[0]) * total_num, m_inner_v, GL_STATIC_DRAW);
		m_buffer.unbind();

		m_buffer.set_index(1);
		m_buffer.bind(GL_ARRAY_BUFFER);
		m_buffer.upload(sizeof(m_outer_v[0]) * total_num, m_outer_v, GL_STATIC_DRAW);
		m_buffer.unbind();

		m_vertex_num = total_num;
		m_halfwayvert = halfwayvert;

		std::cout << "vertex number: " << m_vertex_num << std::endl;

		// ----

		/*
		float vertexes[4][2];
		vertexes[0][0] = 0;
		vertexes[0][1] = 0;
		vertexes[1][0] = size->width();
		vertexes[1][1] = 0;
		vertexes[2][0] = size->width();
		vertexes[2][1] = size->height();
		vertexes[3][0] = 0;
		vertexes[3][1] = size->height();

		m_buffer.generate (1);

		m_buffer.bind (GL_ARRAY_BUFFER);
		m_buffer.upload (sizeof(vertexes[0]) * 4, vertexes, GL_STATIC_DRAW);
		m_buffer.unbind ();
		*/
	}

	void Widget::draw_inner ()
	{
		Theme* tm = Theme::instance();

		// simple fill
		glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
		        tm->themeUI()->wcol_regular.inner.g(),
		        tm->themeUI()->wcol_regular.inner.b(),
		        tm->themeUI()->wcol_regular.inner.a());

		m_buffer.set_index(0);
		m_buffer.rebind();
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);

		glDrawArrays(GL_POLYGON, 0, m_vertex_num);

		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

	}

	void Widget::draw_outline ()
	{
		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
		float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */
		Theme* tm = Theme::instance();

		const unsigned char tcol[4] = {tm->themeUI()->wcol_regular.outline.r(),
				tm->themeUI()->wcol_regular.outline.g(),
				tm->themeUI()->wcol_regular.outline.b(),
				tm->themeUI()->wcol_regular.outline.a() / WIDGET_AA_JITTER};

		widget_verts_to_quad_strip(m_inner_v, m_outer_v, m_vertex_num, quad_strip);

//		if (wtb->emboss) {
//			widget_verts_to_quad_strip_open(wtb, wtb->halfwayvert, quad_strip_emboss);
//		}

		glEnableClientState(GL_VERTEX_ARRAY);

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);

			/* outline */
			glColor4ubv(tcol);

			glVertexPointer(2, GL_FLOAT, 0, quad_strip);
			glDrawArrays(GL_QUAD_STRIP, 0, m_vertex_num * 2 + 2);

			/* emboss bottom shadow */
//			if (wtb->emboss) {
//				glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
//
//				glVertexPointer(2, GL_FLOAT, 0, quad_strip_emboss);
//				glDrawArrays(GL_QUAD_STRIP, 0, wtb->halfwayvert * 2);
//			}

			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}

		glDisableClientState(GL_VERTEX_ARRAY);
	}

} /* namespace BILO */

