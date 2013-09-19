/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_CONTEXTMENUEVENT_H_
#define _BIL_CONTEXTMENUEVENT_H_

#include <BILO/InputEvent.hpp>

namespace BILO {

	class ContextMenuEvent: public InputEvent
	{
	public:

		enum Source {
			Unknown,
			Keyboard,
			Mouse
		};

		ContextMenuEvent (Source source,
						  int mods)
			: InputEvent(mods), source_(source),
			pos_(Coord2f(0.0, 0.0)), window_pos_(Coord2f(0.0, 0.0))
		{}


		ContextMenuEvent (Source source,
						  int mods,
						  const Coord2f& local_pos,
						  const Coord2f& window_pos)
			: InputEvent(mods), source_(source),
			pos_(local_pos), window_pos_(window_pos)
		{}

		virtual ~ContextMenuEvent()
		{}

		Source source (void)
		{
			return source_;
		}

		const Coord2f& pos (void) const
		{
			return pos_;
		}

		void set_pos (float x, float y)
		{
			pos_.set_x (x);
			pos_.set_y (y);
		}

		const Coord2f& window_pos (void) const
		{
			return window_pos_;
		}

		void set_window_pos (float x, float y)
		{
			window_pos_.set_x(x);
			window_pos_.set_y(y);
		}

	private:

		Source source_;

		Coord2f pos_;

		Coord2f window_pos_;
	};

}

#endif	/* _BIL_CONTEXTMENUEVENT_H_ */
