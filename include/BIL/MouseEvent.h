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

#ifndef _BIL_MOUSEEVENT_H_
#define _BIL_MOUSEEVENT_H_

#include <BIL/Types.h>
#include <BIL/Tuple.h>
#include <BIL/Event.h>

namespace BIL {

	class MouseEvent: public Event
	{
	public:
		
		MouseEvent (Type type, MouseButton button,
					const Coord2f& localPos,
					const Coord2f& windowPos)
			: _type (type), _button (button),
			_pos (localPos), _windowPos (windowPos)
		{}

		virtual ~MouseEvent ()
		{}

		MouseButton button (void) const
		{
			return _button;
		}

		void setLocalPos (float x, float y)
		{
			_pos.coord.x = x;
			_pos.coord.y = y;
		}

		const Coord2f& windowPos (void) const
		{
			return _windowPos;
		}

		Coord2f pos (void) const;

	private:

		Type _type;

		MouseButton _button;
		Coord2f _pos;
		Coord2f _windowPos;
	};

}

#endif	/* _BIL_MOUSEEVENT_H_ */
