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

#ifndef _BLENDINT_GUI_BEZIER_HPP_
#define _BLENDINT_GUI_BEZIER_HPP_

#include <vector>
#include <glm/glm.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	enum BezierType {

		LINEAR_BEZIER = 2,
		QUADRATIC_BEZIER = 3,
		CUBIC_BEZIER = 4

	};

	class Bezier: public Object
	{
	public:

		Bezier (int n = 2)
		: Object(), vao_(0)
		{
			control_points_.resize(n, glm::vec2());
		}

		virtual ~Bezier ()
		{

		}

		const glm::vec2& operator [] (size_t n) const
		{
			return control_points_[n];
		}

		void assign (size_t n, const glm::vec2& point)
		{
			if(n < control_points_.size()) {

				if(control_points_[n] != point) {

					control_points_[n] = point;
					UpdateBuffer();

				}

			}
		}

		void assign (size_t n, float x, float y)
		{
			if(n < control_points_.size()) {

				if((control_points_[n].x != x) ||
						(control_points_[n].y != y)) {

					control_points_[n].x = x;
					control_points_[n].y = y;
					UpdateBuffer();

				}

			}
		}

		void append (const glm::vec2& point)
		{
			control_points_.push_back(point);
			UpdateBuffer();
		}

		void append (float x, float y)
		{
			control_points_.push_back(glm::vec2(x, y));
			UpdateBuffer();
		}

		void Draw ();

	private:

		void UpdateBuffer ();

		GLuint vao_;

		std::vector<glm::vec2> control_points_;

		GLBuffer<> buffer_;

	};

}

#endif /* _BLENDINT_GUI_BEZIER_HPP_ */
