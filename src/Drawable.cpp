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
#include <iostream>

#include <BIL/Drawable.hpp>

namespace BIL {

	using namespace std;

	/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

	/* *********************** draw data ************************** */

	static const float cornervec[WIDGET_CURVE_RESOLU][2] = {
		{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
		{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
		{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

#define WIDGET_AA_JITTER 8
	static const float jit[WIDGET_AA_JITTER][2] = {
		{ 0.468813, -0.481430}, {-0.155755, -0.352820},
		{ 0.219306, -0.238501}, {-0.393286, -0.110949},
		{-0.024699,  0.013908}, { 0.343805,  0.147431},
		{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	static const float num_tria_vert[3][2] = {
		{-0.352077, 0.532607}, {-0.352077, -0.549313}, {0.330000, -0.008353}
	};

	static const unsigned int num_tria_face[1][3] = {
		{0, 1, 2}
	};

	static const float scroll_circle_vert[16][2] = { {0.382684, 0.923879},
													 {0.000001, 1.000000},
													 {-0.382683, 0.923880},
													 {-0.707107, 0.707107},
													 {-0.923879, 0.382684},
													 {-1.000000, 0.000000},
													 {-0.923880, -0.382684},
													 {-0.707107, -0.707107},
													 {-0.382683, -0.923880},
													 {0.000000, -1.000000},
													 {0.382684, -0.923880},
													 {0.707107, -0.707107},
													 {0.923880, -0.382684},
													 {1.000000, -0.000000},
													 {0.923880, 0.382683},
													 {0.707107, 0.707107} };

	static const unsigned int scroll_circle_face[14][3] = {	{0, 1, 2},
															{2, 0, 3},
															{3, 0, 15},
															{3, 15, 4},
															{4, 15, 14},
															{4, 14, 5},
															{5, 14, 13},
															{5, 13, 6},
															{6, 13, 12},
															{6, 12, 7},
															{7, 12, 11},
															{7, 11, 8},
															{8, 11, 10},
															{8, 10, 9}
	};


	static const float menu_tria_vert[6][2] = {
		{-0.33, 0.16}, {0.33, 0.16}, {0, 0.82},
		{0, -0.82}, {-0.33, -0.16}, {0.33, -0.16}
	};



	static const unsigned int menu_tria_face[2][3] = {{2, 0, 1}, {3, 5, 4}};

	static const float check_tria_vert[6][2] = {
		{-0.578579, 0.253369},  {-0.392773, 0.412794},  {-0.004241, -0.328551},
		{-0.003001, 0.034320},  {1.055313, 0.864744},   {0.866408, 1.026895}
	};

	static const unsigned int check_tria_face[4][3] = {
		{3, 2, 4}, {3, 4, 5}, {1, 0, 3}, {0, 2, 3}
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

	/* ************************************************* */

	static inline void mul_v2_fl(float r[2], float f)
	{
		r[0] *= f;
		r[1] *= f;
	}

	static void round_box_shade_col(const float col1[3],
									float const col2[3],
									const float fac)
	{
		float col[3];

		col[0] = (fac * col1[0] + (1.0f - fac) * col2[0]);
		col[1] = (fac * col1[1] + (1.0f - fac) * col2[1]);
		col[2] = (fac * col1[2] + (1.0f - fac) * col2[2]);
		glColor3fv(col);
	}

	Drawable::Drawable (Traceable* parent)
		: Traceable(parent), round_box_type_ (RoundBoxNone), visible_(false)
	{

	}

	Drawable::~Drawable ()
	{
		// delete all child object in list

	}

	void Drawable::resize (const Vec2ui& size)
	{
		size_ = size;
		return;
	}

	void Drawable::drawRoundBox (float minx,
								 float miny,
								 float maxx,
								 float maxy,
								 float rad)
	{
		DrawAntiRoundbox(GL_POLYGON,
						 minx, miny,	maxx, maxy, rad, round_box_type_ & UI_RB_ALPHA);
	}

	void Drawable::DrawBox(int mode,
						   float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad)
	{
		float vec[7][2] = {	{0.195, 0.02},
							{0.383, 0.067},
							{0.55, 0.169},
							{0.707, 0.293},
							{0.831, 0.45},
							{0.924, 0.617},
							{0.98, 0.805} };
		int a;

		/* mult */
		for (a = 0; a < 7; a++) {
			mul_v2_fl(vec[a], rad);
		}

		glBegin(mode);

		/* start with corner right-bottom */
		if (round_box_type_ & RoundBoxBottomRight) {
			glVertex2f(maxx - rad, miny);
			for (a = 0; a < 7; a++) {
				glVertex2f(maxx - rad + vec[a][0], miny + vec[a][1]);
			}
			glVertex2f(maxx, miny + rad);
		}
		else {
			glVertex2f(maxx, miny);
		}

		/* corner right-top */
		if (round_box_type_ & RoundBoxTopRight) {
			glVertex2f(maxx, maxy - rad);
			for (a = 0; a < 7; a++) {
				glVertex2f(maxx - vec[a][1], maxy - rad + vec[a][0]);
			}
			glVertex2f(maxx - rad, maxy);
		}
		else {
			glVertex2f(maxx, maxy);
		}

		/* corner left-top */
		if (round_box_type_ & RoundBoxTopLeft) {
			glVertex2f(minx + rad, maxy);
			for (a = 0; a < 7; a++) {
				glVertex2f(minx + rad - vec[a][0], maxy - vec[a][1]);
			}
			glVertex2f(minx, maxy - rad);
		}
		else {
			glVertex2f(minx, maxy);
		}

		/* corner left-bottom */
		if (round_box_type_ & RoundBoxBottomLeft) {
			glVertex2f(minx, miny + rad);
			for (a = 0; a < 7; a++) {
				glVertex2f(minx + vec[a][1], miny + rad - vec[a][0]);
			}
			glVertex2f(minx + rad, miny);
		}
		else {
			glVertex2f(minx, miny);
		}

		glEnd();
	}


	void BIL::Drawable::DrawBoxShade (int mode,
									  float minx,
									  float miny,
									  float maxx,
									  float maxy,
									  float rad,
									  float shadetop,
									  float shadedown)
	{
		float vec[7][2] = { {0.195, 0.02},
							{0.383, 0.067},
							{0.55, 0.169},
							{0.707, 0.293},
							{0.831, 0.45},
							{0.924, 0.617},
							{0.98, 0.805} };
		const float div = maxy - miny;
		const float idiv = 1.0f / div;
		float coltop[3], coldown[3], color[4];
		int a;

		/* mult */
		for (a = 0; a < 7; a++) {
			mul_v2_fl(vec[a], rad);
		}

		/* get current color, needs to be outside of glBegin/End */
		glGetFloatv(GL_CURRENT_COLOR, color);

		/* 'shade' defines strength of shading */
		coltop[0]  = std::min(1.0f, color[0] + shadetop);
		coltop[1]  = std::min(1.0f, color[1] + shadetop);
		coltop[2]  = std::min(1.0f, color[2] + shadetop);
		coldown[0] = std::max(0.0f, color[0] + shadedown);
		coldown[1] = std::max(0.0f, color[1] + shadedown);
		coldown[2] = std::max(0.0f, color[2] + shadedown);

		glShadeModel(GL_SMOOTH);
		glBegin(mode);

		/* start with corner right-bottom */
		if (round_box_type_ & RoundBoxBottomRight) {

			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(maxx - rad, miny);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(coltop, coldown, vec[a][1] * idiv);
				glVertex2f(maxx - rad + vec[a][0], miny + vec[a][1]);
			}

			round_box_shade_col(coltop, coldown, rad * idiv);
			glVertex2f(maxx, miny + rad);
		}
		else {
			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(maxx, miny);
		}

		/* corner right-top */
		if (round_box_type_ & RoundBoxTopRight) {

			round_box_shade_col(coltop, coldown, (div - rad) * idiv);
			glVertex2f(maxx, maxy - rad);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(coltop, coldown,
									(div - rad + vec[a][1]) * idiv);
				glVertex2f(maxx - vec[a][1], maxy - rad + vec[a][0]);
			}
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(maxx - rad, maxy);
		}
		else {
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(maxx, maxy);
		}

		/* corner left-top */
		if (round_box_type_ & RoundBoxTopLeft) {

			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(minx + rad, maxy);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(coltop, coldown, (div - vec[a][1]) * idiv);
				glVertex2f(minx + rad - vec[a][0], maxy - vec[a][1]);
			}

			round_box_shade_col(coltop, coldown, (div - rad) * idiv);
			glVertex2f(minx, maxy - rad);
		}
		else {
			round_box_shade_col(coltop, coldown, 1.0);
			glVertex2f(minx, maxy);
		}

		/* corner left-bottom */
		if (round_box_type_ & RoundBoxBottomLeft) {

			round_box_shade_col(coltop, coldown, rad * idiv);
			glVertex2f(minx, miny + rad);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(coltop, coldown, (rad - vec[a][1]) * idiv);
				glVertex2f(minx + vec[a][1], miny + rad - vec[a][0]);
			}

			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(minx + rad, miny);
		}
		else {
			round_box_shade_col(coltop, coldown, 0.0);
			glVertex2f(minx, miny);
		}

		glEnd();
		glShadeModel(GL_FLAT);
	}

	/* plain antialiased unfilled rectangle */
	void Drawable::DrawRoundRect(float minx,
								 float miny, float maxx, float maxy, float rad)
	{
		float color[4];

		if (round_box_type_ & UI_RB_ALPHA) {
			glGetFloatv(GL_CURRENT_COLOR, color);
			color[3] = 0.5;
			glColor4fv(color);
			glEnable(GL_BLEND);
		}

		/* set antialias line */
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);

		DrawBox(GL_LINE_LOOP, minx, miny, maxx, maxy, rad);

		glDisable(GL_BLEND);
		glDisable(GL_LINE_SMOOTH);
	}

	void Drawable::DrawBoxShadow(unsigned char alpha,
								 float minx, float miny, float maxx, float maxy)
	{
		/* accumulated outline boxes to make shade not linear, is more
		   pleasant */
		DrawShadowBox(minx, miny, maxx, maxy, 11.0, (20 * alpha) >> 8);
		DrawShadowBox(minx, miny, maxx, maxy, 7.0, (40 * alpha) >> 8);
		DrawShadowBox(minx, miny, maxx, maxy, 5.0, (80 * alpha) >> 8);
	}

	void Drawable::DrawShadowBox(float minx, float miny,
								 float maxx, float maxy,
								 float shadsize, unsigned char alpha)
	{
		glEnable(GL_BLEND);
		glShadeModel(GL_SMOOTH);

		/* right quad */
		glBegin(GL_POLYGON);
		glColor4ub(0, 0, 0, alpha);
		glVertex2f(maxx, miny);
		glVertex2f(maxx, maxy - 0.3f * shadsize);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(maxx + shadsize, maxy - 0.75f * shadsize);
		glVertex2f(maxx + shadsize, miny);
		glEnd();

		/* corner shape */
		glBegin(GL_POLYGON);
		glColor4ub(0, 0, 0, alpha);
		glVertex2f(maxx, miny);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(maxx + shadsize, miny);
		glVertex2f(maxx + 0.7f * shadsize, miny - 0.7f * shadsize);
		glVertex2f(maxx, miny - shadsize);
		glEnd();

		/* bottom quad */
		glBegin(GL_POLYGON);
		glColor4ub(0, 0, 0, alpha);
		glVertex2f(minx + 0.3f * shadsize, miny);
		glVertex2f(maxx, miny);
		glColor4ub(0, 0, 0, 0);
		glVertex2f(maxx, miny - shadsize);
		glVertex2f(minx + 0.5f * shadsize, miny - shadsize);
		glEnd();

		glDisable(GL_BLEND);
		glShadeModel(GL_FLAT);
	}

	void Drawable::DrawAntiRoundbox(int mode,
									float minx, float miny, float maxx,
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
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			DrawBox(mode, minx, miny, maxx, maxy, rad);
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}

		glDisable(GL_BLEND);
	}

	void Drawable::DrawBoxVerticalShade(int mode,
										float minx, float miny,
										float maxx, float maxy,
										float rad, float shadeLeft,
										float shadeRight)
	{
		float vec[7][2] = {
			{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
			{0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
		const float div = maxx - minx;
		const float idiv = 1.0f / div;
		float colLeft[3], colRight[3], color[4];
		int a;

		/* mult */
		for (a = 0; a < 7; a++) {
			mul_v2_fl(vec[a], rad);
		}
		/* get current color, needs to be outside of glBegin/End */
		glGetFloatv(GL_CURRENT_COLOR, color);

		/* 'shade' defines strength of shading */
		colLeft[0]  = std::min(1.0f, color[0] + shadeLeft);
		colLeft[1]  = std::min(1.0f, color[1] + shadeLeft);
		colLeft[2]  = std::min(1.0f, color[2] + shadeLeft);
		colRight[0] = std::max(0.0f, color[0] + shadeRight);
		colRight[1] = std::max(0.0f, color[1] + shadeRight);
		colRight[2] = std::max(0.0f, color[2] + shadeRight);

		glShadeModel(GL_SMOOTH);
		glBegin(mode);

		/* start with corner right-bottom */
		if (round_box_type_ & RoundBoxBottomRight) {
			round_box_shade_col(colLeft, colRight, 0.0);
			glVertex2f(maxx - rad, miny);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(colLeft, colRight, vec[a][0] * idiv);
				glVertex2f(maxx - rad + vec[a][0], miny + vec[a][1]);
			}

			round_box_shade_col(colLeft, colRight, rad * idiv);
			glVertex2f(maxx, miny + rad);
		}
		else {
			round_box_shade_col(colLeft, colRight, 0.0);
			glVertex2f(maxx, miny);
		}

		/* corner right-top */
		if (round_box_type_ & RoundBoxTopRight) {
			round_box_shade_col(colLeft, colRight, 0.0);
			glVertex2f(maxx, maxy - rad);

			for (a = 0; a < 7; a++) {

				round_box_shade_col(colLeft, colRight,
									(div - rad - vec[a][0]) * idiv);
				glVertex2f(maxx - vec[a][1], maxy - rad + vec[a][0]);
			}
			round_box_shade_col(colLeft, colRight, (div - rad) * idiv);
			glVertex2f(maxx - rad, maxy);
		}
		else {
			round_box_shade_col(colLeft, colRight, 0.0);
			glVertex2f(maxx, maxy);
		}

		/* corner left-top */
		if (round_box_type_ & RoundBoxTopLeft) {
			round_box_shade_col(colLeft, colRight, (div - rad) * idiv);
			glVertex2f(minx + rad, maxy);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(colLeft, colRight,
									(div - rad + vec[a][0]) * idiv);
				glVertex2f(minx + rad - vec[a][0], maxy - vec[a][1]);
			}

			round_box_shade_col(colLeft, colRight, 1.0);
			glVertex2f(minx, maxy - rad);
		}
		else {
			round_box_shade_col(colLeft, colRight, 1.0);
			glVertex2f(minx, maxy);
		}

		/* corner left-bottom */
		if (round_box_type_ & RoundBoxBottomLeft) {
			round_box_shade_col(colLeft, colRight, 1.0);
			glVertex2f(minx, miny + rad);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(colLeft, colRight, (vec[a][0]) * idiv);
				glVertex2f(minx + vec[a][1], miny + rad - vec[a][0]);
			}

			round_box_shade_col(colLeft, colRight, 1.0);
			glVertex2f(minx + rad, miny);
		}
		else {
			round_box_shade_col(colLeft, colRight, 1.0);
			glVertex2f(minx, miny);
		}

		glEnd();
		glShadeModel(GL_FLAT);
	}

	void Drawable::DrawScroll(const WidgetColors& wcol, const Recti& rect, const Recti& slider, ScrollState state)
	{

	}

} /* namespace BIL */
