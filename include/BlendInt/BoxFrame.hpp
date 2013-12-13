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

#ifndef _BLENDINT_LAYOUTFRAME_HPP_
#define _BLENDINT_LAYOUTFRAME_HPP_

#include <BlendInt/Frame.hpp>
#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	/**
	 * @brief A special frame with a default layout packed
	 */
	class BoxFrame: public Frame
	{
		DISALLOW_COPY_AND_ASSIGN(BoxFrame);

	public:

		BoxFrame();

		BoxFrame(AbstractWidget* parent);

		void SetLayout (AbstractLayout* layout);

		AbstractLayout* layout () const {return m_layout;}

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		inline void set_layout (AbstractLayout* layout)
		{
			m_layout = layout;
		}

	private:

		AbstractLayout* m_layout;
	};
}

#endif /* _BLENDINT_LAYOUTFRAME_HPP_ */
