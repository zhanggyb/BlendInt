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

/**
 * @defgroup functions_throw_exception Functions throw exception
 * @ingroup exceptions
 */

#ifndef _BLENDINT_INTERFACE_HPP_
#define _BLENDINT_INTERFACE_HPP_

#include <BlendInt/Types.hpp>
#include <BlendInt/Size.hpp>

#include <Cpp/Events.hpp>

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

		static Interface* instance ();

		static bool initialize ();

		static void release ();

		bool bind (AbstractWidget* object);

		bool unbind (AbstractWidget* object);

		void render ();

		void resizeEvent (int width, int height);

		void keyEvent (int key, int scancode, int action, int mods);

		void mouseButtonEvent (int button, int action, int mods);

		void cursorPosEvent (double xpos, double ypos);

		const Size& size () const;

		void resize (int width, int height);

		void resize (const Size& size);

		Cpp::ConnectionScope& events () {return m_events;}

	private:

		void dispatch_render_event (AbstractWidget* obj);

		void dispatch_key_press_event (AbstractWidget* obj, KeyEvent* event);

		void dispatch_mouse_press_event (AbstractWidget* obj, MouseEvent* event);

		void dispatch_mouse_release_event (AbstractWidget* obj, MouseEvent* event);

		void dispatch_mouse_move_event (AbstractWidget* obj, MouseEvent* event);

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

/**
 * Doxygen documentation
 */

/**
 * @mainpage BlendInt -- a Blender-like Interface Library in OpenGL
 * 
 * @section sect_intro Introduction
 * 
 * BlendInt (Blender Interface Library in OpenGL) is a C++ library for
 * building application with Blender-like style.
 *
 * This project is for anyone who like Blender and want to build a
 * similar GUI application.
 *
 * Features of vertion 0.1 (developing):
 * 	- Event system
 * 	- Widgets:
 * 		- Buttons:
 * 		- Push Button
 * 		- Toggle Button
 * 		- Checkable Button
 * 	- Scroll Widget
 * 	- Label
 * 	- Slider
 * 	- Tab Widget
 * 	- Layout
 * 	- Horizontal Layout
 * 	- Vertical Layout
 * 
 * @section usage_sec Usage
 * 
 * Use BlendInt is as simple as any C++ object-oriented GUI toolkit
 * like Qt, gtkmm.  For example, create a button with BlendInt:
 *
 *@code
 #include <BlendInt/Interface.hpp> // The main header to start
 #include <BlendInt/Button.hpp>

 int main (int argc, char* argv[])
 {
 	// initialize opengl context
 	BlendInt::Interface::initialize();

 	Button* my_button = new Button;
 	my_button.set_pos(200, 200); 

 	// in the event-render loop
 	app->render();

 	// ready to close application
 	BlendInt::Interface::release();
 }
  @endcode
 * 
 * @section sect_compile Compile
 * 
 * @subsection sect_compile_require Requirements
 * 

 * @ref page_quick_guide
 */

/**
 * @page page_quick_guide Quick Guide
 * @tableofcontents
 * 
 * Here is a quick guide of using BlendInt to build a applicaiton
 * 
 * @section sect_install Install BlendInt
 * 
 * @subsection sect_install_download Download
 * 
 * @subsection sect_install_compile Compile
 * 
 * and More document later.
 */
