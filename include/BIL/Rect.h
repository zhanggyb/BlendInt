/*
 * Rect.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_RECT_H_
#define _BIL_RECT_H_

namespace BIL {

	template<typename T>
	class Rect
	{
	public:
		Rect ()
				: _left(T()), _right(T()), _top(T()), _bottom(T())
		{	}

		Rect<T> (const Rect<T>& orig)
		: _left(T()), _right(T()), _top(T()), _bottom(T())
		{
			_left = orig._left;
			_right = orig._right;
			_top = orig._top;
			_bottom = orig._bottom;
		}

		Rect<T>& operator = (const Rect<T>& orig)
		{
			_left = orig._left;
			_right = orig._right;
			_top = orig._top;
			_bottom = orig._bottom;
			return *this;
		}

		virtual ~Rect() {}

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

} /* namespace BIL */
#endif /* _BIL_RECT_H_ */
