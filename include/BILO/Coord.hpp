/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.	If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_COORD_HPP_
#define _BIL_COORD_HPP_

#include <algorithm>

namespace BILO {

	template<typename T>
	class Coord2
	{
	public:
		Coord2 ()
			: x_(T(0)), y_(T(0))
		{
		}

		Coord2 (T x, T y)
			: x_(x), y_(y)
		{
		}

		Coord2 (const Coord2& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
		}

		Coord2& operator = (const Coord2& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			return *this;
		}

		Coord2& operator + (const Coord2& other)
		{
			x_ += other.x_;
			y_ += other.y_;
			return *this;
		}

		void Add (const Coord2& other)
		{
			x_ += other.x_;
			y_ += other.y_;
		}

		void Add (const T& x, const T& y)
		{
			x_ += x;
			y_ += y;
		}

		T x () const {return x_;}

		void set_x (T x) {x_ = x;}

		T y () const {return y_;}

		void set_y (T y) {y_ = y;}

	private:
		T x_;
		T y_;
	};

	template<typename T>
	class Coord3
	{
	public:
		Coord3 ()
			: x_(T(0)), y_(T(0)), z_(T(0))
		{
		}

		Coord3 (T x, T y, T z)
			: x_(x), y_(y), z_(z)
		{
		}

		Coord3 (const Coord3& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			z_ = orig.z_;
		}

		Coord3& operator = (const Coord3& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			z_ = orig.z_;
			return *this;
		}

		Coord3& operator + (const Coord3& other)
		{
			x_ += other.x_;
			y_ += other.y_;
			z_ += other.z_;
			return *this;
		}

		void Add (const Coord3& other)
		{
			x_ += other.x_;
			y_ += other.y_;
			z_ += other.z_;
		}

		void Add (const T& x, const T& y, const T& z)
		{
			x_ += x;
			y_ += y;
			z_ += z;
		}

		T x () const {return x_;}

		void set_x (T x) {x_ = x;}

		T y () const {return y_;}

		void set_y (T y) {y_ = y;}

		T z () const {return z_;}

		void set_z (T z) {z_ = z;}

	private:
		T x_;
		T y_;
		T z_;
	};

	template <typename T>
	bool operator == (const Coord2<T>& pos1, const Coord2<T>& pos2)
	{
		return (std::abs(pos1.x() - pos2.x()) < 0.0000001) &&
				std::abs((pos1.y() - pos2.y()) < 0.0000001);
	}

	template <typename T>
	bool operator == (const Coord3<T>& pos1, const Coord3<T>& pos2)
	{
		return (std::abs(pos1.x() - pos2.x()) < 0.0000001) &&
				std::abs((pos1.y() - pos2.y()) < 0.0000001) &&
				std::abs((pos1.z() - pos2.z()) < 0.0000001);
	}

	typedef Coord2<int> Coord2i;
	typedef Coord2<float> Coord2f;

	typedef Coord3<int> Coord3i;
	typedef Coord3<float> Coord3f;

}

#endif /* _BIL_COORD_HPP_ */
