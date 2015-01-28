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

#ifndef _BLENDINT_GUI_SCREENSPLITTER_HPP_
#define _BLENDINT_GUI_SCREENSPLITTER_HPP_

#include <deque>
#include <BlendInt/Core/Types.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class FrameSplitter;

	class FrameSplitterHandle: public Frame
	{
	public:

		virtual Size GetPreferredSize () const;

		virtual bool Contain (const Point& point) const;

		virtual ~FrameSplitterHandle ();

	protected:

		friend class FrameSplitter;

		FrameSplitterHandle (Orientation orientation = Horizontal);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (const AbstractWindow* context);

		virtual ResponseType Draw (const AbstractWindow* context);

		virtual void PostDraw (const AbstractWindow* context);

		virtual void PerformFocusOn (const AbstractWindow* context);

		virtual void PerformFocusOff (const AbstractWindow* context);

		virtual void PerformHoverIn (const AbstractWindow* context);

		virtual void PerformHoverOut (const AbstractWindow* context);

		virtual ResponseType PerformKeyPress (const AbstractWindow* context);

		virtual ResponseType PerformContextMenuPress (const AbstractWindow* context);

		virtual ResponseType PerformContextMenuRelease (const AbstractWindow* context);

		virtual ResponseType PerformMousePress (const AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (const AbstractWindow* context);

		virtual ResponseType PerformMouseMove (const AbstractWindow* context);

		virtual ResponseType DispatchHoverEvent (const AbstractWindow* context);

	private:

		Orientation orientation_;

		GLuint vao_;

		GLBuffer<ARRAY_BUFFER> buffer_;

		Point last_;
		Point cursor_;

		int prev_size_;
		int next_size_;
		int nearby_pos_;
	};

	// -------------------------------

	class FrameSplitter: public Frame
	{
	public:

		FrameSplitter (Orientation orientation = Horizontal);

		virtual ~FrameSplitter ();

		void AddFrame (Frame* frame, SizePolicy policy = DefaultSizePolicy);

		void InsertFrame (int index, Frame* frame, SizePolicy policy = DefaultSizePolicy);

		int GetFrameIndex (Frame* frame) const;

		int GetHandleIndex (FrameSplitterHandle* handle) const;

		Frame* GetFrame (int index) const;

		FrameSplitterHandle* GetHandle (int index) const;

		int GetFramesCount () const;

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		virtual AbstractView* GetFocusedView () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (const AbstractWindow* context);

		virtual ResponseType Draw (const AbstractWindow* context);

		virtual void PostDraw (const AbstractWindow* context);

		virtual void PerformFocusOn (const AbstractWindow* context);

		virtual void PerformFocusOff (const AbstractWindow* context);

		virtual void PerformHoverIn (const AbstractWindow* context);

		virtual void PerformHoverOut (const AbstractWindow* context);

		virtual ResponseType PerformKeyPress (const AbstractWindow* context);

		virtual ResponseType PerformMousePress (const AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (const AbstractWindow* context);

		virtual ResponseType PerformMouseMove (const AbstractWindow* context);

		virtual ResponseType DispatchHoverEvent (const AbstractWindow* context);

		virtual bool RemoveSubView (AbstractView* view);

		void FillSubFrames ();

	private:

		friend class FrameSplitterHandle;

		void AddColumn (Frame* frame, SizePolicy policy);

		void AddRow (Frame* frame, SizePolicy policy);

		void InsertColumn (int index, Frame* frame, SizePolicy policy);

		void InsertRow (int index, Frame* frame, SizePolicy policy);

		void DistributeHorizontally ();

		void DistributeHorizontallyInProportion (
		        std::deque<int>* widget_deque, int widget_width_sum,
		        std::deque<int>* prefer_deque, int prefer_width_sum);

		void DistributeExpandableFramesHorizontally (
		        int unexpandable_width_sum, std::deque<int>* widget_deque,
		        int widget_width_sum, std::deque<int>* prefer_deque,
		        int prefer_width_sum);

		void DistributeUnexpandableFramesHorizontally (
		        std::deque<int>* widget_deque, int widget_width_sum,
		        std::deque<int>* prefer_deque, int prefer_width_sum);

		void DistributeVertically ();

		void DistributeVerticallyInProportion (
		        std::deque<int>* widget_deque, int widget_height_sum,
		        std::deque<int>* prefer_deque, int prefer_height_sum);

		void DistributeExpandableFramesVertically (
		        int unexpandable_height_sum, std::deque<int>* widget_deque,
		        int widget_height_sum, std::deque<int>* prefer_deque,
		        int prefer_height_sum);

		void DistributeUnexpandableFramesVertically (
		        std::deque<int>* widget_deque, int widget_height_sum,
		        std::deque<int>* prefer_deque, int prefer_height_sum);

		void AlignHorizontally ();
        
        // this will replace AlignHorizontally later
        void AlignHorizontallyExt ();

		void AlignVertically ();
        
        // this will replace AlignVertically later
        void AlignVerticallyExt ();

		void AlignSubFrames (Orientation orientation, const Size& size);

		int GetAverageRoom (Orientation orientation, const Size& size);

		void SetFocusedFrame (AbstractFrame* frame, const AbstractWindow* context);

		void SetHoveredFrame (const AbstractWindow* context);

		Orientation orientation_;

		AbstractFrame* hover_frame_;

		AbstractFrame* focused_frame_;
	};
}

#endif /* _BLENDINT_GUI_SCREENSPLITTER_HPP_ */
