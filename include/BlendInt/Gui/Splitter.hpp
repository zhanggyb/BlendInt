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

#ifndef _BLENDINT_GUI_SPLITTER_HPP_
#define _BLENDINT_GUI_SPLITTER_HPP_

#include <BlendInt/Gui/AbstractDequeContainer.hpp>
#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class Splitter;

	class SplitterHandle: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(SplitterHandle);

	public:

		friend class Splitter;

		SplitterHandle (Orientation orientation);

		virtual ~SplitterHandle ();

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		Orientation m_orientation;

		GLuint m_vao;

		bool m_highlight;
		bool m_pressed;

		RefPtr<GLArrayBuffer> m_buffer;

		Point m_last;
		Point m_cursor;

		unsigned int m_prev_size;	// width or height of the previous widget
		unsigned int m_next_size;	// width or height of the next widget
		int m_nearby_pos;	// nearby widget position

		AbstractWidget* m_prev_widget;
		AbstractWidget* m_next_widget;

	};

	class Splitter: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Splitter);

	public:

		friend class SplitterHandle;

		Splitter (Orientation orientation = Horizontal);

		virtual ~Splitter ();

		void PushFront (AbstractWidget* widget);

		void PushBack (AbstractWidget* widget);

		void Remove (AbstractWidget* widget);

		int GetWidgetIndex (AbstractWidget* widget) const;

		int GetHandleIndex (SplitterHandle* handle) const;

		AbstractWidget* GetWidget (int index) const;

		SplitterHandle* GetHandle (int index) const;

		int GetWidgetCount () const;

		void MoveHandle (int index, int x, int y);

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

	protected:

		virtual void UpdateContainer (const WidgetUpdateRequest& request);

		virtual bool UpdateGeometryTest (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void FillSubWidgetsInSplitter (const Point& out_pos, const Size& out_size, const Margin& margin, Orientation orientation);

		void FillSubWidgetsInSplitter (const Point& pos, const Size& size, Orientation orientation);

		/**
		 * @brief Fill the sub widgets in this container based on the current size
		 * @param x
		 * @param y
		 * @param width
		 * @param height
		 * @param orientation
		 */
		void FillSubWidgetsInSplitter (int x, int y, int width, int height, Orientation orientation);

	private:

		void DistributeHorizontally (int x, int width);

		void DistributeVertically (int y, int height);

		void AlignHorizontally (int y, int height);

		void AlignVertically (int x, int width);

		void AlignSubWidgets (Orientation orientation, const Size& size, const Margin& margin);

		int GetAverageRoom (Orientation orientation, const Size& size, const Margin& margin);

		int GetWidgetsRoom (Orientation orientation, const Size& size, const Margin& margin);

		Orientation m_orientation;
	};
}


#endif /* _BLENDINT_GUI_SPLITTER_HPP_ */
