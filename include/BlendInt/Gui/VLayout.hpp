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

#ifndef _BLENDINT_GUI_VBOX_HPP_
#define _BLENDINT_GUI_VBOX_HPP_

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	class VBox: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(VBox);

	public:

		VBox(int align = AlignVerticalCenter, int space = 4);

		virtual ~VBox ();

		bool PushBack (AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		int alignment () const
		{
			return m_alignment;
		}

		void SetAlignment (int align);

		void SetSpace (int space);

		int space () const
		{
			return m_space;
		}

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void PerformMarginUpdate (const Margin& request);

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void FillSubWidgetsInVBox (const Point& out_pos, const Size& out_size, const Margin& margin, int alignment, int space);

		void FillSubWidgetsInVBox (const Point& pos, const Size& size, int alignment, int space);

		void FillSubWidgetsProportionallyInVBox (int x, int y, int width, int height, int alignment, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredHeight (int y,
						int height,
						int space,
						const std::deque<int>* expandable_preferred_heights,
						const std::deque<int>* unexpandable_preferred_heights);

		void DistributeWithSmallHeight (int y,
						int height,
						int space,
						const std::deque<int>* expandable_preferred_heights,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_heights,
						int unexpandable_prefer_sum);

		void DistributeWithLargeHeight (int y,
						int height,
						int space,
						const std::deque<int>* expandable_preferred_heights,
						int expandable_prefer_sum,
						const std::deque<int>* unexpandable_preferred_heights,
						int unexpandable_prefer_sum);

		void AlignInVBox (int x, int width, int alignment, const std::deque<int>* unexpandable_preferred_widths);

		void set_alignment (int align)
		{
			m_alignment = align;
		}

		void set_space (int space)
		{
			m_space = space;
		}

	private:

		int m_alignment;

		int m_space;
	};

}

#endif /* _BLENDINT_GUI_VBOX_HPP_ */
