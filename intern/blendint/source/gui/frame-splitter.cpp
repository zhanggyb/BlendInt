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

#include <gui/frame-splitter.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	FrameSplitterHandle::FrameSplitterHandle(Orientation orientation)
	: AbstractFrame(),
	  orientation_(orientation),
	  prev_size_(0),
	  next_size_(0),
	  nearby_pos_(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 1);
		} else {
			set_size(1, 200);
		}
	}

	FrameSplitterHandle::~FrameSplitterHandle()
	{
	}

	bool FrameSplitterHandle::Contain(const Point& point) const
	{
		if(orientation_ == Horizontal) {

			if(point.x() < position().x() ||
					point.y() < (position().y() - 2) ||
					point.x() > static_cast<int>(position().x() + size().width()) ||
					point.y() > static_cast<int>(position().y() + size().height() + 2))
			{
				return false;
			}

		} else {

			if(point.x() < (position().x() - 2) ||
					point.y() < position().y() ||
					point.x() > static_cast<int>(position().x() + size().width() + 2) ||
					point.y() > static_cast<int>(position().y() + size().height()))
			{
				return false;
			}

		}

		return true;
	}

	bool FrameSplitterHandle::IsExpandX() const
	{
		if(orientation_ == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool FrameSplitterHandle::IsExpandY() const
	{
		if(orientation_ == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	Size FrameSplitterHandle::GetPreferredSize() const
	{
		if(orientation_ == Horizontal) {
			return Size(200, 1);
		} else {
			return Size(1, 200);
		}
	}

	bool FrameSplitterHandle::PreDraw(AbstractWindow* context)
	{
		return false;
	}

	Response FrameSplitterHandle::Draw(AbstractWindow* context)
	{
		return Finish;
	}

	void FrameSplitterHandle::PostDraw(AbstractWindow* context)
	{
	}

	void FrameSplitterHandle::PerformFocusOn(AbstractWindow* context)
	{

	}

	void FrameSplitterHandle::PerformFocusOff (AbstractWindow* context)
	{

	}

	Response FrameSplitterHandle::PerformKeyPress(AbstractWindow* context)
	{
		return Ignore;
	}

	Response FrameSplitterHandle::PerformContextMenuPress(
			AbstractWindow* context)
	{
		return Ignore;
	}

	Response FrameSplitterHandle::PerformContextMenuRelease(
			AbstractWindow* context)
	{
		return Ignore;
	}

	Response FrameSplitterHandle::PerformMousePress(
			AbstractWindow* context)
	{
		last_ = position();
		cursor_ = context->GetCursorPosition();

		if(orientation_ == Horizontal) {
			prev_size_ = previous_view()->size().height();
			next_size_ = next_view()->size().height();
			nearby_pos_ = previous_view()->position().y();
		} else {
			prev_size_ = previous_view()->size().width();
			next_size_ = next_view()->size().width();
			nearby_pos_ = next_view()->position().x();
		}

		set_pressed(true);

		return Finish;
	}

	Response FrameSplitterHandle::PerformMouseRelease(
			AbstractWindow* context)
	{
		if(!hover()) {
			context->PopCursor();
		}

		set_pressed(false);
		return Finish;
	}

	void FrameSplitterHandle::PerformHoverIn(AbstractWindow* context)
	{
		context->PushCursor();
		if(orientation_ == Horizontal) {
			context->SetCursor(SplitVCursor);
		} else {
			context->SetCursor(SplitHCursor);
		}

		//RequestRedraw();
	}

	void FrameSplitterHandle::PerformHoverOut(AbstractWindow* context)
	{
		if(!pressed_ext())
			context->PopCursor();

		//RequestRedraw();
	}

	Response FrameSplitterHandle::PerformMouseHover(AbstractWindow* context)
	{
		if(Contain(context->GetCursorPosition())) {
			return Finish;
		} else {
			return Ignore;
		}
	}

	Response FrameSplitterHandle::PerformMouseMove(AbstractWindow* context)
	{
		if(pressed_ext()) {

			FrameSplitter* splitter = dynamic_cast<FrameSplitter*>(superview());
			assert(splitter);

			if(orientation_ == Horizontal) {

				int offset = context->GetCursorPosition().y() - cursor_.y();
				int oy1 = prev_size_ - offset;
				int oy2 = next_size_ + offset;

				if((oy1 <= 0) || (oy2 <= 0)) {
					return Finish;
				}

				splitter->MoveSubViewTo(this, last_.x(), last_.y() + offset);

				splitter->ResizeSubView(previous_view(), previous_view()->size().width(), oy1);
				splitter->MoveSubViewTo(previous_view(), previous_view()->position().x(), nearby_pos_ + offset);
				splitter->ResizeSubView(next_view(), next_view()->size().width(), oy2);

			} else {

				int offset = context->GetCursorPosition().x() - cursor_.x();
				int oy1 = prev_size_ + offset;
				int oy2 = next_size_ - offset;

				if((oy1 <= 0) || (oy2 <= 0)) {
					return Finish;
				}

				splitter->MoveSubViewTo(this, last_.x() + offset, last_.y());

				splitter->ResizeSubView(previous_view(), oy1, previous_view()->size().height());
				splitter->ResizeSubView(next_view(), oy2, next_view()->size().height());
				splitter->MoveSubViewTo(next_view(), nearby_pos_ + offset, next_view()->position().y());

			}

			RequestRedraw();
			return Finish;
		}
		return Finish;
	}

	// --------------------------------

	FrameSplitter::FrameSplitter(Orientation orientation)
	: AbstractFrame(),
	  orientation_(orientation),
	  hover_frame_(0),
	  focused_frame_(0)
	{
		set_size(500, 500);
	}

	FrameSplitter::~FrameSplitter()
	{

	}

	void FrameSplitter::AddFrame (AbstractFrame* frame, SizePolicy policy)
	{
		if((frame == 0) || (frame->superview() == this)) return;

		if(subs_count() == 0) {
			PushBackSubView(frame);
			AlignSubFrames(orientation_, size());
		} else {

			if(orientation_ == Horizontal) {
				AddColumn(frame, policy);
			} else {
				AddRow(frame, policy);
			}

		}

		RequestRedraw();
	}

	void FrameSplitter::InsertFrame(int index, AbstractFrame* frame, SizePolicy policy)
	{
		if((frame == 0) || (frame->superview() == this)) return;

		if(subs_count() == 0) {
			PushBackSubView(frame);
			AlignSubFrames(orientation_, size());
		} else {

			if(orientation_ == Horizontal) {
				InsertColumn(index, frame, policy);
			} else {
				InsertRow(index, frame, policy);
			}

		}

		RequestRedraw();
	}

	int FrameSplitter::GetFrameIndex (AbstractFrame* frame) const
	{
		if(frame->superview() != this) return -1;

		int index = 0;
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p == frame) break;

			index++;
			p = p->next_view()->next_view();
		}

		return index;
	}

	int FrameSplitter::GetHandleIndex (FrameSplitterHandle* handle) const
	{
		if(handle->superview() != this) return -1;

		int index = 0;
		for(AbstractView* p = first_subview()->next_view(); p; p = p->next_view())
		{
			if(p == handle) break;

			index++;
			p = p->next_view()->next_view();
		}

		return index;
	}

	AbstractFrame* FrameSplitter::GetFrame (int index) const
	{
		if(subs_count() == 0) return 0;

		int max = (subs_count() + 1) / 2;
		if(index > max) return 0;

		index = index * 2;

		return dynamic_cast<AbstractFrame*>(GetSubViewAt(index));
	}

	FrameSplitterHandle* FrameSplitter::GetHandle (int index) const
	{
		if(subs_count() <= 1) return 0;

		int max = (subs_count() - 1) / 2;
		if(index > max) return 0;

		index = index * 2 + 1;

		return dynamic_cast<FrameSplitterHandle*>(GetSubViewAt(index));
	}

	int FrameSplitter::GetFramesCount () const
	{
		int sum = subs_count();
		return (sum / 2 + 1);
	}

	bool FrameSplitter::IsExpandX() const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	bool FrameSplitter::IsExpandY() const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}

	Size FrameSplitter::GetPreferredSize() const
	{
		Size preferred_size;

		if(first_subview() == 0) {
			preferred_size.set_width(400);
			preferred_size.set_height(400);
		} else {
			Size tmp;

			if (orientation_ == Horizontal) {
				for(AbstractView* p = first_subview(); p; p = p->next_view())
				{
					if (p->visiable()) {
						tmp = p->GetPreferredSize();
						preferred_size.add_width(tmp.width());
						preferred_size.set_height(
										std::max(preferred_size.height(),
														tmp.height()));
					}
				}
			} else {
				for(AbstractView* p = first_subview(); p; p = p->next_view())
				{
					if(p->visiable()) {
						tmp = p->GetPreferredSize();
						preferred_size.add_height(tmp.height());
						preferred_size.set_width(
										std::max(preferred_size.width(), tmp.width()));
					}
				}
			}

		}

		return preferred_size;
	}

	AbstractView* FrameSplitter::GetFocusedView() const
	{
		return focused_frame_;
	}

	void FrameSplitter::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int ox = request.position()->x() - position().x();
			int oy = request.position()->y() - position().y();

			set_position(*request.position());

			for (AbstractView* p = first_subview(); p; p = p->next_view()) {
				MoveSubViewTo(p, p->position().x() + ox,
				        p->position().y() + oy);
			}

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void FrameSplitter::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
			FillSubFrames();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool FrameSplitter::PreDraw(AbstractWindow* context)
	{
		return visiable();
	}

	Response FrameSplitter::Draw(AbstractWindow* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void FrameSplitter::PostDraw(AbstractWindow* context)
	{
	}

	void FrameSplitter::AlignSubFrames(Orientation orientation,
			const Size& size)
	{
		int room = GetAverageRoom(orientation, size);
		int x = position().x();

		if(orientation == Horizontal) {

			int y = position().y();
			int h = size.height();

			int i = 0;
			int handler_width = 0;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(i % 2 == 0) {
					ResizeSubView(p, room, h);
					MoveSubViewTo(p, x, y);
					x = x + room;
				} else {
					handler_width = p->GetPreferredSize().width();
					ResizeSubView(p, handler_width, h);
					MoveSubViewTo(p, x, y);
					x = x + handler_width;
				}
				i++;
			}

		} else {

			int y = position().y() + this->size().height();
			int w = size.width();

			int i = 0;
			int handler_height = 0;
			for(AbstractView* p = first_subview(); p; p = p->next_view())
			{
				if(i % 2 == 0) {
					y = y - room;
					ResizeSubView(p, w, room);
					MoveSubViewTo(p, x, y);
				} else {
					handler_height = p->GetPreferredSize().height();
					y = y - handler_height;
					ResizeSubView(p, w, handler_height);
					MoveSubViewTo(p, x, y);
				}

				i++;
			}

		}

	}

	void FrameSplitter::PerformFocusOn (AbstractWindow* context)
	{
	}

	void FrameSplitter::PerformFocusOff (AbstractWindow* context)
	{
	}

	void FrameSplitter::PerformHoverIn(AbstractWindow* context)
	{
	}

	void FrameSplitter::PerformHoverOut(AbstractWindow* context)
	{
		if(hover_frame_) {
			delegate_mouse_hover_out_event(hover_frame_, context);
			hover_frame_ = nullptr;
		}
	}

	Response FrameSplitter::PerformKeyPress(AbstractWindow* context)
	{
		if(focused_frame_) {
			return delegate_key_press_event(focused_frame_, context);
		}

		return Ignore;
	}

	Response FrameSplitter::PerformMousePress(AbstractWindow* context)
	{
		Response response = Ignore;
		set_pressed(true);

		if(hover_frame_ != nullptr) {
			response = delegate_mouse_press_event(hover_frame_, context);

			if(response == Finish) {
				SetFocusedFrame(hover_frame_, context);
			}
		} else {
			SetFocusedFrame(0, context);
		}

		return Finish;
	}

	Response FrameSplitter::PerformMouseRelease(AbstractWindow* context)
	{
		Response response = Ignore;
		set_pressed(false);

		if(focused_frame_ != nullptr) {
			response = delegate_mouse_release_event(focused_frame_, context);
		}

		return response;
	}

	Response FrameSplitter::PerformMouseMove(AbstractWindow* context)
	{
		Response response = Ignore;

		if(pressed_ext() && focused_frame_) {
			response = delegate_mouse_move_event(focused_frame_, context);
		}

		return response;
	}

	Response FrameSplitter::PerformMouseHover(AbstractWindow* context)
	{
		if(Contain(context->GetCursorPosition())) {

			Response response = Finish;
			SetHoveredFrame(context);
			if(hover_frame_ != nullptr) {
				response = delegate_dispatch_hover_event(hover_frame_, context);
			}

			return response;

		} else {
			return Ignore;
		}
	}

	void FrameSplitter::FillSubFrames()
	{
		if(orientation_ == Horizontal) {

			DistributeHorizontally();
			AlignHorizontally();

		} else {

			DistributeVertically();
			AlignVertically();

		}
	}

	int FrameSplitter::GetAverageRoom(Orientation orientation,
			const Size& size)
	{
		int retval = 0;

		if(orientation == Horizontal) {
			retval = size.width();
		} else {
			retval = size.height();
		}

		if(first_subview() == 0) {
			return retval;
		}

		Size prefer;
		int space = 0;
		int sum = 1;

		// get all the total width/height of splitter handlers
		AbstractView* p = first_subview()->next_view();
		while (p) {
			prefer = p->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			p = p->next_view()->next_view();
			sum += 2;
		}

		retval = retval - space;
		retval = retval / ((sum + 1) / 2);

		return retval;
	}

	void FrameSplitter::AddColumn(AbstractFrame* frame, SizePolicy policy)
	{
#ifdef DEBUG
		assert(orientation_ == Horizontal);
#endif

		Size squeezed_size = size();
		Size frame_size = frame->size();
		FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Vertical));

		switch (policy) {
			case PreferredWidth: {
				frame_size.set_width(frame->GetPreferredSize().width());
				break;
			}
			case CurrentWidth: {
				break;
			}
			case ExpandX: {
				int w = 0;
				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					w = p->GetPreferredSize().width();
				}
				frame_size.set_height(size().width() - handle->size().width() - w);
				break;
			}
			default: {
				break;
			}
		}
		squeezed_size.set_width(squeezed_size.width() - handle->size().width() - frame_size.width());

		if((squeezed_size.width() < 0) || (policy == DefaultSizePolicy)) {
			PushBackSubView(handle);
			PushBackSubView(frame);
			AlignSubFrames(orientation_, size());
		} else {
			AlignSubFrames(orientation_, squeezed_size);

			Point pos = last_subview()->position();
			pos.set_x(pos.x() + last_subview()->size().width());

			PushBackSubView(handle);
			PushBackSubView(frame);

			ResizeSubView(handle, handle->size().width(), size().height());
			frame_size.set_height(size().height());
			ResizeSubView(frame, frame_size);

			MoveSubViewTo(handle, pos);
			pos.set_x(pos.x() + handle->size().width());
			MoveSubViewTo(frame, pos);

		}

	}

	void FrameSplitter::AddRow(AbstractFrame* frame, SizePolicy policy)
	{
#ifdef DEBUG
		assert(orientation_ == Vertical);
#endif

		Size squeezed_size = size();
		Size frame_size = frame->size();
		FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Horizontal));

		switch (policy) {
			case PreferredHeight: {
				frame_size.set_height(frame->GetPreferredSize().height());
				break;
			}
			case CurrentHeight: {
				break;
			}
			case ExpandY: {
				int h = 0;
				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					h = p->GetPreferredSize().height();
				}
				frame_size.set_height(size().height() - handle->size().height() - h);
				break;
			}

			default: {
				break;
			}
		}
		squeezed_size.set_height(size().height() - handle->size().height() - frame_size.height());

		if((squeezed_size.height() < 0) || (policy == DefaultSizePolicy)) {
			PushBackSubView(handle);
			PushBackSubView(frame);
			AlignSubFrames(orientation_, size());
		} else {
			AlignSubFrames(orientation_, squeezed_size);

			Point pos = last_subview()->position();

			PushBackSubView(handle);
			PushBackSubView(frame);

			ResizeSubView(handle, size().width(), handle->size().height());
			frame_size.set_width(size().width());
			ResizeSubView(frame, frame_size);

			pos.set_y(pos.y() - handle->size().height());
			MoveSubViewTo(handle, pos);
			pos.set_y(pos.y() - frame->size().height());
			MoveSubViewTo(frame, pos);

		}

	}

	void FrameSplitter::InsertColumn(int index, AbstractFrame* frame, SizePolicy policy)
	{
#ifdef DEBUG
		assert(orientation_ == Horizontal);
#endif

		Size squeezed_size = size();
		Size frame_size = frame->size();
		FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Vertical));

		switch (policy) {
			case PreferredWidth: {
				frame_size.set_width(frame->GetPreferredSize().width());
				break;
			}
			case CurrentWidth: {
				break;
			}
			case ExpandX: {
				int w = 0;
				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					w = p->GetPreferredSize().width();
				}
				frame_size.set_height(size().width() - handle->size().width() - w);
				break;
			}
			default: {
				break;
			}
		}
		squeezed_size.set_width(squeezed_size.width() - handle->size().width() - frame_size.width());

		if((squeezed_size.width() < 0) || (policy == DefaultSizePolicy)) {

			index = index * 2;

			if(index > (subs_count() - 1)) {
				// append
				InsertSubView(index, handle);
				InsertSubView(index + 1, frame);
			} else {
				InsertSubView(index, frame);
				InsertSubView(index + 1, handle);
			}

			AlignSubFrames(orientation_, size());

		} else {

			index = index * 2;

			if(index > (subs_count() - 1)) {
				// append
				InsertSubView(index, handle);
				InsertSubView(index + 1, frame);
			} else {
				InsertSubView(index, frame);
				InsertSubView(index + 1, handle);
			}

			ResizeSubView(handle, handle->size().width(), size().height());
			frame_size.set_height(size().height());
			ResizeSubView(frame, frame_size);

			//AlignSubFrames(orientation_, squeezed_size);
			AlignSubFrames(orientation_, size());
		}
	}

	void FrameSplitter::InsertRow(int index, AbstractFrame* frame, SizePolicy policy)
	{
#ifdef DEBUG
		assert(orientation_ == Vertical);
#endif

		Size squeezed_size = size();
		Size frame_size = frame->size();
		FrameSplitterHandle* handle = Manage(new FrameSplitterHandle(Horizontal));

		switch (policy) {
			case PreferredHeight: {
				frame_size.set_height(frame->GetPreferredSize().height());
				break;
			}
			case CurrentHeight: {
				break;
			}
			case ExpandY: {
				int h = 0;
				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					h = p->GetPreferredSize().height();
				}
				frame_size.set_height(size().height() - handle->size().height() - h);
				break;
			}

			default: {
				break;
			}
		}
		squeezed_size.set_height(size().height() - handle->size().height() - frame_size.height());

		if((squeezed_size.height() < 0) || (policy == DefaultSizePolicy)) {

			index = index * 2;

			if(index > (subs_count() - 1)) {
				// append
				InsertSubView(index, handle);
				InsertSubView(index + 1, frame);
			} else {
				InsertSubView(index, frame);
				InsertSubView(index + 1, handle);
			}

			AlignSubFrames(orientation_, size());

		} else {

			index = index * 2;

			if(index > (subs_count() - 1)) {
				// append
				InsertSubView(index, handle);
				InsertSubView(index + 1, frame);
			} else {
				InsertSubView(index, frame);
				InsertSubView(index + 1, handle);
			}

			ResizeSubView(handle, size().width(), handle->size().height());
			frame_size.set_width(size().width());
			ResizeSubView(frame, frame_size);

			//AlignSubFrames(orientation_, squeezed_size);
			AlignSubFrames(orientation_, size());

		}
	}

	void FrameSplitter::DistributeHorizontally()
	{
		boost::scoped_ptr<std::deque<int> > expandable_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_widths(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > handler_prefer_widths (new std::deque<int>);

		int expandable_width_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_width_sum = 0;	// the width sum of the unexpandable widgets' size
		int handlers_width_sum = 0;

		int prefer_width;
		int i = 0;
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {	// widgets

				if (p->visiable()) {

					if(p->IsExpandX()) {
						expandable_width_sum += p->size().width();
						expandable_widths->push_back(p->size().width());
					} else {
						unexpandable_width_sum += p->size().width();
						unexpandable_widths->push_back(p->size().width());
					}

				}

			} else {	// handlers

				if(p->visiable()) {
					prefer_width = p->GetPreferredSize().width();
					handler_prefer_widths->push_back(prefer_width);
					handlers_width_sum += prefer_width;
				}

			}

			i++;
		}

		if((expandable_widths->size() + unexpandable_widths->size()) == 0) return;	// do nothing if all sub widgets are invisible

		if(expandable_widths->size() == 0) {

			DistributeHorizontallyInProportion(unexpandable_widths.get(), unexpandable_width_sum,
							handler_prefer_widths.get(), handlers_width_sum);

		} else if(unexpandable_widths->size() == 0) {

			DistributeHorizontallyInProportion(expandable_widths.get(), expandable_width_sum,
							handler_prefer_widths.get(), handlers_width_sum);

		} else {

			int exp_width = size().width() - handlers_width_sum - unexpandable_width_sum;

			if(exp_width <= 0) {

				DistributeUnexpandableFramesHorizontally(
								unexpandable_widths.get(),
								unexpandable_width_sum,
								handler_prefer_widths.get(),
								handlers_width_sum);

			} else {

				DistributeExpandableFramesHorizontally(
								unexpandable_width_sum, expandable_widths.get(),
								expandable_width_sum,
								handler_prefer_widths.get(),
								handlers_width_sum);

			}

		}

	}

	void FrameSplitter::DistributeHorizontallyInProportion(
			std::deque<int>* widget_deque, int widget_width_sum,
			std::deque<int>* prefer_deque, int prefer_width_sum)
	{
		int x = position().x();
		int i = 0;
		std::deque<int>::iterator width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				ResizeSubView(p,
								(size().width() - prefer_width_sum) * (*width_it) / widget_width_sum,
								p->size().height());
				width_it++;

			} else {

				ResizeSubView(p, *handler_width_it,
								p->size().height());
				handler_width_it++;
			}

			MoveSubViewTo(p, x, p->position().y());
			x += p->size().width();

			i++;
		}
	}

	void FrameSplitter::DistributeExpandableFramesHorizontally(
			int unexpandable_width_sum, std::deque<int>* widget_deque,
			int widget_width_sum, std::deque<int>* prefer_deque,
			int prefer_width_sum)
	{
		int x = position().x();
		int i = 0;
		std::deque<int>::iterator exp_width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (p->IsExpandX()) {
					ResizeSubView(p,
									(size().width() - prefer_width_sum
													- unexpandable_width_sum)
													* (*exp_width_it)
																	/ widget_width_sum,
									p->size().height());
					exp_width_it++;
				}

			} else {
				ResizeSubView(p, *handler_width_it, p->size().height());
				handler_width_it++;
			}

			MoveSubViewTo(p, x, p->position().y());
			x += p->size().width();

			i++;
		}
	}

	void FrameSplitter::DistributeUnexpandableFramesHorizontally(
			std::deque<int>* widget_deque, int widget_width_sum,
			std::deque<int>* prefer_deque, int prefer_width_sum)
	{
		int x = position().x();
		int i = 0;
		std::deque<int>::iterator unexp_width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if(!p->IsExpandX()) {

					ResizeSubView(p,
									(size().width() - prefer_width_sum)
													* (*unexp_width_it)
													/ widget_width_sum,
									p->size().height());
					unexp_width_it++;

				}

			} else {
				ResizeSubView(p, *handler_width_it, p->size().height());
				handler_width_it++;
			}

			MoveSubViewTo(p, x, p->position().y());
			x += p->size().width();

			i++;
		}
	}

	void FrameSplitter::DistributeVertically()
	{
		boost::scoped_ptr<std::deque<int> > expandable_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > unexpandable_heights(new std::deque<int>);
		boost::scoped_ptr<std::deque<int> > handler_prefer_heights (new std::deque<int>);

		int expandable_height_sum = 0;	// the width sum of the expandable widgets' size
		int unexpandable_height_sum = 0;	// the width sum of the unexpandable widgets' size
		int handlers_height_sum = 0;

		int prefer_height;
		int i = 0;
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {	// widgets

				if (p->visiable()) {

					if(p->IsExpandY()) {
						expandable_height_sum += p->size().height();
						expandable_heights->push_back(p->size().height());
					} else {
						unexpandable_height_sum += p->size().height();
						unexpandable_heights->push_back(p->size().height());
					}

				}

			} else {	// handlers

				if(p->visiable()) {
					prefer_height = p->GetPreferredSize().height();
					handler_prefer_heights->push_back(prefer_height);
					handlers_height_sum += prefer_height;
				}

			}

			i++;
		}

		if((expandable_heights->size() + unexpandable_heights->size()) == 0) return;	// do nothing if all sub widgets are invisible

		if(expandable_heights->size() == 0) {

			DistributeVerticallyInProportion(unexpandable_heights.get(), unexpandable_height_sum,
							handler_prefer_heights.get(), handlers_height_sum);

		} else if(unexpandable_heights->size() == 0) {

			DistributeVerticallyInProportion(expandable_heights.get(), expandable_height_sum,
							handler_prefer_heights.get(), handlers_height_sum);

		} else {

			int exp_height = size().height() - handlers_height_sum - unexpandable_height_sum;

			if(exp_height <= 0) {

				DistributeUnexpandableFramesVertically(
								unexpandable_heights.get(),
								unexpandable_height_sum,
								handler_prefer_heights.get(),
								handlers_height_sum);

			} else {

				DistributeExpandableFramesVertically(
								unexpandable_height_sum, expandable_heights.get(),
								expandable_height_sum,
								handler_prefer_heights.get(),
								handlers_height_sum);

			}

		}
	}

	void FrameSplitter::DistributeVerticallyInProportion(
			std::deque<int>* widget_deque, int widget_height_sum,
			std::deque<int>* prefer_deque, int prefer_height_sum)
	{
		int y = position().y();
		int i = 0;
		std::deque<int>::iterator height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();

		y = y + size().height();

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				ResizeSubView(p,
								p->size().width(),
								(size().height() - prefer_height_sum)
												* (*height_it) / widget_height_sum);
				height_it++;

			} else {

				ResizeSubView(p, p->size().width(),
								*handler_height_it);
				handler_height_it++;
			}

			y = y - p->size().height();
			MoveSubViewTo(p, p->position().x(), y);

			i++;
		}
	}

	void FrameSplitter::DistributeExpandableFramesVertically(
			int unexpandable_height_sum, std::deque<int>* widget_deque,
			int widget_height_sum, std::deque<int>* prefer_deque,
			int prefer_height_sum)
	{
		int y = position().y();
		int i = 0;
		std::deque<int>::iterator exp_height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();
		y = y + size().height();

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (p->IsExpandY()) {
					ResizeSubView(p,
									p->size().width(),
									(size().height() - prefer_height_sum
													- unexpandable_height_sum)
													* (*exp_height_it)
																	/ widget_height_sum);
					exp_height_it++;
				}

			} else {
				ResizeSubView(p, p->size().width(), *handler_height_it);
				handler_height_it++;
			}

			y -= p->size().height();
			MoveSubViewTo(p, p->position().x(), y);

			i++;
		}
	}

	void FrameSplitter::DistributeUnexpandableFramesVertically(
			std::deque<int>* widget_deque, int widget_height_sum,
			std::deque<int>* prefer_deque, int prefer_height_sum)
	{
		int y = position().y();
		int i = 0;
		std::deque<int>::iterator unexp_height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();
		y = y + size().height();

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (!p->IsExpandY()) {

					ResizeSubView(p, p->size().width(),
									(size().height() - prefer_height_sum)
													* (*unexp_height_it)
													/ widget_height_sum);
					unexp_height_it++;

				}

			} else {
				ResizeSubView(p, p->size().width(), *handler_height_it);
				handler_height_it++;
			}

			y -= p->size().width();
			MoveSubViewTo(p, p->position().x(), y);

			i++;
		}
	}

	void FrameSplitter::AlignHorizontally()
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, p->size().width(), size().height());
			MoveSubViewTo(p, p->position().x(), position().y());
		}
	}

    void FrameSplitter::AlignHorizontallyExt()
    {
        int x = position().x();
        int y = position().y();
        
        for(AbstractView* p = first_subview(); p; p = p->next_view())
        {
            ResizeSubView(p, p->size().width(), size().height());
            MoveSubViewTo(p, x, y);
            x += p->size().width();
        }
    }
    
	void FrameSplitter::AlignVertically()
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, size().width(), p->size().height());
			MoveSubViewTo(p, position().x(), p->position().y());
		}
	}

    void FrameSplitter::AlignVerticallyExt()
    {
        int x = position().x();
        int y = position().y() + size().height();
        
        for(AbstractView* p = first_subview(); p; p = p->next_view())
        {
            ResizeSubView(p, size().width(), p->size().height());
            
            y = y - p->size().height();
            MoveSubViewTo(p, x, y);
        }
    }

    void FrameSplitter::SetFocusedFrame(AbstractFrame* frame, AbstractWindow* context)
    {
    	if(focused_frame_ == frame) return;

    	if(focused_frame_ != nullptr) {
    		delegate_focus_off(focused_frame_, context);
    	}

    	focused_frame_ = frame;
    	if(focused_frame_ != nullptr) {
    		delegate_focus_on(focused_frame_, context);
    	}
    }

	bool FrameSplitter::RemoveSubView (AbstractView* view)
	{
		DBG_PRINT_MSG("%s", "Remove sub frame");

		if(view == focused_frame_) {
			focused_frame_->set_focus(false);
			focused_frame_ = nullptr;
		}

		if(view == hover_frame_) {
			hover_frame_->set_hover(false);
			hover_frame_ = nullptr;
		}

		// TODO: re-layout

		return AbstractFrame::RemoveSubView(view);
	}

	void FrameSplitter::SetHoveredFrame (AbstractWindow* context)
	{
		AbstractFrame* original = hover_frame_;

		if(hover_frame_ != nullptr) {
			if(!hover_frame_->Contain(context->GetCursorPosition())) {

				hover_frame_ = nullptr;
				for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
					if(p->Contain(context->GetCursorPosition())) {
						hover_frame_ = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
				if(p->Contain(context->GetCursorPosition())) {
					hover_frame_ = dynamic_cast<AbstractFrame*>(p);
					break;
				}
			}

		}

		if (original != hover_frame_) {

			if (original != nullptr) {
				delegate_mouse_hover_out_event(original, context);
			}

			if (hover_frame_ != nullptr) {
				delegate_mouse_hover_in_event(hover_frame_, context);
			}

		}
	}

}
