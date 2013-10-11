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

#ifndef _BILO_SCROLLBAR_HPP_
#define _BILO_SCROLLBAR_HPP_

#include <BILO/AbstractSlider.hpp>

#include <BILO/GLBuffer.hpp>

namespace BILO {

	/**
	 * The controller in the scrollbar
	 */
	class ScrollControl: public Widget
	{
	public:

		ScrollControl ();

		ScrollControl (Drawable* parent);

		virtual ~ScrollControl ();

		bool pressed () const {return m_pressed;}

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		void update_shape (const Size* size);

		bool m_pressed;

		Point m_move_start;

		Point m_position_origin;
	};

	class ScrollBar: public AbstractSlider
	{
	public:

		ScrollBar (Orientation orientation = Horizontal);

		ScrollBar (Orientation orientation, Drawable* parent);

		virtual ~ScrollBar ();

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		ScrollControl* m_scroll_control;

	};

}

#endif /* _BIL_SCROLLBAR_HPP_ */
