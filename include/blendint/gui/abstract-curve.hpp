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

#ifndef _BLENDINT_GUI_ABSTRACTCURVE_HPP_
#define _BLENDINT_GUI_ABSTRACTCURVE_HPP_

#include <deque>
#include <iterator>
#include <glm/glm.hpp>

#include <blendint/core/object.hpp>

namespace BlendInt {

	/**
	 * @brief Abstract class for 2D curve
	 */
	class AbstractCurve: public Object
	{
	public:

		AbstractCurve ()
		{
		}

		virtual ~AbstractCurve ()
		{
		}

		inline void push_back(const glm::vec2& point)
		{
			points_.push_back(point);
		}

		inline void push_back(float x, float y)
		{
			points_.push_back(glm::vec2(x, y));
		}

		inline void push_front (const glm::vec2& point)
		{
			points_.push_front(point);
		}

		inline void push_front (float x, float y)
		{
			points_.push_front(glm::vec2(x, y));
		}

		inline void insert (int index, const glm::vec2& point)
		{
			std::deque<glm::vec2>::iterator it = points_.begin();
			std::advance(it, index);

			points_.insert(it, point);
		}

		inline glm::vec2& operator [] (int i)
		{
			return points_[i];
		}

		inline const glm::vec2& operator [] (int i) const
		{
			return points_[i];
		}

		inline size_t count () const
		{
			return points_.size();
		}

		/**
		 * @brief Upload to GPU buffer
		 */
		virtual void Unpack () = 0;

		virtual void Draw () = 0;

	private:

		std::deque<glm::vec2> points_;
	};

}

#endif /* _BLENDINT_GUI_ABSTRACTCURVE_HPP_ */
