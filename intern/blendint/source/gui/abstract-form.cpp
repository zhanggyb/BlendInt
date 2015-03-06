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

#include <gui/abstract-form.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	float AbstractForm::kBorderWidth = 1.f;

	const float AbstractForm::cornervec[WIDGET_CURVE_RESOLU][2] = {
					{ 0.0, 0.0 },
					{ 0.195, 0.02 },
					{ 0.383, 0.067 },
					{ 0.55,	0.169 },
					{ 0.707, 0.293 },
					{ 0.831, 0.45 },
					{ 0.924, 0.617 },
					{ 0.98, 0.805 },
					{ 1.0, 1.0 } };

	void AbstractForm::SetDefaultBorderWidth (int border)
	{
		kBorderWidth = border;
	}

	void AbstractForm::Resize (int width, int height)
	{
		if(size_.width() == width && size_.height() == height) return;

		Size new_size (width, height);

		PerformSizeUpdate(new_size);
	}
	
	void AbstractForm::Resize (const Size& size)
	{
		if(size_ == size) return;

		PerformSizeUpdate(size);
	}

	void AbstractForm::GenerateVertices(const Size& size, float border,
			int round_type, float radius, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		border *= AbstractWindow::theme()->pixel();

		float rad = radius * AbstractWindow::theme()->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border; 	// U.pixelsize;
		float minyi = miny + border;		// U.pixelsize;
		float maxyi = maxy - border;		// U.pixelsize;

		int minsize = 0;
		int corner = round_type;
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			((*inner_ptr))[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;
			(*inner_ptr)[2] = 0.f;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}

			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			(*inner_ptr)[count * 3] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = 0.f;
		}

		if(outer) {

			if(border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
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
				if (corner & RoundBottomRight) {
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
				if (corner & RoundTopRight) {
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
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}
	}

	void AbstractForm::GenerateVertices(const Size& size, float border,
			int round_type, float radius, Orientation shadedir, short shadetop,
			short shadedown, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		border *= AbstractWindow::theme()->pixel();

		float rad = radius * AbstractWindow::theme()->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border;
		float maxxi = maxx - border;
		float minyi = miny + border;
		float maxyi = maxy - border;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int corner = round_type;
		int minsize = 0;
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		float offset = 0.f;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border * AbstractWindow::theme()->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			(*inner_ptr)[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				offset = make_shaded_offset(shadetop, shadedown,
								facyi * ((*inner_ptr)[1] - minyi));
			} else {
				offset = make_shaded_offset(shadetop, shadedown,
								facxi * ((*inner_ptr)[0] - minxi));
			}
			(*inner_ptr)[2] = offset;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}

					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {

				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			(*inner_ptr)[count * 3 + 0] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = (*inner_ptr)[3 + 2];

		}

		if(outer) {

			if (border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
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
				if (corner & RoundBottomRight) {
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
				if (corner & RoundTopRight) {
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
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2 + 0] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}
	}

	inline float AbstractForm::make_shaded_offset(short shadetop,
			short shadedown, float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

	void AbstractForm::GenerateTriangleStripVertices(
			const std::vector<GLfloat>* inner, const std::vector<GLfloat>* edge,
			unsigned int num, std::vector<GLfloat>* strip)
	{
		if (num > edge->size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, edge->size() / 2);
			return;
		}

		if (strip->size() != (num * 2 + 2) * 2) {
			strip->resize((num * 2 + 2) * 2);
		}

		size_t count = 0;
		for (int i = 0, j = 0; count < num * 2; count++) {
			if (count % 2 == 0) {
				(*strip)[count * 2] = (*inner)[3 + i];
				(*strip)[count * 2 + 1] = (*inner)[3 + i + 1];
				i += 3;
			} else {
				(*strip)[count * 2] = (*edge)[j];
				(*strip)[count * 2 + 1] = (*edge)[j + 1];
				j += 2;
			}
		}

		(*strip)[count * 2] = (*inner)[3 + 0];
		(*strip)[count * 2 + 1] = (*inner)[3 + 1];
		(*strip)[count * 2 + 2] = (*edge)[0];
		(*strip)[count * 2 + 3] = (*edge)[1];
	}

}
