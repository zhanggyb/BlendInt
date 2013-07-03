/*
 * Size.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_SIZE_H_
#define _BIL_SIZE_H_

namespace BIL {

	class Size2i
	{
	public:

		Size2i ()
				: _width(0), _height(0)
		{
		}

		Size2i (unsigned int w, unsigned int h)
				: _width(w), _height(h)
		{
		}

		Size2i (const Size2i& orig)
		{
			_width = orig.getWidth();
			_height = orig.getHeight();
		}

		Size2i & operator = (const Size2i& orig);

		virtual ~Size2i ();

		void setHeight (unsigned int height)
		{
			this->_height = height;
		}

		int getHeight (void) const
		{
			return _height;
		}

		void setWidth (unsigned int width)
		{
			this->_width = width;
		}

		int getWidth (void) const
		{
			return _width;
		}
	private:

		unsigned int _width;
		unsigned int _height;
	};

} /* namespace BIL */
#endif /* _BIL_SIZE_H_ */
