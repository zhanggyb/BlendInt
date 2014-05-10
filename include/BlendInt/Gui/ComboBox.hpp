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

#ifndef _BLENDINT_COMBOBOX_HPP_
#define _BLENDINT_COMBOBOX_HPP_

#include <BlendInt/Gui/RoundWidget.hpp>

#include <BlendInt/Gui/Shadow.hpp>
#include <BlendInt/Gui/Menu.hpp>

namespace BlendInt {

	/**
	 * @brief A combined button and popup list.
	 */
	class ComboBox: public RoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(ComboBox);

	public:

		ComboBox ();

		virtual ~ComboBox ();

		virtual Size GetPreferredSize () const;

	protected:

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

	private:

		void OnMenuActionTriggered (ActionItem* item);

		void InitOnce ();

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;

		bool m_status_down;

		Font m_font;

		String m_text;

		RefPtr<Menu> m_menu;

		static Margin default_combobox_padding;

	};

}

#endif /* _BLENDINT_COMBOBOX_HPP_ */
