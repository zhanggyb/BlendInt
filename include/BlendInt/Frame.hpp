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

#ifndef _BLENDINT_FRAME_HPP_
#define _BLENDINT_FRAME_HPP_

/**
 * @defgroup widgets Widgets
 *
 * @ingroup gui
 */

#include <BlendInt/Widget.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	enum FramePropertyType {
		FramePadding = WidgetPropertyLast + 1,
		FramePropertyLast = FramePadding
	};

	/**
	 * @brief A widget usually contains other Form or widget in a box with padding
	 *
	 * @ingroup widgets
	 */
	class Frame: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(Frame);

	public:

		Frame ();

		Frame (AbstractWidget* parent);

		virtual ~Frame ();

		const Padding& padding () const {return m_padding;}

		void set_padding (const Padding& padding);

		void set_padding (int left, int right, int top, int bottom);

	protected:

		virtual void update (int type, const void* data);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

		/**
		 * @check if geometry contains the cursor, exclude padding
		 * @param cursor
		 * @return
		 */
		bool contain_no_padding (const Coord2d& cursor);

	private:

		Padding m_padding;

	};

} /* namespace BlendInt */
#endif /* _BLENDINT_FRAME_HPP_ */
