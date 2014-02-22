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

#ifndef _BLENDINT_CONTEXTMANAGER_HPP_
#define _BLENDINT_CONTEXTMANAGER_HPP_

#include <map>
#include <set>
#include <deque>

#include <boost/smart_ptr.hpp>
#include <Cpp/Events.hpp>

#include <BlendInt/Core/Point.hpp>

using std::map;
using std::set;

namespace BlendInt {

	class Interface;
	class AbstractWidget;
	class GLTexture2D;

	struct ContextLayer {

		ContextLayer ();
		~ContextLayer ();

		bool refresh;

		set<AbstractWidget*>* widgets;

		GLTexture2D* buffer;
	};

	/**
	 * @brief Class to hold and manage widget objects for Render
	 */
	class ContextManager
	{
	public:

		friend class Interface;

		static ContextManager* Instance ();
		
		bool Register (AbstractWidget* obj);

		bool Unregister (AbstractWidget* obj);

		size_t index_size () const {return m_index.size();}

		size_t layer_size () const {return m_layers.size();}

		void RemoveWidgetFromHoverDeque (AbstractWidget* widget);

		void SetFocusedWidget (AbstractWidget* widget);

#ifdef DEBUG

		void print ();

#endif

	private:

		static bool Initialize ();

		static void Release ();

		void OnDestroyObject (AbstractWidget* obj);

		/**
		 * @brief add the drawable in the context list
		 * @param obj
		 *
		 * add or Update the Drawable object in the context list
		 */
		void AddWidget (AbstractWidget* obj);

		/**
		 * @breif remove a drawable from the context list
		 * @param obj
		 * @return
		 */
		bool RemoveWidget (AbstractWidget* obj);

		ContextManager ();

		~ContextManager ();

		void BuildWidgetListAtCursorPoint (const Point& cursor_point, const AbstractWidget* parent);

		map<int, ContextLayer > m_layers;

		map<AbstractWidget*, int> m_index;

		static ContextManager* context_manager;
		
		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		/**
		 * @brief The widget stack to contain the current mouse cursor
		 */
		boost::scoped_ptr<std::deque<AbstractWidget*> > m_hover_deque;
	};

}

#endif	// _BIL_CONTEXTMANAGER_HPP_
