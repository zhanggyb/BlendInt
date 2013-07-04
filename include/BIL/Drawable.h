/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

#include <set>
#include <stddef.h>

#include <BIL/BasicObject.h>
#include <BIL/Size.h>
#include <BIL/Point.h>
#include <BIL/Rect.h>

namespace BIL {

	class Drawable: public BIL::BasicObject
	{
	public:
		Drawable (Drawable* parent = NULL);

		virtual ~Drawable ();

		const Size2Dui& getSize (void) const
		{
			return _size;
		}

		void resize (unsigned int w, unsigned int h)
		{
			resize (Size2Dui(w,h));
		}

		void resize (const Size2Dui& size);

		const Point2Di& getPos (void) const
		{
			return _pos;
		}

		void setPos (const Point2Di& pos)
		{
			_pos = pos;
			render();
		}

	protected:

		virtual void render (void) = 0;

		Size2Dui _size;

		Point2Di _pos;

		Rect2Di _padding;
		Rect2Di _margin;

	private:

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);

	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
