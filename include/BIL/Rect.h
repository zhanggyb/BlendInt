/*
 * Rect.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_RECT_H_
#define _BIL_RECT_H_

#include <GL/gl.h>

namespace BIL {

	template<typename T>
	class Rect2D
	{
	public:
		Rect2D ()
				: _x(T()), _y(T()), _width(T()), _height(T())
		{
		}

		Rect2D (T l, T r, T w, T h)
				: _x(l), _y(r), _width(w), _height(h)
		{
		}

		Rect2D<T> (const Rect2D<T>& orig)
				: _x(T()), _y(T()), _width(T()), _height(T())
		{
			_x = orig._x;
			_y = orig._y;
			_width = orig._width;
			_height = orig._height;
		}

		Rect2D<T>& operator = (const Rect2D<T>& orig)
		{
			_x = orig._x;
			_y = orig._y;
			_width = orig._width;
			_height = orig._height;
			return *this;
		}

		virtual ~Rect2D ()
		{
		}

		T getHeight (void) const
		{
			return _height;
		}

		void setHeight (T h)
		{
			_height = h;
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

		T getWidth (void) const
		{
			return _width;
		}

		void setWidth (T w)
		{
			_width = w;
		}

	private:
		T _x;
		T _y;
		T _width;
		T _height;
	};

	typedef Rect2D<GLint> Rect2Di;
	typedef Rect2D<GLfloat> Rect2Df;
	typedef Rect2D<GLdouble> Rect2Dd;
	typedef Rect2D<GLuint> Rect2Dui;

} /* namespace BIL */
#endif /* _BIL_RECT_H_ */
