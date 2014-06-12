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

#ifndef _BLENDINT_GUI_FRAME_HPP_
#define _BLENDINT_GUI_FRAME_HPP_

/**
 * @defgroup widgets Widgets
 *
 * @ingroup gui
 */

#include <BlendInt/Gui/AbstractSingleContainer.hpp>

namespace BlendInt {

	struct WidgetTheme;
	class Color;

	/**
	 * @brief A widget usually contains other Form or widget in a box with padding
	 *
	 * @ingroup widgets
	 */
	class Frame: public AbstractSingleContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Frame);

	public:

		Frame ();

		virtual ~Frame ();

		bool Setup (AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool UpdateGeometryTest (const WidgetUpdateRequest& request);

		virtual void UpdateContainer (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		GLuint m_vao;
		RefPtr<GLArrayBuffer> m_inner;

	};

} /* namespace BlendInt */

#endif /* _BLENDINT_GUI_FRAME_HPP_ */
