/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

// if use C++ 11, include <array>
#include <stddef.h>

#include <BIL/BasicObject.h>
#include <BIL/Font.h>
#include <BIL/Tuple.h>

namespace BIL {

	class Drawable: public BIL::BasicObject
	{
	public:
		Drawable (BasicObject* parent = NULL);

		virtual ~Drawable ();

		const Vec2ui& getSize (void) const
		{
			return _size;
		}

		void resize (unsigned int w, unsigned int h)
		{
			resize(Vec2ui(w, h));
		}

		void resize (const Vec2ui& size);

		const Coord3i& getPos (void) const
		{
			return _pos;
		}

		void setPos (const Coord2i& pos)
		{
			_pos = Coord3i(pos.coord.x, pos.coord.y, 0);
		}

		void setPos (const Coord3i& pos)
		{
			_pos = pos;
		}

		virtual void refresh (void)
		{
			render();
		}

		const Vec4i& getMargin (void) const
		{
			return _margin;
		}

		void setMargin (const Vec4i& margin)
		{
			_margin = margin;
		}

		const Vec4i& getPadding (void) const
		{
			return _padding;
		}

		void setPadding (const Vec4i& padding)
		{
			_padding = padding;
		}

		const Font& getFont (void) const {return _font;}

	protected:

		virtual void render (void) = 0;

		Font _font;

		Vec2ui _size;

		Coord3i _pos;

		Vec4i _padding; /** used when in Layout */

		Vec4i _margin; /** used when in Layout */

	private:

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);

	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
