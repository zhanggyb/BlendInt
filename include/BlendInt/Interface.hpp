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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

/**
 * Doxygen documentation
 */

// the top level groups

/** @defgroup time Time input */
/** @defgroup interface The main interface class entry */
/** @defgroup exceptions Exceptions */
/** @defgroup opengl Wrapper classes for OpenGL API */
/** @defgroup gui GUI classes */
/** @defgroup animation Animation */

/**
 * @defgroup functions_throw_exception Functions throw exception
 * @ingroup exceptions
 */

#ifndef _BLENDINT_INTERFACE_HPP_
#define _BLENDINT_INTERFACE_HPP_

#include <BlendInt/Types.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/Point.hpp>

#ifdef DEBUG
#include <BlendInt/AbstractForm.hpp>
#endif

#include <Cpp/Events.hpp>

#include <boost/smart_ptr.hpp>

#include <string>

#define BLENDINT_EVENTS_INIT_ONCE_IN_MAIN Cpp::Events::ProcessInit processInit

namespace BlendInt {

	class AbstractWidget;
	class KeyEvent;
	class MouseEvent;

	/**
	 * @brief The main entry for this library
	 *
	 * @ingroup interface
	 */
	class Interface
	{
	public:

		/**
		 * @brief Get the instance object of this class
		 * @return Pointer to the Interface object
		 *
		 * Must be used after Interface::Initialize()
		 */
		static Interface* Instance ();

		static bool Initialize ();

		static void Release ();

		void Draw ();

		void DispatchKeyEvent (KeyEvent* event);

		void DispatchMouseEvent (MouseEvent* event);

		const Size& size () const;

		/**
		 * @brief Resize the interface
		 */
		void Resize (const Size& size);

		/**
		 * @brief Resize the interface
		 */
		void Resize (unsigned int width, unsigned int height);

		void SetMainWidget (AbstractWidget* widget);

		Cpp::EventRef<unsigned int, unsigned int> resized() {return m_resized;}

		Cpp::ConnectionScope* events() const {return m_events.get();}

		void set_focus_style (FocusStyle style)
		{
			m_focus_style = style;
		}

		FocusStyle focus_style () const {
			return m_focus_style;
		}

	private:

		Interface ();

		~Interface ();

		void RenderToImage ();

		void DrawToOffScreen ();

		void DispatchCursorMoveEvent (MouseEvent* event);

		void DispatchMousePressEvent (MouseEvent* event);

		void DispatchMouseReleaseEvent (MouseEvent* event);

		void DispatchDrawEvent (AbstractWidget* widget);

		void BuildWidgetListAtCursorPoint (const Point& cursor_point, AbstractWidget* parent);

		AbstractWidget* m_main;

		//int cursor_pos_x_;	/** cursor x position */
		//int cursor_pos_y_;	/** cursor y position */
		Point m_cursor;

		Size m_size;

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		Cpp::Event<unsigned int, unsigned int> m_resized;

		FocusStyle m_focus_style;

		static Interface* interface;

#ifdef DEBUG

		void draw_grid (int width, int height);

		void DrawTriangle (bool fbo);

#endif

	};

	inline Interface* interface ()
	{
		return Interface::Instance();
	}

}

#endif /* _BIL_INTERFACE_HPP_ */

