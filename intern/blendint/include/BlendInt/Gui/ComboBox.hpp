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

#ifndef _BLENDINT_GUI_COMBOBOX_HPP_
#define _BLENDINT_GUI_COMBOBOX_HPP_

#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/ListView.hpp>

namespace BlendInt {

	/**
	 * @brief A combined button and popup list.
	 */
	class ComboBox: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(ComboBox);

	public:

		ComboBox ();

		virtual ~ComboBox ();

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		void SetModel (const RefPtr<AbstractItemModel>& model);

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual ResponseType Draw (Profile& profile);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

	private:

		void OnMenuActionTriggered (Action* item);

		void InitializeComboBox ();

		GLuint vaos_[2];

		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		bool status_down_;

		Font font_;

		String text_;

		ListView* list_;

		RefPtr<AbstractItemModel> model_;

		static Margin default_combobox_padding;

	};

}

#endif /* _BLENDINT_GUI_COMBOBOX_HPP_ */
