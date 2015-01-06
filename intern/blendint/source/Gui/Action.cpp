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

#include <algorithm>

#include <BlendInt/Gui/Action.hpp>

namespace BlendInt {

	RefPtr<Action> Action::Create ()
	{
		RefPtr<Action> item(new Action);

		return item;
	}
	
	RefPtr<Action> Action::Create (const String& text)
	{
		RefPtr<Action> item(new Action(text));

		return item;
	}

	RefPtr<Action> Action::Create (const String& text, const String& shortcut)
	{
		RefPtr<Action> item(new Action(text, shortcut));

		return item;
	}

	RefPtr<Action> Action::Create (const RefPtr<AbstractIcon>& icon,
					const String& text)
	{
		RefPtr<Action> item(new Action(icon, text));

		return item;
	}

	RefPtr<Action> Action::Create (const RefPtr<AbstractIcon>& icon,
					const String& text, const String& shortcut)
	{
		RefPtr<Action> item(new Action(icon, text, shortcut));

		return item;
	}
	
	Action::Action ()
	{
	}

	Action::Action (const String& text)
	: text_(text)
	{
	}

	Action::Action (const String& text, const String& shortcut)
	: text_(text), shortcut_(shortcut)
	{
	}

	Action::Action (const RefPtr<AbstractIcon>& icon, const String& text)
	: icon_(icon), text_(text)
	{
	}

	Action::Action (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut)
	: icon_(icon), text_(text), shortcut_(shortcut)
	{
	}

	Action::~Action ()
	{
	}
	
	Size Action::GetHSize (const Font& font, const Margin& margin, int space)
	{
		Size size;

		Rect text_outline = font.GetTextOutline(text_);

		size.add_width(margin.left() + margin.right());
		if(icon_) {
			size.add_width(icon_->size().width());
			size.set_height(icon_->size().height());
		}
		size.add_width(space);
		size.add_width(text_outline.width());

		size.set_height(std::max(size.height(), text_outline.height()));

		text_outline = font.GetTextOutline(shortcut_);

		size.add_width(space);
		size.add_width(text_outline.width());

		size.set_height(std::max(size.height(), text_outline.height()));

		size.add_height(margin.top() + margin.bottom());

		return size;
	}

	unsigned int Action::GetTextLength (const Font& font)
	{
		unsigned int length = 0;

		Rect text_outline = font.GetTextOutline(text_);

		if(icon_) {
			length += icon_->size().width();
		}

		length += text_outline.width();

		text_outline = font.GetTextOutline(shortcut_);

		length += text_outline.width();

		return length;
	}

}

