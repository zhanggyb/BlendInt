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
		// enums
	public:

		/**
		 * flags to set which corners will become rounded:
		 *
		 * 1------2
		 * |      		|
		 * 8------4
		 */
		enum CornerPosition
		{
			CORNER_TOP_LEFT = (1 << 0),
			CORNER_TOP_RIGHT = (1 << 1),
			CORNER_BOTTOM_RIGHT = (1 << 2),
			CORNER_BOTTOM_LEFT = (1 << 3),
			/* just for convenience */
			CORNER_NONE = 0,
			CORNER_ALL = (CORNER_TOP_LEFT | CORNER_TOP_RIGHT
			        | CORNER_BOTTOM_RIGHT | CORNER_BOTTOM_LEFT)
		};

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

		const Font& getFont (void) const
		{
			return _font;
		}

		virtual void render (void) = 0;

	protected:

		/**
		 * @brief Draw a box shape
		 * @param mode GLenum in glBegin
		 * @param minx
		 * @param miny
		 * @param maxx
		 * @param maxy
		 * @param rad Corner radius, this will multiple the pre-defined vecter inside this function
		 */
		void drawBox (int mode, float minx, float miny, float maxx, float maxy,
		        float rad);

		void drawBoxShade (int mode, float minx, float miny, float maxx,
		        float maxy, float rad, float shadetop, float shadedown);

	protected:
		// member variables

		Font _font;

		Vec2ui _size;

		Coord3i _pos;

		Vec4i _padding; /** used when in Layout */

		Vec4i _margin; /** used when in Layout */

		CornerPosition _corner;

	private:

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);

	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
