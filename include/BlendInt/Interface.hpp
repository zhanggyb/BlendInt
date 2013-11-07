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

/**
 * Doxygen documentation
 */

// the top level groups

/** @defgroup time Time input */
/** @defgroup interface The main interface class entry */
/** @defgroup exceptions Exceptions */
/** @defgroup opengl Wrapper classes for OpenGL API */
/** @defgroup gui GUI classes */

#ifndef _BLENDINT_INTERFACE_HPP_
#define _BLENDINT_INTERFACE_HPP_

#include <BlendInt/Types.hpp>
#include <BlendInt/Size.hpp>

#include <Cpp/Events.hpp>

#define BLENDINT_EVENTS_INIT_ONCE_IN_MAIN Cpp::Events::ProcessInit processInit

namespace BlendInt {

	class AbstractForm;
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

		static Interface* instance ();

		static bool initialize ();

		static void release ();

		bool bind (AbstractForm* object);

		bool unbind (AbstractForm* object);

		void render ();

		void resizeEvent (int width, int height);

		void keyEvent (int key, int scancode, int action, int mods);

		void mouseButtonEvent (int button, int action, int mods);

		void cursorPosEvent (double xpos, double ypos);

		const Size& size () const;

		void resize (int width, int height);

		void resize (const Size& size);

		void dispatch_render_event (AbstractForm* obj);

		void dispatch_key_press_event (AbstractForm* obj, KeyEvent* event);

		void dispatch_mouse_press_event (AbstractForm* obj, MouseEvent* event);

		void dispatch_mouse_release_event (AbstractForm* obj, MouseEvent* event);

		void dispatch_mouse_move_event (AbstractForm* obj, MouseEvent* event);

		Cpp::ConnectionScope& events () {return m_events;}

	private:

		Interface ();

		~Interface ();

		double cursor_pos_x_;	/** cursor x position */
		double cursor_pos_y_;	/** cursor y position */

		Size size_;

		unsigned char m_ticktack;

		Cpp::ConnectionScope m_events;

		static Interface* interface;

#ifdef DEBUG
		void draw_grid (int width, int height);
#endif

	};

	inline Interface* interface()
	{
		return Interface::instance();
	}


}


#endif /* _BIL_INTERFACE_HPP_ */
