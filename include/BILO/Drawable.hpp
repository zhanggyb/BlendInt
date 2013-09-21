/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_DRAWABLE_HPP_
#define _BIL_DRAWABLE_HPP_

#include <list>

#include <set>

#include <BILO/Traceable.hpp>
#include <BILO/EventHandler.hpp>

#include <BILO/Point.hpp>
#include <BILO/Size.hpp>
#include <BILO/Padding.hpp>
#include <BILO/Margin.hpp>
#include <BILO/String.hpp>

#include <BILO/Types.hpp>

namespace BILO {

	class ContextManager;
	class Drawable;
	class Rect;

	struct WidgetColors;

	enum ParentType {
		ParentUnknown,
		ParentContextManager,
		ParentDrawable
	};

	union ParentPointer {
		ParentPointer ()
		: nameless(0)
		{

		}
		ContextManager* context;
		Drawable* drawable;
		void* nameless;
	};

	struct Parent {
		Parent ()
		: type(ParentUnknown)
		{

		}
		ParentType type;
		ParentPointer object;

		DISALLOW_COPY_AND_ASSIGN(Parent);
	};

	class Drawable: public Traceable, public EventHandler
	{
		DISALLOW_COPY_AND_ASSIGN(Drawable);

	public:

		friend class ContextManager;

		/**
		 * @brief Default constructor
		 * @param parent
		 *
		 * A drawable instance without a parent will be registered in
		 * a static list -- solo, it's usually a pop-up widget such as
		 * context menu, message box
		 */
		Drawable (Drawable* parent = 0);

		virtual ~Drawable ();

		void bind (Drawable* child);

		void unbind (Drawable* child);

		/**
		 * @brief unbind this and set parent to 0
		 */
		void unbind ();

		void bind_to (ContextManager* parent);

		void bind_to (Drawable* parent);

		virtual void set_parent (Drawable* parent);

		bool add_child (Drawable* child);

		bool remove_child (Drawable* child);

		const Size& size () const;

		void resize (int w, int h);

		void resize (const Size& size);

		const Point& pos () const;

		void set_pos (int x, int y);

		void set_pos (const Point& pos);

		int z () const;

		void set_z (int z);

		const Margin& margin () const;

		void set_margin (int left, int right, int top, int bottom);

		void set_margin (const Margin& margin);

		const Padding& padding () const;

		void set_padding (const Padding& padding);

		void set_padding (int l, int r, int t, int b);

		void set_round_box_type (RoundCornerType type);

		RoundCornerType round_box_type () const;

		bool visible () const;

		void set_visible (bool visible);

		void show ();

		void hide ();

		const String& name () const;

		void set_name (const String& name);

	protected:	// member functions

		bool contain (const Coord2d& cursor);

		/**
		 * @brief Update data for render, used in size, shape change only
		 *
		 * @warning DO NOT call functions to change own properties again,
		 * e.g, resize()
		 */
		virtual void update (int property) = 0;

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

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		Padding padding_; /** used when in Layout */

		Margin margin_; /** used when in Layout */

		RoundCornerType round_box_type_;

		bool visible_;

		String m_name;

		Parent m_parent_new;

		std::set<Drawable*> m_children_new;

	};

} /* namespace BILO */
#endif /* _BIL_DRAWABLE_H_ */
