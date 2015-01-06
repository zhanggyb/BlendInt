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

#ifndef _BLENDINT_GUI_TOGGLEBUTTON_HPP_
#define _BLENDINT_GUI_TOGGLEBUTTON_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

#include <BlendInt/Gui/AbstractIcon.hpp>

namespace BlendInt {

	class String;

	/**
	 * @brief Toggle Button
	 *
	 * @ingroup widgets
	 */
	class ToggleButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(ToggleButton);

	public:

		ToggleButton ();

		ToggleButton (const String& text);

		ToggleButton (const RefPtr<AbstractIcon>& icon);

		ToggleButton (const RefPtr<AbstractIcon>& icon, const String& text);

		virtual ~ToggleButton ();

		void SetIcon (const RefPtr<AbstractIcon>& icon);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (const Context* context);

		void CalculateIconTextPosition (const Size& size, int round_type, float radius);

	private:

		void InitializeToggleButtonOnce ();

		void InitializeToggleButtonOnce (const String& text);

		void InitializeToggleButtonOnce (const RefPtr<AbstractIcon>& icon, const String& text);

		GLuint vao_[2];

		RefPtr<GLArrayBuffer> inner_;

		RefPtr<GLArrayBuffer> outer_;

		RefPtr<AbstractIcon> icon_;

		float icon_offset_x_;	// the offset along x to draw the icon
		float icon_offset_y_;

		bool show_icon_;
	};

}

#endif /* _BIL_CHECKBUTTON_HPP_ */
