/*
 * Size.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_SIZE_H_
#define _BIL_SIZE_H_

#include <GL/gl.h>

namespace BIL {

	template<typename T>
	class Size2D
	{
	public:

		Size2D ()
				: _width(T()), _height(T())
		{
		}

		Size2D<T> (T w, T h)
				: _width(w), _height(h)
		{
		}

		Size2D<T> (const Size2D<T>& orig)
				: _width(T()), _height(T())
		{
		}

		Size2D<T>& operator = (const Size2D<T>& orig)
		{
			_width = orig._width;
			_height = orig._height;
			return *this;
		}

		virtual ~Size2D ()
		{
		}

		T getHeight (void) const
		{
			return _height;
		}

		void setHeight (T height)
		{
			_height = height;
		}

		T getWidth (void) const
		{
			return _width;
		}

		void setWidth (T width)
		{
			_width = width;
		}

	private:
		T _width;
		T _height;
	};

	typedef Size2D<GLint> Size2Di;
	typedef Size2D<GLfloat> Size2Df;
	typedef Size2D<GLdouble> Size2Dd;
	typedef Size2D<GLuint> Size2Dui;

} /* namespace BIL */
#endif /* _BIL_SIZE_H_ */
