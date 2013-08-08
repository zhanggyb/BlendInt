/*
 * Tuple
 *
 *  Created on: 2013年7月15日
 *      Author: zhanggyb
 */

#ifndef _BIL_TUPLE_
#define _BIL_TUPLE_

#include <string.h>

namespace BIL {

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

		Tuple4 (T x, T y, T z, T w = T(1.0))
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

	typedef Tuple2<int> Tuple2i, Coord2i, Vec2i;
	typedef Tuple3<int> Tuple3i, Coord3i, Vec3i;
	typedef Tuple4<int> Tuple4i, Vec4i;

	typedef Tuple2<unsigned int> Tuple2ui, Coord2ui, Vec2ui;

	typedef Tuple2<float> Tuple2f, Coord2f, Vec2f;
	typedef Tuple3<float> Tuple3f, Coord3f, Vec3f;
	typedef Tuple4<float> Tuple4f, Vec4f, Rectf, RGBAf;

	typedef Tuple2<long> Tuple2l, Coord2l, Vec2l;
	typedef Tuple3<long> Tuple3l, Coord3l, Vec3l;
	typedef Tuple4<long> Tuple4l, Coord4l, Vec4l;

}       // namespace of BIL

#endif /* TUPLE_ */
