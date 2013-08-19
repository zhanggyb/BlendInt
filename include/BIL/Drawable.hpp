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

#include <BIL/Traceable.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/EventHandler.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Rect.hpp>

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
		enum RoundBoxType
		{
			RoundBoxTopLeft = (1 << 0),
			RoundBoxTopRight = (1 << 1),
			RoundBoxBottomRight = (1 << 2),
			RoundBoxBottomLeft = (1 << 3),
			/* just for convenience */
			RoundBoxNone = 0,
			RoundBoxAll = (RoundBoxTopLeft | RoundBoxTopRight
			        | RoundBoxBottomRight | RoundBoxBottomLeft),
			UI_RB_ALPHA = RoundBoxAll + 1
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
		 * A drawable instance without a parent will be registered in
		 * a static list -- solo, it's usually a pop-up widget such as
		 * context menu, message box
		 */
		Drawable (Traceable* parent = NULL);

		virtual ~Drawable ();

		const Vec2ui& size (void) const
		{
			return size_;
		}

		void resize (unsigned int w, unsigned int h)
		{
			resize(Vec2ui(w, h));
		}

		void resize (const Vec2ui& size);

		const Coord3f& pos (void) const
		{
			return pos_;
		}

		void set_pos (float x, float y, float z)
		{
			pos_ = Coord3f(x, y, z);
		}

		void set_pos (const Coord2f& pos)
		{
			pos_ = Coord3f(pos.x(), pos.y(), 0.0);
		}

		void set_pos (const Coord3f& pos)
		{
			pos_ = pos;
		}

		const Vec4i& margin (void) const
		{
			return margin_;
		}

		void set_margin (const Vec4i& margin)
		{
			margin_ = margin;
		}

		const Vec4i& padding (void) const
		{
			return padding_;
		}

		void set_padding (const Vec4i& padding)
		{
			padding_ = padding;
		}

		void set_round_box_type (RoundBoxType type)
		{
			round_box_type_ = type;
		}

		RoundBoxType round_box_type (void)
		{
			return round_box_type_;
		}

		bool visible (void) const {return visible_;}

		void set_visible (bool visible) {visible_ = visible;}

		void Show (void)
		{
			visible_ = true;
		}

		void Hide (void)
		{
			visible_ = false;
		}

	public:	// virtual functions

		virtual void Render (void) = 0;

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
		void DrawBox (int mode,
					  float minx,
					  float miny,
					  float maxx,
					  float maxy,
					  float rad);

		void DrawBoxShade (int mode,
						   float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad,
						   float shadetop,
						   float shadedown);

		void DrawRoundRect(float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float rad);

		void DrawBoxShadow(unsigned char alpha,
						   float minx,
						   float miny,
						   float maxx,
						   float maxy);

		void DrawShadowBox(float minx,
						   float miny,
						   float maxx,
						   float maxy,
						   float shadsize,
						   unsigned char alpha);

		/* linear vertical shade within button or in outline */
		/* view2d scrollers use it */
		void DrawBoxVerticalShade(int mode,
								  float minx,
								  float miny,
								  float maxx,
								  float maxy,
								  float rad,
								  float shadeLeft,
								  float shadeRight);

		// (old, used in outliner) plain antialiased filled box
		void DrawAntiRoundbox(int mode,
							  float minx,
							  float miny,
							  float maxx,
							  float maxy,
							  float rad,
							  bool use_alpha);

		void DrawScroll (const WidgetColors& wcol,
				const Recti& rect,
				const Recti& slider,
				ScrollState state);

	protected:
		// member variables

		Vec2ui size_;

		Coord3f pos_;

		Vec4i padding_; /** used when in Layout */

		Vec4i margin_; /** used when in Layout */

		RoundBoxType round_box_type_;

		bool visible_;

	private:	// member function disabled

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);
	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
