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

#ifndef _BIL_WIDGET_H_
#define _BIL_WIDGET_H_

#include <BIL/Drawable.h>

namespace BIL {

	// TODO: rename to Widget, confused to controller in MVC

	class Widget: public BIL::Drawable
	{
	public:
		Widget (Traceable * parent = NULL);

		virtual ~Widget ();

		virtual void render (void);

	protected:

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void InputMethodEvent (unsigned int character);

		virtual void CursorEnterEvent (int entered);

	};

} /* namespace BIL */
#endif /* _BIL_WIDGET_H_ */
