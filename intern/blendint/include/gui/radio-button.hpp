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
   * @brief Radio button
   *
   * @ingroup blendint_gui_widgets_buttons
   */
	class RadioButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(RadioButton);

	public:

		RadioButton ();

		RadioButton (const String& test);

		RadioButton (const RefPtr<AbstractIcon>& icon);

		RadioButton (const RefPtr<AbstractIcon>& icon, const String& text);

		virtual ~RadioButton ();

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request) final;

		virtual void PerformRoundTypeUpdate (int round_type) final;

		virtual void PerformRoundRadiusUpdate (float radius) final;

		virtual Response Draw (AbstractWindow* context) override;

	private:

		void InitializeRadioButtonOnce ();

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> vbo_;

	};
}
