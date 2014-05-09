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

#ifndef _BLENDINT_VERTICALLAYOUT_HPP_
#define _BLENDINT_VERTICALLAYOUT_HPP_

#include <list>
#include <set>

#include <BlendInt/Gui/AbstractLayout.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	class VLayoutExt: public AbstractLayoutExt
	{
	public:

		VLayoutExt (Context* context, int alignment = AlignCenter, int space = 2);

		~VLayoutExt ();

		virtual void Fill ();

	protected:

		virtual bool AddLayoutItem (AbstractWidget* widget);

		virtual bool RemoveLayoutItem (AbstractWidget* widget);

	private:

		void Distribute ();

		void Align ();

		WidgetDeque m_widgets;

		int m_alignment;

		int m_space;
	};

	// ------------------------------------

	class VBox: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(VBox);

	public:

		VBox(int align = AlignVerticalCenter, int space = 4);

		virtual ~VBox ();

		bool Add (AbstractWidget* widget);

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

	protected:

		virtual bool UpdateTest (const UpdateRequest& request);

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		/**
		 * @brief scan, distribute and align the items
		 */
		void FillSubWidgetsInVBox (const Point& out_pos, const Size& out_size, const Margin& margin, int space);

		void FillSubWidgetsInVBox (const Point& pos, const Size& size, int space);

		void FillSubWidgetsProportionally (int x, int y, unsigned int width, unsigned int height, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredHeight (int y,
						unsigned int height,
						int space,
						const std::deque<Size>* expandable_prefers,
						const std::deque<Size>* unexpandable_prefers);

		void DistributeWithSmallHeight (int y,
						unsigned int height,
						int space,
						const std::deque<Size>* expandable_prefers,
						unsigned int expandable_prefer_sum,
						const std::deque<Size>* unexpandable_prefers,
						unsigned int unexpandable_prefer_sum);

		void DistributeWithLargeHeight (int y,
						unsigned int height,
						int space,
						const std::deque<Size>* expandable_prefers,
						unsigned int expandable_prefer_sum,
						const std::deque<Size>* unexpandable_prefers,
						unsigned int unexpandable_prefer_sum);

		void Align (int x, unsigned int width);

		int m_alignment;

		int m_space;
	};

}

#endif	// _BLENDINT_VERTICALLAYOUT_HPP_
