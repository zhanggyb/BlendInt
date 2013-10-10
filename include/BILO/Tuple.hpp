/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_TUPLE_HPP_
#define _BILO_TUPLE_HPP_

#include <stdint.h>
#include <string.h>

namespace BILO {

	template<typename T>
	union Tuple2
	{
		Tuple2 ()
		{
			memset(this, 0, sizeof(*this));
		}

		Tuple2 (T x, T y)
		{
			data[0] = x;
			data[1] = y;
		}

		Tuple2 (const Tuple2& orig)
		{
			memcpy(this, &orig, sizeof(*this));
		}

		Tuple2& operator = (const Tuple2& orig)
		{
			memcpy(this, &orig, sizeof(*this));
			return *this;
		}

		bool operator == (const Tuple2& orig)
		{
			return (memcmp(this, &orig, 2 * sizeof(T)) == 0);
		}

		T data[2]; /**< All compnents at once */

		struct
		{
			T x;
			T y;
		} coord;

		struct
		{
			T x;
			T y;
		} vec;

	};

	template<typename T>
	union Tuple3
	{
		Tuple3 ()
		{
			memset(this, 0, sizeof(*this));
		}

		Tuple3 (T x, T y, T z)
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
		}

		Tuple3 (const Tuple3& orig)
		{
			memcpy(this, &orig, sizeof(*this));
		}

		Tuple3& operator = (const Tuple3& orig)
		{
			memcpy(this, &orig, sizeof(*this));
			return *this;
		}

		bool operator == (const Tuple3& orig)
		{
			return (memcmp(this, &orig, 3 * sizeof(T)) == 0);
		}

		T data[3]; /**< All components at once */

		struct
		{
			T x; /**< Alias for first component */
			T y; /**< Alias for second component */
			T z; /**< Alias for third component */
		} coord;

		struct
		{
			T x;
			T y;
			T z;
		} vec;

		struct
		{
			T r;
			T g;
			T b;
		} rgb;

		struct
		{
			T red;
			T green;
			T blue;
		} color;
	};

	template<typename T>
	union Tuple4
	{
		Tuple4 ()
		{
			memset(this, 0, sizeof(*this));
		}

		/**
		 * @brief use this constructor only in RGBA
		 */
		Tuple4 (uint32_t value)
		{
			if (value > 0xFFFFFF) {
				data[3] = value & 0xFF;
				data[2] = (value >> 8) & 0xFF;
				data[1] = (value >> 16) & 0xFF;
				data[0] = (value >> 24) & 0xFF;
			} else {
				data[3] = 0xFF;
				data[2] = value & 0xFF;
				data[1] = (value >> 8) & 0xFF;
				data[0] = (value >> 16) & 0xFF;
			}
		}

		Tuple4 (T x, T y, T z, T w = T(1))
		{
			data[0] = x;
			data[1] = y;
			data[2] = z;
			data[3] = w;
		}

		Tuple4 (const Tuple4& orig)
		{
			memcpy(this, &orig, sizeof(*this));
		}

		Tuple4& operator = (const Tuple4& orig)
		{
			memcpy(this, &orig, sizeof(*this));
			return *this;
		}

		/**
		 * @brief use this assignement only in RGBA
		 */
		Tuple4& operator = (uint32_t value)
		{
			if (value > 0xFFFFFF) {
				data[3] = value & 0xFF;
				data[2] = (value >> 8) & 0xFF;
				data[1] = (value >> 16) & 0xFF;
				data[0] = (value >> 24) & 0xFF;
			} else {
				data[3] = 0xFF;
				data[2] = value & 0xFF;
				data[1] = (value >> 8) & 0xFF;
				data[0] = (value >> 16) & 0xFF;
			}
			
			return *this;
		}

		bool operator == (const Tuple4& orig)
		{
			return (memcmp(this, &orig, 4 * sizeof(T)) == 0);
		}

		T data[4]; /**< All components at once */

		struct
		{
			T x; /**< Alias for first component */
			T y; /**< Alias for second component */
			T z; /**< Alias for third component */
			T w; /**< Alias for fourth component */
		} coord;

		struct
		{
			T x;
			T y;
			T z;
			T w;
		} vec;

		struct
		{
			T x;
			T y;
			T w;
			T h;
		} rect;

		struct
		{
			T l;				/* left */
			T r;				/* right */
			T t;				/* top */
			T b;				/* bottom */
		} border;

		struct
		{
			T r;
			T g;
			T b;
			T a;
		} rgba;

		struct
		{
			T red;
			T green;
			T blue;
			T alpha;
		} color;

	};

	typedef Tuple4<unsigned char> RGBA;

	typedef Tuple2<int> Tuple2i;
	typedef Tuple3<int> Tuple3i;
	typedef Tuple4<int> Tuple4i;

	typedef Tuple2<unsigned int> Tuple2ui;

	typedef Tuple2<float> Tuple2f;
	typedef Tuple3<float> Tuple3f;
	typedef Tuple4<float> Tuple4f;

	typedef Tuple2<long> Tuple2l;
	typedef Tuple3<long> Tuple3l;
	typedef Tuple4<long> Tuple4l;

	typedef Tuple2<double> Tuple2d;

}	// namespace of BILO

#endif /* TUPLE_ */
