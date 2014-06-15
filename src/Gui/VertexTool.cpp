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
	: m_total(0), m_half(0)
	{

	}
	
	VertexTool::~VertexTool ()
	{
	}
	
	void VertexTool::Initialize()
	{
		m_total = 0;
		m_half = 0;

		m_inner.clear();
		m_outer.clear();
	}

	void VertexTool::Setup (
					const Size& size,
					int border,
					int round_type,
					int radius)
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
		int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
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

		{	// generate inner vertices

			if(m_inner.size() != ((outline_vertex_number + 2) * 2)) {
				m_inner.resize((outline_vertex_number + 2) * 2);
			}

			// inner[0, 0] is the center of a triangle fan
			m_inner[0] = (maxxi - minxi) / 2.f;
			m_inner[1] = (maxyi - minyi) / 2.f;

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 2] = minxi + veci[i][1];
					m_inner[count * 2 + 1] = minyi + radi - veci[i][0];
				}
			} else {
				m_inner[count * 2] = minxi;
				m_inner[count * 2 + 1] = minyi;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 2] = maxxi - radi + veci[i][0];
					m_inner[count * 2 + 1] = minyi + veci[i][1];
				}
			} else {
				m_inner[count * 2] = maxxi;
				m_inner[count * 2 + 1] = minyi;
				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 2] = maxxi - veci[i][1];
					m_inner[count * 2 + 1] = maxyi - radi + veci[i][0];
				}
			} else {
				m_inner[count * 2] = maxxi;
				m_inner[count * 2 + 1] = maxyi;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 2] = minxi + radi - veci[i][0];
					m_inner[count * 2 + 1] = maxyi - veci[i][1];
				}

			} else {
				m_inner[count * 2] = minxi;
				m_inner[count * 2 + 1] = maxyi;
				count++;
			}

			m_inner[count * 2] = m_inner[2];
			m_inner[count * 2 + 1] = m_inner[3];
		}

		if(border > 0) {

			if(m_outer.size() != (outline_vertex_number * 2)) {
				m_outer.resize(outline_vertex_number * 2);
			}

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2] = minx + vec[i][1];
					m_outer[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				m_outer[count * 2] = minx;
				m_outer[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2] = maxx - rad + vec[i][0];
					m_outer[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				m_outer[count * 2] = maxx;
				m_outer[count * 2 + 1] = miny;
				count++;
			}

			m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2] = maxx - vec[i][1];
					m_outer[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				m_outer[count * 2] = maxx;
				m_outer[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2] = minx + rad - vec[i][0];
					m_outer[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				m_outer[count * 2] = minx;
				m_outer[count * 2 + 1] = maxy;
				count++;
			}

		}

		m_total = count;
	}
	
	void VertexTool::Setup (
					const Size& size,
					int border,
					int round_type,
					int radius,
					const Color& color,
					Orientation shadedir,
					short shadetop,
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

		Color color_top = color + shadetop;
		Color color_down = color + shadedown;
		Color shaded_color;

		unsigned int corner = round_type & RoundAll;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

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

		{	// generate inner vertices

			if(m_inner.size() != ((outline_vertex_number + 2) * 6)) {
				m_inner.resize((outline_vertex_number + 2) * 6);
			}

			// inner[0, 0] is the center of a triangle fan
			m_inner[0] = (maxxi - minxi) / 2.f;
			m_inner[1] = (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				shaded_color = MakeShadedColor(color_top, color_down,
								facyi * (m_inner[1] - minyi));
			} else {
				shaded_color = MakeShadedColor(color_top, color_down,
								facxi * (m_inner[0] - minxi));
			}
			m_inner[2] = shaded_color.r();
			m_inner[3] = shaded_color.g();
			m_inner[4] = shaded_color.b();
			m_inner[5] = shaded_color.a();

			count = 1;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 6 + 0] = minxi + veci[i][1];
					m_inner[count * 6 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (m_inner[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (m_inner[count * 6 + 0] - minxi));
					}

					m_inner[count * 6 + 2] = shaded_color.r();
					m_inner[count * 6 + 3] = shaded_color.g();
					m_inner[count * 6 + 4] = shaded_color.b();
					m_inner[count * 6 + 5] = shaded_color.a();
				}
			} else {
				m_inner[count * 6 + 0] = minxi;
				m_inner[count * 6 + 1] = minyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				}
				m_inner[count * 6 + 2] = shaded_color.r();
				m_inner[count * 6 + 3] = shaded_color.g();
				m_inner[count * 6 + 4] = shaded_color.b();
				m_inner[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 6 + 0] = maxxi - radi + veci[i][0];
					m_inner[count * 6 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (m_inner[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (m_inner[count * 6 + 0] - minxi));
					}
					m_inner[count * 6 + 2] = shaded_color.r();
					m_inner[count * 6 + 3] = shaded_color.g();
					m_inner[count * 6 + 4] = shaded_color.b();
					m_inner[count * 6 + 5] = shaded_color.a();
				}
			} else {
				m_inner[count * 6 + 0] = maxxi;
				m_inner[count * 6 + 1] = minyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				}
				m_inner[count * 6 + 2] = shaded_color.r();
				m_inner[count * 6 + 3] = shaded_color.g();
				m_inner[count * 6 + 4] = shaded_color.b();
				m_inner[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 6 + 0] = maxxi - veci[i][1];
					m_inner[count * 6 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (m_inner[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (m_inner[count * 6 + 0] - minxi));
					}
					m_inner[count * 6 + 2] = shaded_color.r();
					m_inner[count * 6 + 3] = shaded_color.g();
					m_inner[count * 6 + 4] = shaded_color.b();
					m_inner[count * 6 + 5] = shaded_color.a();
				}
			} else {
				m_inner[count * 6 + 0] = maxxi;
				m_inner[count * 6 + 1] = maxyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				}
				m_inner[count * 6 + 2] = shaded_color.r();
				m_inner[count * 6 + 3] = shaded_color.g();
				m_inner[count * 6 + 4] = shaded_color.b();
				m_inner[count * 6 + 5] = shaded_color.a();

				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_inner[count * 6 + 0] = minxi + radi - veci[i][0];
					m_inner[count * 6 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						shaded_color = MakeShadedColor(color_top, color_down,
										facyi * (m_inner[count * 6 + 1] - minyi));
					} else {
						shaded_color = MakeShadedColor(color_top, color_down,
										facxi * (m_inner[count * 6 + 0] - minxi));
					}
					m_inner[count * 6 + 2] = shaded_color.r();
					m_inner[count * 6 + 3] = shaded_color.g();
					m_inner[count * 6 + 4] = shaded_color.b();
					m_inner[count * 6 + 5] = shaded_color.a();
				}
			} else {

				m_inner[count * 6 + 0] = minxi;
				m_inner[count * 6 + 1] = maxyi;

				if (shadedir == Vertical) {
					shaded_color = MakeShadedColor(color_top, color_down, 1.0f);
				} else {
					shaded_color = MakeShadedColor(color_top, color_down, 0.0f);
				}
				m_inner[count * 6 + 2] = shaded_color.r();
				m_inner[count * 6 + 3] = shaded_color.g();
				m_inner[count * 6 + 4] = shaded_color.b();
				m_inner[count * 6 + 5] = shaded_color.a();

				count++;
			}

			m_inner[count * 6 + 0] = m_inner[6 + 0];
			m_inner[count * 6 + 1] = m_inner[6 + 1];
			m_inner[count * 6 + 2] = m_inner[6 + 2];
			m_inner[count * 6 + 3] = m_inner[6 + 3];
			m_inner[count * 6 + 4] = m_inner[6 + 4];
			m_inner[count * 6 + 5] = m_inner[6 + 5];

		}

		if (border > 0) {

			if(m_outer.size() != (outline_vertex_number * 2)) {
				m_outer.resize(outline_vertex_number * 2);
			}

			count = 0;

			// corner left-bottom
			if (round_type & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2 + 0] = minx + vec[i][1];
					m_outer[count * 2 + 1] = miny + rad - vec[i][0];
				}
			} else {
				m_outer[count * 2 + 0] = minx;
				m_outer[count * 2 + 1] = miny;
				count++;
			}

			// corner right-bottom
			if (round_type & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2 + 0] = maxx - rad + vec[i][0];
					m_outer[count * 2 + 1] = miny + vec[i][1];
				}
			} else {
				m_outer[count * 2 + 0] = maxx;
				m_outer[count * 2 + 1] = miny;
				count++;
			}

			m_half = count;

			// corner right-top
			if (round_type & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2 + 0] = maxx - vec[i][1];
					m_outer[count * 2 + 1] = maxy - rad + vec[i][0];
				}
			} else {
				m_outer[count * 2 + 0] = maxx;
				m_outer[count * 2 + 1] = maxy;
				count++;
			}

			// corner left-top
			if (round_type & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					m_outer[count * 2 + 0] = minx + rad - vec[i][0];
					m_outer[count * 2 + 1] = maxy - vec[i][1];
				}
			} else {
				m_outer[count * 2 + 0] = minx;
				m_outer[count * 2 + 1] = maxy;
				count++;
			}
		}

		m_total = count;
	}
	
	void VertexTool::GenerateTriangleStripVertices (
					const std::vector<GLfloat>& inner,
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
							m_total, outer.size() / 2);
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
	
	RefPtr<GLArrayBuffer> VertexTool::GenerateInnerBuffer (GLenum usage)
	{
		RefPtr<GLArrayBuffer> buf(new GLArrayBuffer);

		buf->Generate();
		buf->Bind();
		buf->SetData(sizeof(GLfloat) * m_inner.size(), &m_inner[0], usage);
		buf->Reset();

		return buf;
	}

	void VertexTool::SetInnerBufferData(GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			buffer->set_data(sizeof(GLfloat) * m_inner.size(), &m_inner[0], usage);
		}
	}

	void VertexTool::UpdateInnerBuffer (GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			if(!buffer->IsBuffer()) {
				buffer->Generate();
			}
			buffer->Bind();
			buffer->SetData(sizeof(GLfloat) * m_inner.size(), &m_inner[0], usage);
			buffer->Reset();
		}
	}
	
	RefPtr<GLArrayBuffer> VertexTool::GenerateOuterBuffer (GLenum usage)
	{
		RefPtr<GLArrayBuffer> buf(new GLArrayBuffer);

		std::vector<GLfloat> strip;
		GenerateTriangleStripVertices(m_inner, m_outer, m_total, strip);

		buf->Generate();
		buf->Bind();
		buf->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
		buf->Reset();

		return buf;
	}
	
	void VertexTool::SetOuterBufferData (GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			std::vector<GLfloat> strip;
			GenerateTriangleStripVertices(m_inner, m_outer, m_total, strip);

			buffer->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
		}
	}

	void VertexTool::UpdateOuterBuffer (GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			std::vector<GLfloat> strip;
			GenerateTriangleStripVertices(m_inner, m_outer, m_total, strip);

			if(!buffer->IsBuffer()) {
				buffer->Generate();
			}
			buffer->Bind();
			buffer->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
			buffer->Reset();
		}
	}
	
	RefPtr<GLArrayBuffer> VertexTool::GenerateEmbossBuffer (GLenum usage)
	{
		RefPtr<GLArrayBuffer> buf(new GLArrayBuffer);

		std::vector<GLfloat> strip;
		GenerateTriangleStripVertices(m_inner, m_outer, m_half, strip);

		buf->Generate();
		buf->Bind();
		buf->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
		buf->Reset();

		return buf;
	}
	
	void VertexTool::SetEmbossBufferData (GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			std::vector<GLfloat> strip;
			GenerateOpenTriangleStripVertices(m_outer, m_half, strip);

			buffer->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
		}
	}

	void VertexTool::UpdateEmbossBuffer (GLArrayBuffer* buffer, GLenum usage)
	{
		if(buffer) {
			std::vector<GLfloat> strip;
			GenerateOpenTriangleStripVertices(m_outer, m_half, strip);

			if(!buffer->IsBuffer()) {
				buffer->Generate();
			}
			buffer->Bind();
			buffer->SetData(sizeof(GLfloat) * strip.size(), &strip[0], usage);
			buffer->Reset();
		}
	}

	void VertexTool::GenerateOpenTriangleStripVertices (
					const std::vector<GLfloat>& outer,
					unsigned int num,
					std::vector<GLfloat>& strip)
	{
		if (num > outer.size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
							m_total, outer.size() / 2);
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

}
