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
/** @defgroup window Classes to process native window system */
/** @defgroup animation Animation */

/**
 * @defgroup functions_throw_exception Functions throw exception
 * @ingroup exceptions
 */

#ifndef _BLENDINT_INTERFACE_HPP_
#define _BLENDINT_INTERFACE_HPP_

#include <set>
#include <deque>
#include <string>

#include <boost/smart_ptr.hpp>

#include <Cpp/Events.hpp>

#include <BlendInt/Types.hpp>
#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/Point.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

#define BLENDINT_EVENTS_INIT_ONCE_IN_MAIN Cpp::Events::ProcessInit processInit

namespace BlendInt {

	class AbstractWidget;
	class KeyEvent;
	class MouseEvent;
	class Context;

	/**
	 * @brief The main entry for this library
	 *
	 * @ingroup interface
	 */
	class Interface
	{
	public:

		static Interface* instance;

		static bool Initialize ();

		static void Release ();

		void Draw ();

		void Draw (const RedrawEvent& event);

		void DrawContext (Context* context, const RedrawEvent& event);

		void DispatchKeyEvent (Context* context, const KeyEvent& event);

		void DispatchKeyEvent (const KeyEvent& event);

		void DispatchMouseEvent (Context* context, const MouseEvent& event);

		void DispatchMouseEvent (const MouseEvent& event);

		const Size& size () const;

		int GetCurrentContextWidth () const;

		int GetCurrentContextHeight () const;

		/**
		 * @brief Resize the interface
		 */
		void Resize (const Size& size);

		void ResizeContext (Context* context, const Size& size);

		/**
		 * @brief Resize the interface
		 */
		void Resize (int width, int height);

		void ResizeContext (Context* context, int width, int height);

		void SetCurrentContext (Context* context);

		Cpp::ConnectionScope* events () const
		{
			return m_events.get();
		}

		void set_focus_style (FocusStyle style)
		{
			m_focus_style = style;
		}

		FocusStyle focus_style () const
		{
			return m_focus_style;
		}

	private:

		Interface ();

		~Interface ();

		void RenderToImage ();

		void OnCurrentContextDestroyed (AbstractWidget* context);

		boost::scoped_ptr<Cpp::ConnectionScope> m_events;

		FocusStyle m_focus_style;

		static void GetGLVersion (int *major, int *minor);
		
		static void GetGLSLVersion (int *major, int *minor);

		Context* m_current_context;
	};

}

#endif /* _BLENDINT_INTERFACE_HPP_ */

