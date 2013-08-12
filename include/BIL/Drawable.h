/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

/*
 * Drawable.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

#include <list>

#include <BIL/Traceable.h>
#include <BIL/Font.h>
#include <BIL/Tuple.h>
#include <BIL/EventHandler.h>
#include <BIL/Theme.h>

namespace BIL {

	class Drawable: public Traceable, public EventHandler
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
			        | CORNER_BOTTOM_RIGHT | CORNER_BOTTOM_LEFT),
			UI_RB_ALPHA = CORNER_ALL + 1
		};

		enum ScrollState
		{
			SCROLL_PRESSED = (1 << 0),
			SCROLL_ARROW = (1 << 1),
			SCROLL_NO_OUTLINE = (1 << 2)
		};

	public:

		/**
		 * @brief Default constructor
		 * @param parent
		 *
		 * A drawable instance without a parent will be registered in TBD
		 * it's usually a pop-up widget such as context menu, message box
		 */
		Drawable (Traceable* parent = NULL);

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

		const Coord3f& getPos (void) const
		{
			return _pos;
		}

		void setPos (float x, float y, float z)
		{
			_pos = Coord3f(x, y, z);
		}

		void setPos (const Coord2f& pos)
		{
			_pos = Coord3f(pos.coord.x, pos.coord.y, 0);
		}

		void setPos (const Coord3f& pos)
		{
			_pos = pos;
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

		void setRoundBox (CornerPosition type)
		{
			_corner = type;
		}

		CornerPosition getRoundBox (void)
		{
			return _corner;
		}

		void show (void)
		{
			_isVisible = true;
		}

		void hide (void)
		{
			_isVisible = false;
		}

	public:	// virtual functions

		virtual void render (void) = 0;

	protected:	// member functions

		void drawRoundBox (float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad);

		/**
		 * @brief Draw a box shape
		 * @param mode GLenum in glBegin
		 * @param minx
		 * @param miny
		 * @param maxx
		 * @param maxy
		 * @param rad Corner radius, this will multiple the pre-defined vecter
		 * inside this function
		 */
		void drawBox (int mode,
					  float minx,
					  float miny,
					  float maxx,
					  float maxy,
					  float rad);

		void drawBoxShade (int mode,
						   float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad,
						   float shadetop,
						   float shadedown);

		void drawRoundRect(float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad);

		void drawBoxShadow(unsigned char alpha,
						   float minx,
						   float miny,
						   float maxx,
						   float maxy);

		void drawShadowBox(float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float shadsize,
						   unsigned char alpha);

		/* linear vertical shade within button or in outline */
		/* view2d scrollers use it */
		void drawBoxVerticalShade(int mode,
								  float minx,
								  float miny,
								  float maxx,
								  float maxy,
								  float rad,
								  float shadeLeft,
								  float shadeRight);

		// (old, used in outliner) plain antialiased filled box
		void drawAntiRoundbox(int mode,
							  float minx,
							  float miny,
							  float maxx,
							  float maxy,
							  float rad,
							  bool use_alpha);

		void drawScroll (const WidgetColors& wcol,
				const Recti& rect,
				const Recti& slider,
				ScrollState state);

	protected:
		// member variables

		Font _font;

		Vec2ui _size;

		Coord3f _pos;

		Vec4i _padding; /** used when in Layout */

		Vec4i _margin; /** used when in Layout */

		CornerPosition _corner;

		bool _isVisible;

	private:	// member function disabled

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);
	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
