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

#include <BlendInt/Gui/AbstractMenuItem.hpp>
#include <BlendInt/Gui/Action.hpp>

#ifndef _BLENDINT_GUI_MENUITEM_HPP_
#define _BLENDINT_GUI_MENUITEM_HPP_

namespace BlendInt {

	class MenuItem: public AbstractMenuItem
	{
	public:

		MenuItem (const String& text);

		MenuItem (const RefPtr<Action>& action);

		MenuItem (const String& text, const String& shortcut);

		MenuItem (const RefPtr<AbstractIcon>& icon, const String& text);

		MenuItem (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut);

		bool AddSubMenuItem (MenuItem* menuitem);

		bool InsertSubMenuItem (int index, MenuItem* menuitem);

		virtual ~MenuItem();

	protected:

		virtual void PerformHoverIn (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType Draw (const Context* context);

	private:

		void InitializeMenuItem ();

		RefPtr<Action> action_;

	};

}

#endif /* _BLENDINT_GUI_MENUITEM_HPP_ */
