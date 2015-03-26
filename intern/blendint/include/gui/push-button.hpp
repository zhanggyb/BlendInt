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
#include <gui/abstract-button.hpp>

namespace BlendInt {

	/**
	 * @brief The most common button class
	 *
	 * @ingroup blendint_gui_widgets_buttons
	 */
	class PushButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(PushButton);

	public:

		PushButton ();

		PushButton (const String& text);

		PushButton (const RefPtr<AbstractIcon>& icon);

		PushButton (const RefPtr<AbstractIcon>& icon, const String& text);

		virtual ~PushButton ();

		virtual Size GetPreferredSize () const;

		void SetText (const String& text);

		void SetIcon (const RefPtr<AbstractIcon>& icon);

	protected:

		virtual void PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height) final;

		virtual void PerformRoundTypeUpdate (int round_type) final;

		virtual void PerformRoundRadiusUpdate (float radius) final;

    virtual void PerformHoverIn (AbstractWindow* context) final;

    virtual void PerformHoverOut (AbstractWindow* context) final;

		virtual Response Draw (AbstractWindow* context) final;

	private:

		void InitializeButtonOnce ();

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> vbo_;

	};

}
