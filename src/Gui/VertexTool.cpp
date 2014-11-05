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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	const float VertexTool::cornervec[WIDGET_CURVE_RESOLU][2] = {
        { 0.0, 0.0 },
        { 0.195, 0.02 },
        { 0.383, 0.067 },
        { 0.55,	0.169 },
        { 0.707, 0.293 },
        { 0.831, 0.45 },
        { 0.924, 0.617 },
        { 0.98, 0.805 },
        { 1.0, 1.0 }
    };

	VertexTool::VertexTool()
	{
	}
	
	VertexTool::~VertexTool ()
	{
	}
	
	void VertexTool::GenerateVertices(
					const Size& size,
					float border,
					int round_type,
					float radius)
	{
		float rad = radius * Theme::instance->pixel();
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border * Theme::instance->pixel(); 	// U.pixelsize;
		float minyi = miny + border * Theme::instance->pixel();		// U.pixelsize;
		float maxyi = maxy - border * Theme::instance->pixel();		// U.pixelsize;

		int count = 0;

		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_.size() != ((outline_vertex_number + 2) * 2)) {
				inner_.resize((outline_vertex_number + 2) * 2);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 2] = minxi + veci[i][1];
					inner_[count * 2 + 1] = minyi + radi - veci[i][0];
				}
			} else {
				inner_[count * 2] = minxi;
				inner_[count * 2 + 1] = minyi;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 2] = maxxi - radi + veci[i][0];
					inner_[count * 2 + 1] = minyi + veci[i][1];
				}
			} else {
				inner_[count * 2] = maxxi;
				inner_[count * 2 + 1] = minyi;
				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 2] = maxxi - veci[i][1];
					inner_[count * 2 + 1] = maxyi - radi + veci[i][0];
				}
			} else {
				inner_[count * 2] = maxxi;
				inner_[count * 2 + 1] = maxyi;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 2] = minxi + radi - veci[i][0];
					inner_[count * 2 + 1] = maxyi - veci[i][1];
				}

			} else {
				inner_[count * 2] = minxi;
				inner_[count * 2 + 1] = maxyi;
				count++;
			}

			inner_[count * 2] = inner_[2];
			inner_[count * 2 + 1] = inner_[3];
		}

		if(border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateTriangleStripVertices(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}
	}

	void VertexTool::GenerateShadedVertices(
					const Size& size,
					float border,
					int round_type,
					float radius)
	{
		float rad = radius * Theme::instance->pixel();
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border * Theme::instance->pixel(); 	// U.pixelsize;
		float minyi = miny + border * Theme::instance->pixel();		// U.pixelsize;
		float maxyi = maxy - border * Theme::instance->pixel();		// U.pixelsize;

		int count = 0;

		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_.size() != ((outline_vertex_number + 2) * 3)) {
				inner_.resize((outline_vertex_number + 2) * 3);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;
			inner_[2] = 0.f;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = minxi + veci[i][1];
					inner_[count * 3 + 1] = minyi + radi - veci[i][0];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = minxi;
				inner_[count * 3 + 1] = minyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = maxxi - radi + veci[i][0];
					inner_[count * 3 + 1] = minyi + veci[i][1];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = maxxi;
				inner_[count * 3 + 1] = minyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = maxxi - veci[i][1];
					inner_[count * 3 + 1] = maxyi - radi + veci[i][0];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = maxxi;
				inner_[count * 3 + 1] = maxyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = minxi + radi - veci[i][0];
					inner_[count * 3 + 1] = maxyi - veci[i][1];
					inner_[count * 3 + 2] = 0.f;
				}

			} else {
				inner_[count * 3] = minxi;
				inner_[count * 3 + 1] = maxyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			inner_[count * 3] = inner_[3 + 0];
			inner_[count * 3 + 1] = inner_[3 + 1];
			inner_[count * 3 + 2] = 0.f;
		}

		if(border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateTriangleStripVerticesExt(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}
	}

	void VertexTool::GenerateShadedVerticesExt(
					const Size& size,
					float border,
					int round_type,
					float radius)
	{
		float rad = radius * Theme::instance->pixel();
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border * Theme::instance->pixel(); 	// U.pixelsize;
		float minyi = miny + border * Theme::instance->pixel();		// U.pixelsize;
		float maxyi = maxy - border * Theme::instance->pixel();		// U.pixelsize;

		int count = 0;

		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_.size() != ((outline_vertex_number + 2) * 3)) {
				inner_.resize((outline_vertex_number + 2) * 3);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;
			inner_[2] = 0.f;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = minxi + veci[i][1];
					inner_[count * 3 + 1] = minyi + radi - veci[i][0];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = minxi;
				inner_[count * 3 + 1] = minyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = maxxi - radi + veci[i][0];
					inner_[count * 3 + 1] = minyi + veci[i][1];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = maxxi;
				inner_[count * 3 + 1] = minyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = maxxi - veci[i][1];
					inner_[count * 3 + 1] = maxyi - radi + veci[i][0];
					inner_[count * 3 + 2] = 0.f;
				}
			} else {
				inner_[count * 3] = maxxi;
				inner_[count * 3 + 1] = maxyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3] = minxi + radi - veci[i][0];
					inner_[count * 3 + 1] = maxyi - veci[i][1];
					inner_[count * 3 + 2] = 0.f;
				}

			} else {
				inner_[count * 3] = minxi;
				inner_[count * 3 + 1] = maxyi;
				inner_[count * 3 + 2] = 0.f;
				count++;
			}

			inner_[count * 3] = inner_[3 + 0];
			inner_[count * 3 + 1] = inner_[3 + 1];
			inner_[count * 3 + 2] = 0.f;
		}

		if(border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateOutlineTriangleStripVertices(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}
	}

	void VertexTool::GenerateShadedVertices (const Size& size, float border,
			int round_type, float radius, Orientation shadedir, short shadetop,
			short shadedown)
	{
		float rad = radius;
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();
		float maxxi = maxx - border * Theme::instance->pixel();
		float minyi = miny + border * Theme::instance->pixel();
		float maxyi = maxy - border * Theme::instance->pixel();

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int count = 0;
		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		float offset = 0.f;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_.size() != ((outline_vertex_number + 2) * 3)) {
				inner_.resize((outline_vertex_number + 2) * 3);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				offset = make_shaded_offset(shadetop, shadedown,
								facyi * (inner_[1] - minyi));
			} else {
				offset = make_shaded_offset(shadetop, shadedown,
								facxi * (inner_[0] - minxi));
			}
			inner_[2] = offset;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = minxi + veci[i][1];
					inner_[count * 3 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}

					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = minxi;
				inner_[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = maxxi - radi + veci[i][0];
					inner_[count * 3 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = maxxi;
				inner_[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = maxxi - veci[i][1];
					inner_[count * 3 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = maxxi;
				inner_[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = minxi + radi - veci[i][0];
					inner_[count * 3 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {

				inner_[count * 3 + 0] = minxi;
				inner_[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			inner_[count * 3 + 0] = inner_[3 + 0];
			inner_[count * 3 + 1] = inner_[3 + 1];
			inner_[count * 3 + 2] = inner_[3 + 2];

		}

		if (border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateTriangleStripVerticesExt(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}

	}

	void VertexTool::GenerateShadedVerticesExt (const Size& size, float border,
			int round_type, float radius, Orientation shadedir, short shadetop,
			short shadedown)
	{
		float rad = radius;
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();
		float maxxi = maxx - border * Theme::instance->pixel();
		float minyi = miny + border * Theme::instance->pixel();
		float maxyi = maxy - border * Theme::instance->pixel();

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int count = 0;
		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		float offset = 0.f;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_.size() != ((outline_vertex_number + 2) * 3)) {
				inner_.resize((outline_vertex_number + 2) * 3);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				offset = make_shaded_offset(shadetop, shadedown,
								facyi * (inner_[1] - minyi));
			} else {
				offset = make_shaded_offset(shadetop, shadedown,
								facxi * (inner_[0] - minxi));
			}
			inner_[2] = offset;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = minxi + veci[i][1];
					inner_[count * 3 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}

					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = minxi;
				inner_[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = maxxi - radi + veci[i][0];
					inner_[count * 3 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = maxxi;
				inner_[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = maxxi - veci[i][1];
					inner_[count * 3 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {
				inner_[count * 3 + 0] = maxxi;
				inner_[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 3 + 0] = minxi + radi - veci[i][0];
					inner_[count * 3 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * (inner_[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * (inner_[count * 3 + 0] - minxi));
					}
					inner_[count * 3 + 2] = offset;
				}
			} else {

				inner_[count * 3 + 0] = minxi;
				inner_[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				}
				inner_[count * 3 + 2] = offset;

				count++;
			}

			inner_[count * 3 + 0] = inner_[3 + 0];
			inner_[count * 3 + 1] = inner_[3 + 1];
			inner_[count * 3 + 2] = inner_[3 + 2];

		}

		if (border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateOutlineTriangleStripVertices(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}

	}

	void VertexTool::GenerateVertices (const Size& size, float border,
			int round_type, float radius, const Color& color, Orientation shadedir,
			short shadetop, short shadedown)
	{
		float rad = radius;
		float radi = rad - border * Theme::instance->pixel();

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border * Theme::instance->pixel();
		float maxxi = maxx - border * Theme::instance->pixel();
		float minyi = miny + border * Theme::instance->pixel();
		float maxyi = maxy - border * Theme::instance->pixel();

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int count = 0;
		int minsize = 0;
		const int hnum = (
                (round_type & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (round_type & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (round_type & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (round_type & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		Color color_top = color + shadetop;
		Color color_down = color + shadedown;
		Color shaded_color;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * radius > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border * Theme::instance->pixel()) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_.size() != ((outline_vertex_number + 2) * 6)) {
				inner_.resize((outline_vertex_number + 2) * 6);
			}

			// inner[0, 0] is the center of a triangle fan
			inner_[0] = minxi + (maxxi - minxi) / 2.f;
			inner_[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				shaded_color = MakeShadedColor(color_top, color_down,
								facyi * (inner_[1] - minyi));
			} else {
				shaded_color = MakeShadedColor(color_top, color_down,
								facxi * (inner_[0] - minxi));
			}
			inner_[2] = shaded_color.r();
			inner_[3] = shaded_color.g();
			inner_[4] = shaded_color.b();
			inner_[5] = shaded_color.a();

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 6 + 0] = minxi + veci[i][1];
					inner_[count * 6 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (inner_[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (inner_[count * 6 + 0] - minxi));
					}

					inner_[count * 6 + 2] = shaded_color.r();
					inner_[count * 6 + 3] = shaded_color.g();
					inner_[count * 6 + 4] = shaded_color.b();
					inner_[count * 6 + 5] = shaded_color.a();
				}
			} else {
				inner_[count * 6 + 0] = minxi;
				inner_[count * 6 + 1] = minyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				}
				inner_[count * 6 + 2] = shaded_color.r();
				inner_[count * 6 + 3] = shaded_color.g();
				inner_[count * 6 + 4] = shaded_color.b();
				inner_[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 6 + 0] = maxxi - radi + veci[i][0];
					inner_[count * 6 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (inner_[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (inner_[count * 6 + 0] - minxi));
					}
					inner_[count * 6 + 2] = shaded_color.r();
					inner_[count * 6 + 3] = shaded_color.g();
					inner_[count * 6 + 4] = shaded_color.b();
					inner_[count * 6 + 5] = shaded_color.a();
				}
			} else {
				inner_[count * 6 + 0] = maxxi;
				inner_[count * 6 + 1] = minyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				}
				inner_[count * 6 + 2] = shaded_color.r();
				inner_[count * 6 + 3] = shaded_color.g();
				inner_[count * 6 + 4] = shaded_color.b();
				inner_[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 6 + 0] = maxxi - veci[i][1];
					inner_[count * 6 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (inner_[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (inner_[count * 6 + 0] - minxi));
					}
					inner_[count * 6 + 2] = shaded_color.r();
					inner_[count * 6 + 3] = shaded_color.g();
					inner_[count * 6 + 4] = shaded_color.b();
					inner_[count * 6 + 5] = shaded_color.a();
				}
			} else {
				inner_[count * 6 + 0] = maxxi;
				inner_[count * 6 + 1] = maxyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				}
				inner_[count * 6 + 2] = shaded_color.r();
				inner_[count * 6 + 3] = shaded_color.g();
				inner_[count * 6 + 4] = shaded_color.b();
				inner_[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					inner_[count * 6 + 0] = minxi + radi - veci[i][0];
					inner_[count * 6 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (inner_[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (inner_[count * 6 + 0] - minxi));
					}
					inner_[count * 6 + 2] = shaded_color.r();
					inner_[count * 6 + 3] = shaded_color.g();
					inner_[count * 6 + 4] = shaded_color.b();
					inner_[count * 6 + 5] = shaded_color.a();
				}
			} else {

				inner_[count * 6 + 0] = minxi;
				inner_[count * 6 + 1] = maxyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				}
				inner_[count * 6 + 2] = shaded_color.r();
				inner_[count * 6 + 3] = shaded_color.g();
				inner_[count * 6 + 4] = shaded_color.b();
				inner_[count * 6 + 5] = shaded_color.a();

				count++;
			}

			inner_[count * 6 + 0] = inner_[6 + 0];
			inner_[count * 6 + 1] = inner_[6 + 1];
			inner_[count * 6 + 2] = inner_[6 + 2];
			inner_[count * 6 + 3] = inner_[6 + 3];
			inner_[count * 6 + 4] = inner_[6 + 4];
			inner_[count * 6 + 5] = inner_[6 + 5];

		}

		if (border > 0.f) {

			std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + vec[i][1];
					edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - rad + vec[i][0];
					edge_vertices[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = miny;
				count++;
			}

			// m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = maxx - vec[i][1];
					edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				edge_vertices[count * 2 + 0] = maxx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
					edge_vertices[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				edge_vertices[count * 2 + 0] = minx;
				edge_vertices[count * 2 + 1] = maxy;
				count++;
			}

			GenerateTriangleStripVertices(inner_, edge_vertices, count, outer_);

		} else {

			outer_.clear();

		}

	}

	void VertexTool::GenerateTriangleStripVerticesExt (
					const std::vector<GLfloat>& inner,
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, outer.size() / 2);
			return;
		}

		if (strip.size() != (num * 2 + 2) * 3) {
			strip.resize((num * 2 + 2) * 3);
		}

		size_t count = 0;
		for (int i = 0, j = 0; count < num * 2; count++) {
			if (count % 2 == 0) {
				strip[count * 3] = inner[3 + i];
				strip[count * 3 + 1] = inner[3 + i + 1];
				strip[count * 3 + 2] = 0.f;
				i += 3;
			} else {
				strip[count * 3] = outer[j];
				strip[count * 3 + 1] = outer[j + 1];
				strip[count * 3 + 2] = 0.f;
				j += 2;
			}
		}

		strip[count * 3] = inner[3 + 0];
		strip[count * 3 + 1] = inner[3 + 1];
		strip[count * 3 + 2] = 0.f;
		strip[count * 3 + 3] = outer[0];
		strip[count * 3 + 4] = outer[1];
		strip[count * 3 + 5] = 0.f;
	}

	void VertexTool::GenerateOutlineTriangleStripVertices (
					const std::vector<GLfloat>& inner,
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, outer.size() / 2);
			return;
		}

		if (strip.size() != (num * 2 + 2) * 2) {
			strip.resize((num * 2 + 2) * 2);
		}

		size_t count = 0;
		for (int i = 0, j = 0; count < num * 2; count++) {
			if (count % 2 == 0) {
				strip[count * 2] = inner[3 + i];
				strip[count * 2 + 1] = inner[3 + i + 1];
				i += 3;
			} else {
				strip[count * 2] = outer[j];
				strip[count * 2 + 1] = outer[j + 1];
				j += 2;
			}
		}

		strip[count * 2] = inner[3 + 0];
		strip[count * 2 + 1] = inner[3 + 1];
		strip[count * 2 + 2] = outer[0];
		strip[count * 2 + 3] = outer[1];
	}

	void VertexTool::GenerateTriangleStripVertices (
					const std::vector<GLfloat>& inner,
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, outer.size() / 2);
			return;
		}

		int step = 0;	// stride in inner

		if (inner.size() >= (outer.size() * 3 + 6 * 2)) {
			step = 6;
		} else if (inner.size() >= (outer.size() + 2 * 2)) {
			step = 2;
		} else {
			DBG_PRINT_MSG("Not enough inner vertices: %ld", inner.size());
			return;
		}

		if (strip.size() != (num * 2 + 2) * 2) {
			strip.resize((num * 2 + 2) * 2);
		}

		size_t count = 0;
		for (int i = 0, j = 0; count < num * 2; count++) {
			if (count % 2 == 0) {
				strip[count * 2] = inner[step + i];
				strip[count * 2 + 1] = inner[step + i + 1];
				i += step;
			} else {
				strip[count * 2] = outer[j];
				strip[count * 2 + 1] = outer[j + 1];
				j += 2;
			}
		}

		strip[count * 2] = inner[step + 0];
		strip[count * 2 + 1] = inner[step + 1];
		strip[count * 2 + 2] = outer[0];
		strip[count * 2 + 3] = outer[1];
	}

	void VertexTool::GenerateOpenTriangleStripVertices (
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, outer.size() / 2);
			return;
		}

		if (strip.size() != (num * 2 + 2) * 2) {
			strip.resize((num * 2 + 2) * 2);
		}

		size_t count = 0;
		for (int i = 0; count < (num * 4); i += 2, count += 4) {
			strip[count + 0] = outer[i];
			strip[count + 1] = outer[i + 1];
			strip[count + 2] = outer[i];
			strip[count + 3] = outer[i + 1] - 1.f;
		}
	}

	float VertexTool::make_shaded_offset (short shadetop, short shadedown, float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

}
