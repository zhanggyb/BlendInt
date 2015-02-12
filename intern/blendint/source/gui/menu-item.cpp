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

#include <gui/menu-item.hpp>

namespace BlendInt {

	MenuItem::MenuItem (const String& text)
	: AbstractMenuItem()
	{
		action_.reset(new Action(text));

		InitializeMenuItem();
	}

	MenuItem::MenuItem(const RefPtr<Action>& action)
	: AbstractMenuItem()
	{
#ifdef DEBUG
		assert(action);
#endif

		action_ = action;
		InitializeMenuItem();
	}

	MenuItem::MenuItem (const String& text, const String& shortcut)
	: AbstractMenuItem()
	{
		action_.reset(new Action(text, shortcut));
		InitializeMenuItem();
	}

	MenuItem::MenuItem (const RefPtr<AbstractIcon>& icon, const String& text)
	: AbstractMenuItem()
	{
		action_.reset(new Action(icon, text));
		InitializeMenuItem();
	}

	MenuItem::MenuItem (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut)
	: AbstractMenuItem()
	{
		action_.reset(new Action(icon, text, shortcut));
		InitializeMenuItem();
	}

	MenuItem::~MenuItem()
	{
		// nothing to do here
	}

	bool MenuItem::AddSubMenuItem(MenuItem* menuitem)
	{
		if(PushBackSubView(menuitem)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	bool MenuItem::InsertSubMenuItem(int index, MenuItem* menuitem)
	{
		if(InsertSubView(index, menuitem)) {
			RequestRedraw();
			return true;
		}

		return false;
	}

	void MenuItem::PerformHoverIn(AbstractWindow* context)
	{
		RequestRedraw();
	}

	void MenuItem::PerformHoverOut(AbstractWindow* context)
	{
		RequestRedraw();
	}

	Response MenuItem::Draw(AbstractWindow* context)
	{
		// Menu Icon only show itself
		if(action_->icon()) {
			action_->icon()->Draw(0.f, 0.f, 0);
		}

//		Font font;

//		if(hover()) {
//			font.set_color(Color(1.f, 0.f, 0.f, 1.f));
//		} else {
//			font.set_color(Color(1.f, 1.f, 1.f, 1.f));
//		}
//
//		int x = 16 + 2;
//		int y = (size().height() - font.GetHeight()) / 2 + std::abs(font.descender());
//		font.set_pen(x, y);
//
//		font.Print(0.f, 0.f, action_->text());

		return Finish;
	}

	void MenuItem::InitializeMenuItem()
	{
		set_size(240, 20);
	}
}
