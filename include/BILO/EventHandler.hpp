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

#ifndef _BILO_EVENTHANDLER_HPP_
#define _BILO_EVENTHANDLER_HPP_

#include <BILO/MouseEvent.hpp>
#include <BILO/KeyEvent.hpp>
#include <BILO/ContextMenuEvent.hpp>

namespace BILO {

	class Interface;

	class EventHandler
	{
	public:

		friend class Interface;

		EventHandler ()
		: m_ticktack(0)
		{}
		virtual ~EventHandler () {}

	protected:

		virtual void press_key (KeyEvent* event) = 0;

		virtual void press_context_menu (ContextMenuEvent* event) = 0;

		virtual void release_context_menu (ContextMenuEvent* event) = 0;

		virtual void press_mouse (MouseEvent* event) = 0;

		virtual void release_mouse (MouseEvent* event) = 0;

		virtual void move_mouse (MouseEvent* event) = 0;

		virtual void render () = 0;

	private:

		/**
		 * A ticktack for one render loop
		 */
		unsigned char m_ticktack;
	};

} /* namespace BILO */
#endif /* _BIL_EVENTHANDLER_H_ */
