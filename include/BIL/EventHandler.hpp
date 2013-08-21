/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_EVENTHANDLER_H_
#define _BIL_EVENTHANDLER_H_

#include <BIL/MouseEvent.hpp>
#include <BIL/KeyEvent.hpp>
#include <BIL/ContextMenuEvent.hpp>

namespace BIL {

	class Window;

	class EventHandler
	{
	public:

		friend class Window;

		EventHandler () {}
		virtual ~EventHandler () {}

	protected:

		virtual void KeyPressEvent (KeyEvent* event) = 0;

		virtual void ContextMenuPressEvent (ContextMenuEvent* event) = 0;

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event) = 0;

		virtual void MousePressEvent (MouseEvent* event) = 0;

		virtual void MouseReleaseEvent (MouseEvent* event) = 0;

		virtual void MouseMoveEvent (MouseEvent* event) = 0;

		virtual void InputMethodEvent (unsigned int character) = 0;

		virtual void CursorEnterEvent (int entered) = 0;

		virtual void Render () = 0;

	//private:
		//EventHandler(const EventHandler& orig);
		//EventHandler& operator = (const EventHandler& orig);
	};

} /* namespace BIL */
#endif /* _BIL_EVENTHANDLER_H_ */
