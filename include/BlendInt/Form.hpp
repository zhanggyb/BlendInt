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

#ifndef _BLENDINT_FORM_HPP_
#define _BLENDINT_FORM_HPP_

#include <BlendInt/AbstractForm.hpp>
#include <BlendInt/GLBuffer.hpp>

#define WIDGET_AA_JITTER 8

/* max as used by round_box__edges */
#define WIDGET_CURVE_RESOLU 9
#define WIDGET_SIZE_MAX (WIDGET_CURVE_RESOLU * 4)

namespace BlendInt {

	/**
	 * @brief A Normal form
	 */
	class Form: public AbstractForm
	{
		DISALLOW_COPY_AND_ASSIGN(Form);

	public:

		Form ();

		Form (AbstractForm* parent);

		virtual ~Form();

	protected:

		/**
		 * Structure used in calulating vertex buffer for inner and outline
		 */
		struct VerticesSum {
			VerticesSum ()
			: total(0), half(0)
			{ }

			int total;	/**< total number of vertices for widget */
			int half;	/**< halfway vertices number */
		};

		virtual void update (int property_type);

		virtual void render ();

		virtual void press_key (KeyEvent* event);

		virtual void press_context_menu (ContextMenuEvent* event);

		virtual void release_context_menu (ContextMenuEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

		virtual void move_mouse (MouseEvent* event);
	};
}

#endif /* _BLENDINT_FORM_HPP_ */
