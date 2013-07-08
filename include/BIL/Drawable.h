/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

// if use C++ 11, include <array>
#include <boost/array.hpp>
#include <stddef.h>

#include <BIL/BasicObject.h>
#include <BIL/Size.h>
#include <BIL/Point.h>
#include <BIL/Rect.h>

namespace BIL {

	typedef boost::array<GLint, 4> Array4i;

	class Drawable: public BIL::BasicObject
	{
	public:
		Drawable (BasicObject* parent = NULL);

		virtual ~Drawable ();

		const Size2Dui& getSize (void) const
		{
			return _size;
		}

		void resize (unsigned int w, unsigned int h)
		{
			resize(Size2Dui(w, h));
		}

		void resize (const Size2Dui& size);

		const Point3Di& getPos (void) const
		{
			return _pos;
		}

		void setPos (const Point2Di& pos)
		{
			_pos = Point3Di(pos.getX(), pos.getY(), 0);
			render ();
		}

		void setPos (const Point3Di& pos)
		{
			_pos = pos;
			render();
		}

		virtual void refresh (void)
		{
			render();
		}

		const Array4i& getMargin (void) const
		{
			return _margin;
		}

		void setMargin (GLint l, GLint r, GLint t, GLint b)
		{
			_margin[0] = l;
			_margin[1] = r;
			_margin[2] = t;
			_margin[3] = b;
		}

		void setMargin (const Array4i& margin)
		{
			_margin = margin;
		}

		const Array4i& getPadding (void) const
		{
			return _padding;
		}

		void setPadding (GLint l, GLint r, GLint t, GLint b)
		{
			_padding[0] = l;
			_padding[1] = r;
			_padding[2] = t;
			_padding[b] = b;
		}

		void setPadding (const Array4i& padding)
		{
			_padding = padding;
		}

	protected:

		virtual void render (void) = 0;

		Size2Dui _size;

		Point3Di _pos;

		Array4i _padding; /** used when in Layout */
		Array4i _margin; /** used when in Layout */

	private:

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);

	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
