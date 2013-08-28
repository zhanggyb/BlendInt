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

#ifndef _BIL_DRAWABLE_H_
#define _BIL_DRAWABLE_H_

#include <list>

#include <BIL/Traceable.hpp>
#include <BIL/EventHandler.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Rect.hpp>
#include <BIL/Size.hpp>
#include <BIL/Point.hpp>
#include <BIL/Margin.hpp>
#include <BIL/Padding.hpp>

#include <BIL/Types.hpp>

namespace BIL {

	class Drawable: public Traceable, public EventHandler
	{
		// enums
	public:
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

		const Size& size () const
		{
			return size_;
		}

		void resize (int w, int h)
		{
			size_.set_width(w);
			size_.set_height(h);
			this->Update();
		}

		void resize (const Size& size)
		{
			size_ = size;
			this->Update();
		}

		const Point& pos () const
		{
			return pos_;
		}

		void set_pos (int x, int y)
		{
			pos_.set_x(x);
			pos_.set_y(y);
			this->Update();
		}

		void set_pos (const Point& pos)
		{
			pos_ = pos;
			this->Update();
		}

		float z () const
		{
			return z_;
		}

		void set_z (float depth)
		{
			z_ = depth;
		}

		const Margin& margin () const
		{
			return margin_;
		}

		void set_margin (int left, int right, int top, int bottom)
		{
			margin_.set_left(left);
			margin_.set_right(right);
			margin_.set_top(top);
			margin_.set_bottom(bottom);
		}

		void set_margin (const Margin& margin)
		{
			margin_ = margin;
		}

		const Padding& padding () const
		{
			return padding_;
		}

		void set_padding (const Padding& padding)
		{
			padding_ = padding;
		}

		void set_round_box_type (RoundBoxType type)
		{
			round_box_type_ = type;
			Update();
		}

		RoundBoxType round_box_type () const
		{
			return round_box_type_;
		}

		bool visible () const {return visible_;}

		void set_visible (bool visible) {
			visible_ = visible;
		}

		void Show ()
		{
			visible_ = true;
		}

		void Hide ()
		{
			visible_ = false;
		}

	protected:	// member functions

		/**
		 * @brief Update data for render, used in size, position change
		 */
		virtual void Update () = 0;

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
				const Rect& rect,
				const Rect& slider,
				ScrollState state);

	protected:
		// member variables

		Size size_;

		Point pos_;

		float z_;	// the depth of the widget, currently it's not used

		Padding padding_; /** used when in Layout */

		Margin margin_; /** used when in Layout */

		RoundBoxType round_box_type_;

		bool visible_;

	private:	// member function disabled

		Drawable (const Drawable& orig);
		Drawable& operator = (const Drawable& orig);
	};

} /* namespace BIL */
#endif /* _BIL_DRAWABLE_H_ */
