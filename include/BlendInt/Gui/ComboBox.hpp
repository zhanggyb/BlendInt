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

#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Gui/Menu.hpp>

namespace BlendInt {

	/**
	 * @brief A combined button and popup list.
	 */
	class ComboBox: public AbstractWidget
	{
		DISALLOW_COPY_AND_ASSIGN(ComboBox);

	public:

		ComboBox ();

		virtual ~ComboBox ();

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

	protected:

		virtual bool UpdateGeometryTest (const GeometryUpdateRequest& request);

		virtual void UpdateGeometry (const GeometryUpdateRequest& request);

		virtual void BroadcastUpdate (const GeometryUpdateRequest& request);

		virtual void ProcessSizeUpdate (const SizeUpdateRequest& request);

		virtual void ProcessRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void ProcessRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void OnMenuActionTriggered (Action* item);

		void InitializeComboBox ();

		GLuint m_vao[2];

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;

		bool m_status_down;

		Font m_font;

		String m_text;

		RefPtr<Menu> m_menu;

		static Margin default_combobox_padding;

	};

}

#endif /* _BLENDINT_COMBOBOX_HPP_ */
