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

#include <assert.h>
#include <BlendInt/AbstractForm.hpp>

#include <BlendInt/Utilities-inl.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	const float AbstractForm::cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	const float AbstractForm::jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};


	AbstractForm::AbstractForm()
	{

	}

	AbstractForm::AbstractForm(const AbstractForm& orig)
	{
		m_size = orig.size();
	}

	AbstractForm::~AbstractForm()
	{

	}

	bool AbstractForm::Resize(unsigned int width, unsigned int height)
	{
		Size new_size (width, height);

		if (m_size == new_size) return false;

		Update(FormSize, &new_size);

		m_size.set_width(width);
		m_size.set_height(height);

		return true;
	}

	bool AbstractForm::Resize(const Size& size)
	{
		if(m_size == size) return false;

		Update(FormSize, &size);
		m_size = size;

		return true;
	}

	void AbstractForm::generate_rect_vertices(const Size* size,
			float border,
			float inner_v[4][2],
			float outer_v[4][2])
	{
		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + border;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border; 	// U.pixelsize;
		float minyi = miny + border;		// U.pixelsize;
		float maxyi = maxy - border;		// U.pixelsize;

		inner_v[0][0] = minxi;
		inner_v[0][1] = minyi;
		outer_v[0][0] = minx;
		outer_v[0][1] = miny;

		inner_v[1][0] = maxxi;
		inner_v[1][1] = minyi;
		outer_v[1][0] = maxx;
		outer_v[1][1] = miny;

		inner_v[2][0] = maxxi;
		inner_v[2][1] = maxyi;
		outer_v[2][0] = maxx;
		outer_v[2][1] = maxy;

		inner_v[3][0] = minxi;
		inner_v[3][1] = maxyi;
		outer_v[3][0] = minx;
		outer_v[3][1] = maxy;
	}

	VerticesSum AbstractForm::generate_round_vertices(const Size* size,
			float border,
			int round_type,
			float radius,
			float inner_v[WIDGET_SIZE_MAX][2],
			float outer_v[WIDGET_SIZE_MAX][2])
	{
		float rad = radius;
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + border;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border; 	// U.pixelsize;
		float minyi = miny + border;		// U.pixelsize;
		float maxyi = maxy - border;		// U.pixelsize;

		VerticesSum sum;

		int count = 0;

		int minsize = 0;
		const int hnum = ((round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight) ||
		                  (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft) ||
		                  (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)) ? 1 : 2;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type & RoundBottomLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = minxi + veci[i][1];
				inner_v[count][1] = minyi + radi - veci[i][0];

				outer_v[count][0] = minx + vec[i][1];
				outer_v[count][1] = miny + rad - vec[i][0];
			}
		}	else {
			inner_v[count][0] = minxi;
			inner_v[count][1] = minyi;

			outer_v[count][0] = minx;
			outer_v[count][1] = miny;
			count++;
		}

		// corner right-bottom
		if (round_type & RoundBottomRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = maxxi - radi + veci[i][0];
				inner_v[count][1] = minyi + veci[i][1];

				outer_v[count][0] = maxx - rad + vec[i][0];
				outer_v[count][1] = miny + vec[i][1];
			}
		}	else {
			inner_v[count][0] = maxxi;
			inner_v[count][1] = minyi;

			outer_v[count][0] = maxx;
			outer_v[count][1] = miny;
			count++;
		}

		sum.half = count;

		// corner right-top
		if (round_type & RoundTopRight) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = maxxi - veci[i][1];
				inner_v[count][1] = maxyi - radi + veci[i][0];

				outer_v[count][0] = maxx - vec[i][1];
				outer_v[count][1] = maxy - rad + vec[i][0];
			}
		}	else {
			inner_v[count][0] = maxxi;
			inner_v[count][1] = maxyi;

			outer_v[count][0] = maxx;
			outer_v[count][1] = maxy;
			count++;
		}

		// corner left-top
		if (round_type & RoundTopLeft) {
			for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
				inner_v[count][0] = minxi + radi - veci[i][0];
				inner_v[count][1] = maxyi - veci[i][1];

				outer_v[count][0] = minx + rad - vec[i][0];
				outer_v[count][1] = maxy - vec[i][1];
			}

		}	else {
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

	VerticesSum AbstractForm::generate_round_vertices (const Size* size,
			float border,
			int round_type,
			float radius,
			const WidgetTheme* theme,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = radius;
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + border;
		float maxxi = maxx - border;
		float minyi = miny + border;
		float maxyi = maxy - border;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		VerticesSum sum;
		int count = 0;
		int minsize = 0;
		const int hnum = ((round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight) ||
		                  (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft) ||
		                  (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)) ? 1 : 2;

		Color color_top = theme->inner + theme->shadetop;
		Color color_down = theme->inner + theme->shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type & RoundBottomLeft) {
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
		} else {
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
		if (round_type & RoundBottomRight) {
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
		}	else {
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
		if (round_type & RoundTopRight) {
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
		}	else {
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
		if (round_type & RoundTopLeft) {
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
		}	else {

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

	VerticesSum AbstractForm::generate_round_vertices (const Size* size,
			float border,
			int round_type,
			float radius,
			const Color& color,
			short shadetop,
			short shadedown,
			Orientation shadedir,
			float inner[WIDGET_SIZE_MAX][6],
			float outer[WIDGET_SIZE_MAX][2])
	{
		float rad = radius;
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size->width();
		float maxy = size->height();

		float minxi = minx + border;
		float maxxi = maxx - border;
		float minyi = miny + border;
		float maxyi = maxy - border;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		VerticesSum sum;
		int count = 0;
		int minsize = 0;
		const int hnum = ((round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight) ||
		                  (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)) ? 1 : 2;
		const int vnum = ((round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft) ||
		                  (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)) ? 1 : 2;

		Color color_top = color + shadetop;
		Color color_down = color + shadedown;
		Color shaded_color;

		minsize = std::min(size->width() * hnum,
		                 size->height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + 1.0f) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		// corner left-bottom
		if (round_type & RoundBottomLeft) {
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
		}	else {
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
		if (round_type & RoundBottomRight) {
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
		}	else {
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
		if (round_type & RoundTopRight) {
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
		}	else {
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
		if (round_type & RoundTopLeft) {
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
		}	else {

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


	void AbstractForm::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][2],
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

	void AbstractForm::verts_to_quad_strip(const float inner_v[WIDGET_SIZE_MAX][6],
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

	void AbstractForm::verts_to_quad_strip_open (
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

	void AbstractForm::dispatch_render(AbstractForm* obj)
	{
		obj->Render();
	}

}
