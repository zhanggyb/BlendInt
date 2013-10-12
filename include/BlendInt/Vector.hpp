/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_VECTOR_HPP_
#define _BLENDINT_VECTOR_HPP_

namespace BlendInt {

	template <typename T>
	class Vector2
	{
	public:
		
		Vector2 ()
			: x_(T(0)), y_(T(0))
		{}

		Vector2 (const T& x, const T& y)
		: x_(x), y_(y)
		{}

		Vector2 (const T val[2])
			: x_(val[0]), y_(val[1])
		{
		}

		Vector2 (const Vector2& orig)
			: x_(orig.x_), y_(orig.y_)
		{
		}

		const T& operator [] (int index) const
		{
			if (index == 0) return x_;
			if (index == 1) return y_;

			return T(0);
		}

		T& operator [] (int index)
		{
			if (index <= 0) return x_;

			return y_;
		}

		Vector2& operator = (const T val[2])
		{
			x_ = val[0];
			y_ = val[1];
			return *this;
		}

		Vector2& operator = (const Vector2& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			return *this;
		}

		bool operator == (const Vector2& dist)
		{
			return x_ == dist.x_ && y_ == dist.y_;
		}

		const T& x (void) const {
			return x_;
		}

		void set_x (T x) {x_ = x;}

		const T& y (void) const {
			return y_;
		}

		void set_y (T y) {y_ = y;}

	private:
		
		T x_;
		T y_;
	};

	template <typename T>
	class Vector3
	{
	public:

		Vector3 ()
		: x_(T(0)), y_(T(0)), z_(T(0))
		{}

		Vector3 (const T& x, const T& y, const T& z)
		: x_(x), y_(y), z_(z)
		{}

		Vector3 (const T val[3])
		: x_(val[0]), y_(val[1]), z_(val[2])
		{}

		Vector3 (const Vector3& orig)
		: x_(orig.x_), y_(orig.y_), z_(orig.z_)
		{}

		const T& operator [] (int index) const
		{
			if (index == 0) return x_;
			if (index == 1) return y_;
			if (index == 2) return z_;

			return T(0);
		}

		T& operator [] (int index)
		{
			if (index <= 0) return x_;
			if (index == 1) return y_;

			return z_;
		}

		Vector3& operator = (const T val[3])
		{
			x_ = val[0];
			y_ = val[1];
			z_ = val[2];
			return *this;
		}

		Vector3& operator = (const Vector3& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			z_ = orig.z_;
			return *this;
		}

		bool operator == (const Vector3& dist)
		{
			return x_ == dist.x_ && y_ == dist.y_ && z_ == dist.z_;
		}

		const T& x (void) const {
			return x_;
		}

		void set_x (T x) {x_ = x;}

		const T& y (void) const {
			return y_;
		}

		void set_y (T y) {y_ = y;}

		const T& z (void) const {
			return z_;
		}

		void set_z (T z) {z_ = z;}

	private:

		T x_;
		T y_;
		T z_;
	};

	template <typename T>
	class Vector4
	{
	public:

		Vector4 ()
		: x_(T(0)), y_(T(0)), z_(T(0)), w_(T(0))
		{}

		Vector4 (const T& x, const T& y, const T& z, const T& w)
		: x_(x), y_(y), z_(z), w_(w)
		{}

		Vector4 (const T val[4])
		: x_(val[0]), y_(val[1]), z_(val[2]), w_(val[3])
		{}

		Vector4 (const Vector4& orig)
		: x_(orig.x_), y_(orig.y_), z_(orig.z_), w_(orig.w_)
		{}

		const T& operator [] (int index) const
		{
			if (index == 0) return x_;
			if (index == 1) return y_;
			if (index == 2) return z_;
			if (index == 3) return w_;

			return T(0);
		}

		T& operator [] (int index)
		{
			if (index <= 0) return x_;
			if (index == 1) return y_;
			if (index == 2) return z_;

			return w_;
		}

		Vector4& operator = (const T val[4])
		{
			x_ = val[0];
			y_ = val[1];
			z_ = val[2];
			w_ = val[3];

			return *this;
		}

		Vector4& operator = (const Vector4& orig)
		{
			x_ = orig.x_;
			y_ = orig.y_;
			z_ = orig.z_;
			w_ = orig.w_;
			return *this;
		}

		bool operator == (const Vector4& dist)
		{
			return x_ == dist.x_ && y_ == dist.y_
					&& z_ == dist.z_ && w_ == dist.w_;
		}

		const T& x (void) const {
			return x_;
		}

		void set_x (T x) {x_ = x;}

		const T& y (void) const {
			return y_;
		}

		void set_y (T y) {y_ = y;}

		const T& z (void) const {
			return z_;
		}

		void set_z (T z) {z_ = z;}

		const T& w (void) const {
			return w_;
		}

		void set_w (T w) {w_ = w;}

	private:

		T x_;
		T y_;
		T z_;
		T w_;
	};

	typedef Vector2<int> Vector2i;
	typedef Vector3<int> Vector3i;
	typedef Vector4<int> Vector4i;

	typedef Vector2<float> Vector2f;
	typedef Vector3<float> Vector3f;
	typedef Vector4<float> Vector4f;

}

#endif	// _BIL_VECTOR_HPP_
