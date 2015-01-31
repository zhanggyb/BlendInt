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

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/PopupFrame.hpp>

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

		virtual ResponseType Draw (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual ResponseType PerformMousePress (AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (AbstractWindow* context);

	private:

		void InitializeComboBox ();

		void OnPopupListDestroyed (AbstractFrame* frame);

		GLuint vaos_[2];

		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		bool status_down_;

		Font font_;

		String text_;

		PopupFrame* popup_;

		RefPtr<AbstractItemModel> model_;

		static Margin default_combobox_padding;

	};

}
