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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <cmath>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/CubicBezierCurve.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	CubicBezierCurve::CubicBezierCurve ()
	: AbstractCurve(),
	  vao_(0)
	{
		GLfloat vertices[] = {
				0.f, 0.f,	0.f,
				100.f, 100.f, 	0.f
		};

		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);
		buffer_.generate();
		buffer_.bind();

		buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_LINE_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_LINE_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	CubicBezierCurve::~CubicBezierCurve()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void CubicBezierCurve::Unpack()
	{
		// demo and test

		size_t n = 0;
		n = GetPointNumber(max_subdiv_count);
		DBG_PRINT_MSG("10, points: %ld", n);

		glm::vec2 p1(0.f, 0.f);
		glm::vec2 p2(100.f, 100.f);
		glm::vec2 p3(200.f, 100.f);
		glm::vec2 p4(300.f, 0.f);
		std::vector<GLfloat> verts(n * 3, 0.f);

		size_t index = 0;
		GenerateBezierCurveVertices(p1, p2, p3, p4, 0, &index, verts);

		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * verts.size(), &verts[0]);
		buffer_.reset();
	}

	void CubicBezierCurve::Draw()
	{
		Context::shaders->widget_line_program()->use();

		glUniform4f(Context::shaders->location(Shaders::WIDGET_LINE_COLOR), 1.f, 0.1f, 0.1f, 1.f);

		size_t n = GetPointNumber(max_subdiv_count);

		glBindVertexArray(vao_);
		glDrawArrays(GL_LINE_STRIP, 0, n);
		glBindVertexArray(0);

		GLSLProgram::reset();
	}

	/*
	 * procedure draw_curve(P1, P2, P3, P4, level)
	 * 	if (curve is flat)	# or if curve is flat
	 * 		draw_line(P1, P4)
	 * 	else
	 * 		L1 = P1
	 * 		L2 = midpoint(P1, P2)
	 * 		H  = midpoint(P2, P3)
	 * 		R3 = midpoint(P3, P4)
	 * 		R4 = P4
	 * 		L3 = midpoint(L2, H)
	 * 		R2 = midpoint(R3, H)
	 * 		L4 = midpoint(L3, R2)
	 * 		R1 = L4
	 * 		draw_curve(L1, L2, L3, L4, level + 1)
	 * 		draw_curve(R1, R2, R3, R4, level + 1)
	 * 	end procedure draw_curve
	 */
	void CubicBezierCurve::GenerateBezierCurveVertices(const glm::vec2& p1,
			const glm::vec2& p2,
			const glm::vec2& p3,
			const glm::vec2& p4,
			unsigned int level,
			size_t* index,
			std::vector<GLfloat>& vertices)
	{
		if(level > (max_subdiv_count - 1)) {

			// draw line from p1 to p4
			vertices[(*index) * 3 + 0] = p1.x;
			vertices[(*index) * 3 + 1] = p1.y;
			vertices[(*index + 1) * 3 + 0] = p4.x;
			vertices[(*index + 1) * 3 + 1] = p4.y;

			(*index)++;

			return;
		}

		// calculate all the mid-points of the line segments
		// L1 = p1
		glm::vec2 p12 = midpoint(p1, p2);	// L2
		glm::vec2 p23 = midpoint(p2, p3);	// H
		glm::vec2 p34 = midpoint(p3, p4);	// R3
		// R4 = p4
		glm::vec2 p1223 = midpoint(p12, p23);	// L3
		glm::vec2 p2334 = midpoint(p23, p34);	// R2
		glm::vec2 p = midpoint(p1223, p2334);	// L4
		// R1 = L4

		GenerateBezierCurveVertices(p1, p12, p1223, p, level + 1, index, vertices);
		GenerateBezierCurveVertices(p, p2334, p34, p4, level + 1, index, vertices);
	}

	size_t CubicBezierCurve::GetPointNumber(unsigned int level)
	{
		if(level == 0) return 2;

		int n = GetPointNumber(level - 1);
		return n * 2 - 1;
	}

	inline glm::vec2 CubicBezierCurve::midpoint(
			const glm::vec2& p1,
			const glm::vec2& p2)
	{
		return glm::vec2((p1.x + p2.x) / 2.f, (p1.y + p2.y) / 2.f);
	}

}
