/*
 * Position.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_POSITION_H_
#define _BIL_POSITION_H_

#include <GL/gl.h>

namespace BIL {

	template<typename T>
	class Point2D
	{
	public:
		Point2D ()
				: _x(T()), _y(T())
		{
		}

		Point2D (T x, T y)
				: _x(x), _y(y)
		{
		}

		Point2D<T> (const Point2D<T>& orig)
				: _x(T()), _y(T())
		{
			_x = orig._x;
			_y = orig._y;
		}

		virtual ~Point2D ()
		{
		}

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

	template<typename T>
	class Point3D
	{
	public:
		Point3D ()
				: _x(T()), _y(T()), _z(T())
		{
		}

		Point3D (T x, T y, T z)
				: _x(x), _y(y), _z(z)
		{
		}

		Point3D<T> (const Point3D<T>& orig)
				: _x(T()), _y(T()), _z(T())
		{
			_x = orig._x;
			_y = orig._y;
			_z = orig._z;
		}

		virtual ~Point3D ()
		{
		}

		Point3D<T>& operator = (const Point3D<T>& orig)
		{
			_x = orig._x;
			_y = orig._y;
			_z = orig._z;
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

		T getZ (void) const
		{
			return _z;
		}

		void setZ (T z)
		{
			_z = z;
		}

	private:
		T _x;
		T _y;
		T _z;
	};

	typedef Point2D<GLint> Point2Di;
	typedef Point2D<GLfloat> Point2Df;
	typedef Point2D<GLdouble> Point2Dd;
	typedef Point2D<GLuint> Point2Dui;

	typedef Point3D<GLint> Point3Di;
	typedef Point3D<GLfloat> Point3Df;
	typedef Point3D<GLdouble> Point3Dd;
	typedef Point3D<GLuint> Point3Dui;

} /* namespace BIL */
#endif /* _BIL_POSITION_H_ */
