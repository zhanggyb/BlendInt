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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_ABSTRACTPANEL_HPP_
#define _BLENDINT_GUI_ABSTRACTPANEL_HPP_

#include <BlendInt/Gui/Layout.hpp>

namespace BlendInt {

	/**
	 * @brief A special container works as a window in desktop
	 *
	 * The margin of a VirtualWindow is always zero.
	 */
	class AbstractPanel: public Layout
	{
	public:

		AbstractPanel ();

		virtual ~AbstractPanel ();

		void SetDecoration (AbstractWidget* widget);

		void SetContent (AbstractWidget* widget);

		void SetSpace (int space);

		virtual Size GetPreferredSize () const;

		AbstractWidget* decoration () const
		{
			return decoration_;
		}

		AbstractWidget* content () const
		{
			return content_;
		}

		int space () const
		{
			return space_;
		}

	protected:

		enum SubWidgetIndex {
			DecorationIndex = 0,
			ContentIndex
		};

		virtual void PerformMarginUpdate (const Margin& margin);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		void FillSubWidgets (const Size& size, const Margin& margin);

		void FillSubWidgets (int x, int y, int w, int h);

	private:

		AbstractWidget* decoration_;
		AbstractWidget* content_;
		int space_;

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTPANEL_HPP_ */
