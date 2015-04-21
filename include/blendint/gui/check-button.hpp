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

#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/abstract-button.hpp>

namespace BlendInt {

  /**
   * @brief Check button
   *
   * @ingroup blendint_gui_widgets_buttons
   */
	class CheckButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(CheckButton);

	public:

		CheckButton ();

		CheckButton (const String& text);

		virtual ~CheckButton ();

		virtual Size GetPreferredSize () const override;

	protected:

		virtual Response Draw (AbstractWindow* context) final;

	};
}
