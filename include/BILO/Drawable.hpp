/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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
#include <string>

#ifdef DEBUG
#include <map>
#endif

#include <BILO/EventHandler.hpp>

#include <BILO/Point.hpp>
#include <BILO/Size.hpp>
#include <BILO/Padding.hpp>
#include <BILO/Margin.hpp>

#include <BILO/Types.hpp>

#ifdef DEBUG
using std::map;
#endif

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

	class Drawable: public EventHandler
	{
		DISALLOW_COPY_AND_ASSIGN(Drawable);

	public:

		friend class ContextManager;

		Drawable ();

		/**
		 * @brief Default constructor
		 * @param parent
		 *
		 * A drawable instance without a parent will be registered in
		 * a static list -- solo, it's usually a pop-up widget such as
		 * context menu, message box
		 */
		Drawable (Drawable* parent);

		virtual ~Drawable ();

		bool bind (Drawable* child);

		bool unbind (Drawable* child);

		/**
		 * @brief unbind this and set parent to 0
		 */
		void unbind ();

		bool bind_to (ContextManager* parent);

		bool bind_to (Drawable* parent);

		/**
		 * @brief if the root of this Drawable object is bounded to ContextManager
		 * @return
		 */
		bool bounded ();

		const Size& size () const;

		void resize (int w, int h);

		void resize (const Size& size);

		const Point& pos () const;

		void set_pos (int x, int y);

		void set_pos (const Point& pos);

		int z () const
		{
			return m_z;
		}

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

		const std::string& name () const;

		void set_name (const std::string& name);

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

		/**
		 * @brief just change m_z simply
		 * @param z
		 */
		void set_z_simple (int z);

	protected:
		// member variables

		bool visible_;

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		Size size_;

		Point pos_;

		Padding padding_; /** used when in Layout */

		Margin margin_; /** used when in Layout */

		RoundCornerType round_box_type_;

		std::string m_name;

		Parent m_parent;

		std::set<Drawable*> m_children;

#ifdef DEBUG

	public:

		static Drawable* find (uint64_t id);

		static unsigned int map_size ()
		{
			return obj_map.size();
		}

		static const map<uint64_t, Drawable*>& get_map ()
		{
			return obj_map;
		}

		static void reset ()
		{
			id_last = 1;
			obj_map.clear();
		}

	private:

		inline bool register_in_map ();

		inline bool unregister_from_map ();

		// member variables

		uint64_t m_id; /** A unique ID for object */

		// static member variables
		static uint64_t id_last;

		static map<uint64_t, Drawable*> obj_map;

	public:

		static void print ();

#endif

	};

} /* namespace BILO */
#endif /* _BIL_DRAWABLE_H_ */
