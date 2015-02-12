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

#include <opengl/gl-buffer.hpp>

#include <gui/action.hpp>
#include <gui/abstract-button.hpp>

#include <gui/abstract-icon.hpp>

namespace BlendInt {

	/**
	 * @brief A special button usually used in ToolBar
	 */
	class ToolButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(ToolButton);

	public:

		ToolButton ();

		virtual ~ToolButton();

		void SetAction (const String& text);

		void SetAction (const String& text, const String& shortcut);

		void SetAction (const RefPtr<AbstractIcon>& AbstractIcon, const String& text);

		void SetAction (const RefPtr<AbstractIcon>& AbstractIcon, const String& text, const String& shortcut);

		void SetAction (const RefPtr<Action>& item);

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

	private:

		void InitializeToolButton ();

		GLuint vao_[2];

        GLBuffer<ARRAY_BUFFER, 2> vbo_;

		RefPtr<Action> action_;
	};
}
