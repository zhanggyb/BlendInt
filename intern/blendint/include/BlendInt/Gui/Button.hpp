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

#ifndef _BLENDINT_GUI_BUTTON_HPP_
#define _BLENDINT_GUI_BUTTON_HPP_

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

#include <BlendInt/Gui/AbstractIcon.hpp>

namespace BlendInt {

	/**
	 * @brief The most common button class
	 *
	 * @ingroup widgets
	 */
	class Button: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(Button);

	public:

		Button ();

		Button (const String& text);

		Button (const RefPtr<AbstractIcon>& icon);

		Button (const RefPtr<AbstractIcon>& icon, const String& text);

		virtual ~Button ();

		void SetIcon (const RefPtr<AbstractIcon>& icon);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (Profile& profile);

		void CalculateIconTextPosition (const Size& size, int round_type, float radius);

	private:

		void InitializeButtonOnce ();

		void InitializeButtonOnce (const String& text);

		void InitializeButtonOnce (const RefPtr<AbstractIcon>& icon, const String& text);

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		RefPtr<AbstractIcon> icon_;

		float icon_offset_x_;	// the offset along x to draw the icon
		float icon_offset_y_;

		bool show_icon_;
	};

}

#endif	// _BIL_BUTTON_H_
