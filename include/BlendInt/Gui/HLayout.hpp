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

#ifndef _BLENDINT_HORIZONTALLAYOUT_HPP_
#define _BLENDINT_HORIZONTALLAYOUT_HPP_

#include <list>
#include <set>

#include <BlendInt/Core/RefPtr.hpp>

#include <BlendInt/Gui/AbstractDequeContainer.hpp>

namespace BlendInt {

	class HBox: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(HBox);

	public:

		HBox(int align = AlignHorizontalCenter, int space = 4);

		virtual ~HBox ();

		bool Add (AbstractWidget* obj);

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		bool Remove (AbstractWidget* object);

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

		/**
		 * @brief Add a Widget object into this layout
		 * @param object
		 *
		 * Change and lock the object geometry, and put this widget into HorizontalLayout
		 *
		 */
		virtual void AddItem (AbstractWidget* object);

		virtual void RemoveItem (AbstractWidget* object);

		/**
		 * @brief Count how many widgets are expandable along x
		 */
		unsigned int CountHExpandableNumber ();

	private:

		/**
		 * @brief scan, distribute and align the items
		 */
		void FillSubWidgetsInHBox (const Size& size, const Margin& margin, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredWidth (int x, int space, const std::deque<Size>* list);

		/**
		 * @brief distribute horizontally with small size
		 */
		void DistributeWithSmallWidth (int x, int space, const std::deque<Size>* list);

		/**
		 * @brief distribute horizontally with large size
		 */
		void DistributeWithLargeWidth (int x, int space, const std::deque<Size>* list);

		void Distribute (int space, int start = 0);

		/**
		 * @brief align horizontally
		 */
		void Align (const Size* size, const Margin* margin);

		unsigned int AdjustExpandableWidth (std::list<AbstractWidget*>* item_list_p, unsigned int width_plus);

		unsigned int AdjustMinimalWidth (std::list<AbstractWidget*>* item_list_p, unsigned int width_plus);

		/**
		 * @brief calculate and return the minimal width of the expandable items
		 * @return
		 */
		unsigned int GetAllMinimalExpandableWidth ();

		unsigned int GetAllMaximalExpandableWidth ();

		/**
		 * @brief calculate and return the width of fixed items
		 * @return
		 */
		unsigned int GetAllFixedWidth ();

		/**
		 * @brief scan the children and get the total size hint
		 * @param[in] if count margin
		 * @param[in] if count space
		 * @param[out] size the layout size
		 * @param[out] min the layout minimal size
		 * @param[out] prefer the layout preferred size
		 */
		void GetSizeHint (bool count_margin, bool count_space, Size* size, Size* min, Size* prefer);

		int m_alignment;

		int m_space;

	};

}

#endif	// _BIL_HORIZONTALLAYOUT_H_
