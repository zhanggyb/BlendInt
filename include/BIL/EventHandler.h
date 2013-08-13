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

#include <BIL/MouseEvent.h>
#include <BIL/KeyEvent.h>

namespace BIL {

	class Window;

	class EventHandler
	{
	public:

		friend class Window;

		EventHandler ();
		virtual ~EventHandler ();

	protected:

		virtual void keyEvent (KeyEvent* event) = 0;

		virtual void mouseEvent (MouseEvent* event) = 0;

		// virtual void keyEvent (int key, int scancode, int action, int mods) = 0;

		virtual void charEvent (unsigned int character) = 0;

		virtual void mouseButtonEvent (int button, int action, int modes) = 0;
		
		virtual void cursorPosEvent (double xpos, double ypos) = 0;

		virtual void cursorEnterEvent (int entered) = 0;

	private:
		EventHandler(const EventHandler& orig);
		EventHandler& operator = (const EventHandler& orig);
	};

} /* namespace BIL */
#endif /* _BIL_EVENTHANDLER_H_ */
