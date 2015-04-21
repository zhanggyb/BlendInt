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

#pragma once

#include <blendint/core/margin.hpp>

#include <blendint/gui/abstract-menu-item.hpp>
#include <blendint/gui/action.hpp>

namespace BlendInt {

	class MenuItem: public AbstractMenuItem
	{
	public:

		MenuItem (const String& text);

		MenuItem (const RefPtr<Action>& action);

		MenuItem (const String& text, const String& shortcut);

		MenuItem (const RefPtr<AbstractIcon>& icon, const String& text);

		MenuItem (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut);

		virtual ~MenuItem();

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

    virtual void PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

	private:

		void InitializeMenuItem ();

    GLuint vao_;

    GLBuffer<> vbo_;

    bool hovered_;

		RefPtr<Action> action_;

		static Margin kPadding;

		static int kSpace;  // space between icon and text, or text and shortcut

	};

}
