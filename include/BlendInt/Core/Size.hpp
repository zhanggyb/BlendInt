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

#ifndef _BLENDINT_SIZE_HPP_
#define _BLENDINT_SIZE_HPP_

#include <BlendInt/Core/Point.hpp>

namespace BlendInt {

	/**
	 * @brief Size
	 *
	 * only used in widgets/windows to define its size
	 */
	class Size
	{
	public:

		Size ()
			: m_width(0), m_height(0)
		{

		}

		Size (unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{

		}

		Size (const Size& orig)
			: m_width(orig.m_width), m_height(orig.m_height)
		{

		}

		Size& operator = (const Size& orig)
		{
			m_width = orig.m_width;
			m_height = orig.m_height;
			return *this;
		}

		bool is_valid () const
		{
			return m_width > 0 && m_height > 0;
		}

		inline bool equal (const Size& size)
		{
			return (m_width == size.width() && m_height == size.height());
		}

		inline bool equal (unsigned int width, unsigned int height)
		{
			return (m_width == width && m_height == height);
		}

		bool contain (const Point& pos)
		{
			if (pos.x() < 0 || pos.y() < 0 ||
				pos.x() > static_cast<int>(m_width) || pos.y() > static_cast<int>(m_height))
			{
				return false;
			}
			
			return true;
		}

		bool contain (int x, int y)
		{
			if (x < 0 || y < 0 ||
				x > static_cast<int>(m_width) || y > static_cast<int>(m_height))
			{
				return false;
			}
			
			return true;
		}

		unsigned int width () const {return m_width;}

		void set_width (unsigned int width) {m_width = width;}

		void set_width (int width) {m_width = static_cast<unsigned int>(width);}

		unsigned int height () const {return m_height;}

		void set_height (unsigned int height) {m_height = height;}

		void set_height (int height) {m_height = static_cast<unsigned int>(height);}

		void add_width (int width)
		{
			m_width = m_width + width;
		}

		void add_height (int height)
		{
			m_height = m_height + height;
		}

		unsigned int& operator [] (int index)
		{
			if(index <= 0) return m_width;

			return m_height;
		}

	private:
		unsigned int m_width;
		unsigned int m_height;
	};


	/**
	 * @brief compare two size object and return true if they are equal
	 * @param size1
	 * @param size2
	 * @return
	 */
	extern inline bool operator == (const Size& size1, const Size& size2)
	{
		return ((size1.width() == size2.width())
		        && (size1.height() == size2.height()));
	}
}

#endif /* SIZE_HPP_ */
