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

#ifndef _BLENDINT_VERTICALLAYOUT_HPP_
#define _BLENDINT_VERTICALLAYOUT_HPP_

#include <BlendInt/AbstractLayout.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	class VerticalLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(VerticalLayout);

	public:

		VerticalLayout(int align = AlignVerticalCenter);

		VerticalLayout(int align, Drawable* parent);

		virtual ~VerticalLayout ();

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);

	private:

		void change_layout (const Size* size);

		void generate_default_layout ();

		Size get_minimal_size ();

		void add_item (Drawable* object);

		/**
		 * @brief align the objects in layout according to alignment
		 * @param width[in] the max width of area contains children (width - (left + right margin))
		 */
		void align_along_y (unsigned int width);
	};

}

#endif	// _BLENDINT_VERTICALLAYOUT_HPP_
