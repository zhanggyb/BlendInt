/*
 * Position.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_POSITION_H_
#define _BIL_POSITION_H_

namespace BIL {

	template<typename T>
	class Point2D
	{
	public:
		Point2D ()
				: _x(T()), _y(T())
		{
		}

		Point2D<T> (const Point2D<T>& orig)
				: _x(T()), _y(T())
		{
			_x = orig._x;
			_y = orig._y;
		}

		virtual ~Point2D() {}

		Point2D<T>& operator = (const Point2D<T>& orig)
		{
			_x = orig._x;
			_y = orig._y;
			return *this;
		}

		T getX (void) const
		{
			return _x;
		}

		void setX (T x)
		{
			_x = x;
		}

		T getY (void) const
		{
			return _y;
		}

		void setY (T y)
		{
			_y = y;
		}

	private:
		T _x;
		T _y;
	};

} /* namespace BIL */
#endif /* _BIL_POSITION_H_ */
