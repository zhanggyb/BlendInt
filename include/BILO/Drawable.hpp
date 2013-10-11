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

#ifndef _BILO_DRAWABLE_HPP_
#define _BILO_DRAWABLE_HPP_

#include <list>
#include <set>
#include <string>

#include <BILO/EventHandler.hpp>

#include <BILO/Point.hpp>
#include <BILO/Size.hpp>
#include <BILO/Padding.hpp>
#include <BILO/Margin.hpp>

#include <BILO/Types.hpp>

#ifdef DEBUG
#include <map>
using std::map;
#endif

namespace BILO {

	class ContextManager;
	class Drawable;
	class Rect;

	struct WidgetTheme;

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
		bool is_bound ();

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

		void reset_z (int z);

		void set_roundcorner (int type);

		int roundcorner () const;

		void set_corner_radius (float radius);

		float corner_radius () const {return m_corner_radius;}

		void set_hexpand (bool expand) {m_hexpand = expand;}

		bool hexpand () const {return m_hexpand;}

		void set_vexpand (bool expand) {m_vexpand = expand;}

		bool vexpand () const {return m_vexpand;}

		void set_expand (bool expand) {m_hexpand = expand; m_vexpand = expand;}

		bool visible () const;

		void set_visible (bool visible);

		void show ();

		void hide ();

		const std::string& name () const;

		void set_name (const std::string& name);

		const Size& minimal_size () const {return m_minimal_size;}

		void set_minimal_size (const Size& size);

		void set_minimal_size (int width, int height);

	protected:	// member functions

		bool contain (const Coord2d& cursor);

		/**
		 * @brief Update data for render, used in size, shape change only
		 *
		 * @return true - accept this update, false - discard this update
		 */
		virtual bool update (int type, const void* property) = 0;

		/**
		 * @brief just change m_z simply
		 * @param z
		 */
		void set_z_simple (int z);

		/**
		 * @brief set Drawable object position without checking layout
		 * @param obj
		 * @param x
		 * @param y
		 *
		 * This function should be called in Layout only
		 */
		void set_pos_priv (Drawable* obj, int x, int y);

		/**
		 * @brief set Drawable object position without checking layout
		 * @param obj
		 * @param pos
		 *
		 * This function should be called in layout only
		 */
		void set_pos_priv (Drawable* obj, const Point& pos);

		void resize_priv (Drawable* obj, int width, int height);

		void resize_priv (Drawable* obj, const Size& size);

	protected:
		// member variables

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		int m_roundcorner;

		/**
		 * @brief the round corner radius
		 *
		 * should be 0.0, 1.0, 2.0 etc
		 */
		float m_corner_radius;

		bool m_visible;

		bool m_hexpand;

		bool m_vexpand;

		Size m_size;

		Size m_minimal_size;

		Point m_pos;

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
