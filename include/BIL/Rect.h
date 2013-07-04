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
				: _left(T()), _right(T()), _top(T()), _bottom(T())
		{	}

		Rect2D (T l, T r, T t, T b)
				: _left(l), _right(r), _top(t), _bottom(b)
		{	}


		Rect2D<T> (const Rect2D<T>& orig)
		: _left(T()), _right(T()), _top(T()), _bottom(T())
		{
			_left = orig._left;
			_right = orig._right;
			_top = orig._top;
			_bottom = orig._bottom;
		}

		Rect2D<T>& operator = (const Rect2D<T>& orig)
		{
			_left = orig._left;
			_right = orig._right;
			_top = orig._top;
			_bottom = orig._bottom;
			return *this;
		}

		virtual ~Rect2D() {}

		T getBottom (void) const
		{
			return _bottom;
		}

		void setBottom (T bottom)
		{
			_bottom = bottom;
		}

		T getLeft (void) const
		{
			return _left;
		}

		void setLeft (T left)
		{
			_left = left;
		}

		T getRight (void) const
		{
			return _right;
		}

		void setRight (T right)
		{
			_right = right;
		}

		T getTop (void) const
		{
			return _top;
		}

		void setTop (T top)
		{
			_top = top;
		}

	private:
		T _left;
		T _right;
		T _top;
		T _bottom;
	};

	typedef Rect2D<GLint> Rect2Di;
	typedef Rect2D<GLfloat> Rect2Df;
	typedef Rect2D<GLdouble> Rect2Dd;
	typedef Rect2D<GLuint> Rect2Dui;

} /* namespace BIL */
#endif /* _BIL_RECT_H_ */
