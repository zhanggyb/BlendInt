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

#ifndef _BLENDINT_GUI_SPLITTER_HPP_
#define _BLENDINT_GUI_SPLITTER_HPP_

#include <BlendInt/Gui/AbstractDequeContainer.hpp>

namespace BlendInt {

	class Splitter: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Splitter);

	public:

		Splitter (Orientation orientation = Horizontal);

		virtual ~Splitter ();

		void Add (AbstractWidget* widget);

		void Remove (AbstractWidget* widget);

	protected:

		virtual bool UpdateTest (const UpdateRequest& request);

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void AlignSubWidgets (Orientation orientation, const Size& size, const Margin& margin, int space);

		unsigned int GetAverageRoom (Orientation orientation, const Size& size, const Margin& margin, int space);

		Orientation m_orientation;

		int m_space;

	};
}


#endif /* _BLENDINT_GUI_SPLITTER_HPP_ */
