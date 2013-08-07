/*
 * Drawable.cpp
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <algorithm>
#include <iostream>

#include <BIL/Drawable.h>

namespace BIL {

	using namespace std;

	static inline void mul_v2_fl(float r[2], float f)
	{
		r[0] *= f;
		r[1] *= f;
	}

	static inline float min_ff(float a, float b)
	{
		return (a < b) ? a : b;
	}

	static inline float max_ff(float a, float b)
	{
		return (a > b) ? a : b;
	}

	static void round_box_shade_col(const float col1[3], float const col2[3], const float fac)
	{
		float col[3];

		col[0] = (fac * col1[0] + (1.0f - fac) * col2[0]);
		col[1] = (fac * col1[1] + (1.0f - fac) * col2[1]);
		col[2] = (fac * col1[2] + (1.0f - fac) * col2[2]);
		glColor3fv(col);
	}

	Drawable::Drawable (BasicObject* parent)
			: BasicObject(parent), _corner (CORNER_NONE)
	{

	}

	Drawable::~Drawable ()
	{
		// delete all child object in list

	}

	void Drawable::resize (const Vec2ui& size)
	{
		_size = size;
		return;
	}

	void Drawable::drawBox(int mode, float minx, float miny, float maxx, float maxy, float rad)
	{
		float vec[7][2] = {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
		                   {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
		int a;

		/* mult */
		for (a = 0; a < 7; a++) {
			mul_v2_fl(vec[a], rad);
		}

		glBegin(mode);

		/* start with corner right-bottom */
		if (_corner & CORNER_BOTTOM_RIGHT) {
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
		if (_corner & CORNER_TOP_RIGHT) {
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
		if (_corner & CORNER_TOP_LEFT) {
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
		if (_corner & CORNER_BOTTOM_LEFT) {
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


	void BIL::Drawable::drawBoxShade (int mode, float minx, float miny, float maxx,
	        float maxy, float rad, float shadetop, float shadedown)
	{
		float vec[7][2] = {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
		                   {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
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
		coltop[0]  = min_ff(1.0f, color[0] + shadetop);
		coltop[1]  = min_ff(1.0f, color[1] + shadetop);
		coltop[2]  = min_ff(1.0f, color[2] + shadetop);
		coldown[0] = max_ff(0.0f, color[0] + shadedown);
		coldown[1] = max_ff(0.0f, color[1] + shadedown);
		coldown[2] = max_ff(0.0f, color[2] + shadedown);

		glShadeModel(GL_SMOOTH);
		glBegin(mode);

		/* start with corner right-bottom */
		if (_corner & CORNER_BOTTOM_RIGHT) {

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
		if (_corner & CORNER_TOP_RIGHT) {

			round_box_shade_col(coltop, coldown, (div - rad) * idiv);
			glVertex2f(maxx, maxy - rad);

			for (a = 0; a < 7; a++) {
				round_box_shade_col(coltop, coldown, (div - rad + vec[a][1]) * idiv);
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
		if (_corner & CORNER_TOP_LEFT) {

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
		if (_corner & CORNER_BOTTOM_LEFT) {

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

} /* namespace BIL */

