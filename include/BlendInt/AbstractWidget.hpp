/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_ABSTRACTWIDGET_HPP_
#define _BLENDINT_ABSTRACTWIDGET_HPP_

#include <list>
#include <set>
#include <string>

#include <boost/smart_ptr.hpp>

#include <BlendInt/AbstractExtraForm.hpp>
#include <BlendInt/RoundBoxBase.hpp>
#include <BlendInt/MouseEvent.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/ContextMenuEvent.hpp>
#include <BlendInt/Point.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/Margin.hpp>

#include <BlendInt/Types.hpp>

#include <Cpp/Events.hpp>

#ifdef DEBUG
#include <map>
using std::map;
#endif

namespace BlendInt {

	class ContextManager;
	class AbstractWidget;
	class Rect;

	struct WidgetTheme;

	enum ParentType {
		ParentUnknown,
		ParentContextManager,
		ParentForm
	};

	union ParentPointer {
		ParentPointer ()
		: nameless(0)
		{

		}
		ContextManager* context;
		AbstractWidget* form;
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

	/**
	 * @brief The basic abstract class for GUI forms in BlendInt
	 *
	 * @ingroup gui
	 */
	class AbstractWidget: public AbstractExtraForm
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractWidget);

	public:

		friend class ContextManager;
		friend class Interface;

		/**
		 * @brief the default constructor
		 */
		AbstractWidget ();

		/**
		 * @brief Default constructor
		 * @param parent
		 *
		 * A drawable instance without a parent will be registered in
		 * a static list -- solo, it's usually a pop-up widget such as
		 * context menu, message box
		 */
		AbstractWidget (AbstractWidget* parent);

		virtual ~AbstractWidget ();

		bool bind (AbstractWidget* child);

		bool unbind (AbstractWidget* child);

		/**
		 * @brief unbind this and set parent to 0
		 */
		void unbind ();

		bool bind_to (ContextManager* parent);

		bool bind_to (AbstractWidget* parent);

		/**
		 * @brief if the root of this Drawable object is bounded to ContextManager
		 * @return
		 */
		bool is_bound ();

		/**
		 * @brief Call Update() and Resize the widget
		 * @param width the newwidth of the widget
		 * @param height the new height of the widget
		 *
		 * @note this function hide the same function in the base class
		 * (AbstractForm::Resize()) as it need to check the new size and call
		 * Update()
		 */
		void Resize (unsigned int width, unsigned int height);

		void Resize (const Size& size);

		void SetPosition (int x, int y);

		void SetPosition (const Point& pos);

		void SetPreferredSize (unsigned int widt, unsigned int height);

		void SetPreferredSize (const Size& size);

		void SetMinimalSize (unsigned int width, unsigned int height);

		void SetMinimalSize (const Size& size);

		void SetMaximalSize (unsigned int width, unsigned int height);

		void SetMaximalSize (const Size& size);

		int z () const
		{
			return m_z;
		}

		void reset_z (int z);

		void show ();

		void hide ();

		const std::string& name () const;

		void set_name (const std::string& name);

		const Parent& parent () const {return m_parent;}

		bool locked () const {return m_locked;}

		EVENT_CALLER Cpp::EventRef<AbstractWidget*, int> property_changed() {return m_property_changed;}

		void activate_events ()
		{
			m_fire_events = true;
		}

		void deactivate_events ()
		{
			m_fire_events = false;
		}

		bool fire_events () const {return m_fire_events;}

		const std::set<AbstractWidget*>& children() const {return m_children;}

		/**
		 * @brief move this object along x axis
		 * @param offset_x
		 */
		inline void MoveX (int offset_x)
		{
			SetPosition(position().x() + offset_x, position().y());
		}

		/**
		 * @brief move this object along y axis
		 * @param offset_y
		 */
		inline void MoveY (int offset_y)
		{
			SetPosition(position().x(), position().y() + offset_y);
		}

		/**
		 * @brief move this object
		 * @param offset_x
		 * @param offset_y
		 */
		inline void Move (int offset_x, int offset_y)
		{
			SetPosition(position().x() + offset_x, position().y() + offset_y);
		}

	protected:	// member functions

		virtual void KeyPressEvent (KeyEvent* event) = 0;

		virtual void ContextMenuPressEvent (ContextMenuEvent* event) = 0;

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event) = 0;

		virtual void MousePressEvent (MouseEvent* event) = 0;

		virtual void MouseReleaseEvent (MouseEvent* event) = 0;

		virtual void MouseMoveEvent (MouseEvent* event) = 0;

		bool contain (const Coord2d& cursor);

		/**
		 * @brief Update opengl data (usually the GL buffer) for Render
		 * @param[in] type the enumeration defined by each form class, e.g.
		 * FormPropertySize
		 * @param[in] data the pointer to the new property data
		 *
		 * This virtual function should be implemented in each derived class,
		 * and should only use the form's property to draw opengl elements once.
		 */
		virtual void Update (int type, const void* data) = 0;

		/**
		 * @brief just change m_z simply
		 * @param z
		 */
		void set_z_simple (int z);

		void LockGeometry (AbstractWidget* obj, bool status) {obj->m_locked = status;}

		//Cpp::ConnectionScope& events() {return m_events;}

		boost::scoped_ptr<Cpp::ConnectionScope>& events() {return m_events;}

		/**
		 * @brief fire event to inform the property of this object is changed
		 * @param[in] type the property type, defined in FormPropertyType
		 */
		inline void fire_property_changed_event (int type)
		{
			if (m_fire_events)
				m_property_changed.fire(this, type);
		}

		static void SetPosition (AbstractWidget* obj, int x, int y);

		static void SetPosition (AbstractWidget* obj, const Point& pos);

		/**
		 * @brief resize other object's size
		 * @param obj
		 * @param w
		 * @param h
		 *
		 * @note should be used in layout only
		 */
		static void Resize (AbstractWidget* obj, unsigned int w, unsigned int h);

		/**
		 * @brief resize other object's size
		 * @param obj
		 * @param size
		 *
		 * @note should be used in layout only
		 */
		static void Resize (AbstractWidget* obj, const Size& size);

		static void dispatch_key_press_event (AbstractWidget* obj, KeyEvent* event);

		static void dispatch_mouse_move_event (AbstractWidget* obj, MouseEvent* event);

		static void dispatch_mouse_press_event (AbstractWidget* obj, MouseEvent* event);

		static void dispatch_mouse_release_event (AbstractWidget* obj, MouseEvent* event);

	private:

		/**
		 * @brief the depth(layer) of the widget
		 */
		int m_z;

		/**
		 * @brief if lock the geometry of this widget
		 */
		bool m_locked;

		bool m_fire_events;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		Cpp::Event<AbstractWidget*, int> m_property_changed;

		Parent m_parent;

		std::set<AbstractWidget*> m_children;

		std::string m_name;

#ifdef DEBUG
	public:

		static AbstractWidget* find (uint64_t id);

		static unsigned int map_size ()
		{
			return obj_map.size();
		}

		static const map<uint64_t, AbstractWidget*>& get_map ()
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

		uint64_t m_id; /** A unique ID for object */

		static uint64_t id_last;

		static map<uint64_t, AbstractWidget*> obj_map;

	public:

		static void print ();

#endif

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_ABSTRACTWIDGET_HPP_ */
