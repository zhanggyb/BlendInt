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

#ifndef _BLENDINT_GUI_DECORATION_HPP_
#define _BLENDINT_GUI_DECORATION_HPP_

#include <Cpp/Events.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	/**
	 * @brief A special widget used in virtual window only as the decoration
	 */
	class Decoration:  public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Decoration);

	public:

		Decoration ();

		virtual ~Decoration ();

		void PushFront (AbstractWidget* widget);

		void PushBack (AbstractWidget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void InitializeDecoration ();

		void FillSubWidgets (const Point& out_pos, const Size& out_size, const Margin& margin, int space);

		void FillSubWidgets (int x, int y, int width, int height, int space);

		void RealignSubWidgets (const Size& size, const Margin& margin, int space);

		int GetLastPosition () const;

		GLuint m_vao[1];

		/**
		 * space between tool buttons
		 */
		int m_space;

		bool m_pressed;

		Point m_last;
		Point m_cursor;

		RefPtr<GLArrayBuffer> m_inner;
	};

}


#endif /* _BLENDINT_GUI_DECORATION_HPP_ */
