/*
 * Size.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef SIZE_H_
#define SIZE_H_

namespace BIL {

	class Size
	{
	public:

		Size ()
				: _width(0), _height(0)
		{
		}

		Size (int w, int h)
				: _width(w), _height(h)
		{
		}

		Size (const Size& orig)
		{
			_width = orig.getWidth();
			_height = orig.getHeight();
		}

		Size & operator = (const Size& orig);

		virtual ~Size ();

		void setHeight (int height)
		{
			this->_height = height;
		}

		int getHeight (void) const
		{
			return _height;
		}

		void setWidth (int width)
		{
			this->_width = width;
		}

		int getWidth (void) const
		{
			return _width;
		}
	private:

		int _width;
		int _height;
	};

} /* namespace BIL */
#endif /* SIZE_H_ */
