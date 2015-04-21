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

#ifndef _BLENDINT_GUI_CUBICBEZIERCURVE_HPP_
#define _BLENDINT_GUI_CUBICBEZIERCURVE_HPP_

#include <vector>

#include <blendint/gui/abstract-curve.hpp>
#include <blendint/opengl/gl-buffer.hpp>

namespace BlendInt {

	enum BezierType {

		LINEAR_BEZIER = 2,
		QUADRATIC_BEZIER = 3,
		CUBIC_BEZIER = 4

	};

	// 	TODO:
	// 		refer to
	//			http://www.antigrain.com/research/adaptive_bezier/index.html#PAGE_ADAPTIVE_BEZIER
	//		for better bezier shape

	/**
	 * @brief Class to manage and draw Bezie curve
	 *
	 * The curve is generated with De Casteljau's algorithm:
	 */
	class CubicBezierCurve: public AbstractCurve
	{
	public:

		CubicBezierCurve ();

		virtual ~CubicBezierCurve ();

		virtual void Unpack ();

		virtual void Draw ();

	private:

		/**
		 * @brief Generate vertex data for drawing the curve
		 * @param[in] p1 Control point 1
		 * @param[in] p2 Control point 2
		 * @param[in] p3 Control point 3
		 * @param[in] p4 Control point 4
		 * @param[in] level The recursion level of this function, MUST be 0, and will be increased in recursion
		 * @param[in/out] index The index where to start to set the x,y coords in vertices, in general is 0
		 * @param[in/out] vertices The vector to store the vertex data
		 *
		 * Usage of this function:
		 * @code
		 * size_t n = GetPointNumber(max_subdiv_count);
		 * std::vector<GLfloat> vertices(n * 2, 0.f);
		 * size_t index = 0;
		 *
		 * GenerateBezierCurveVertices(p1, p2, p3, p4, 0, &index, vertices);
		 * @endcode
		 *
		 * TODO: use http://www.antigrain.com/research/adaptive_bezier/index.html#PAGE_ADAPTIVE_BEZIER for better curve shape.
		 */
		void GenerateBezierCurveVertices (const glm::vec2& p1,
				const glm::vec2& p2,
				const glm::vec2& p3,
				const glm::vec2& p4,
				unsigned int level,
				size_t* index,
				std::vector<GLfloat>& vertices);

		size_t GetPointNumber (unsigned int level);

		inline glm::vec2 midpoint (const glm::vec2& p1, const glm::vec2& p2);

		GLuint vao_;

		GLBuffer<> buffer_;

		//static const float curve_distance_epsilon = 0.f;
		//static const float curve_collinearity_epsilon = 0.f;
		//static const float curve_angle_tolerance_epsilon = 0.01f;
		static const unsigned int max_subdiv_count = 8;

	};

}

#endif /* _BLENDINT_GUI_CUBICBEZIERCURVE_HPP_ */
