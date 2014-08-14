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

#ifndef _BLENDINT_GUI_TOOLBUTTON_HPP_
#define _BLENDINT_GUI_TOOLBUTTON_HPP_

#include <BlendInt/Gui/Action.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

#include <BlendInt/Gui/PixelIcon.hpp>

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

		void SetAction (const RefPtr<Icon>& icon, const String& text);

		void SetAction (const RefPtr<Icon>& icon, const String& text, const String& shortcut);

		void SetAction (const RefPtr<Action>& item);

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (const Profile& profile);

	private:

		void InitializeToolButton ();

		GLuint vaos_[2];

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		RefPtr<Action> action_;

		RefPtr<Icon> icon_;
	};
}

#endif /* _BLENDINT_GUI_TOOLBUTTON_HPP_ */
