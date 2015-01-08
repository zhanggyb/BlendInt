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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FrameSplitter.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	FrameSplitterHandle::FrameSplitterHandle(Orientation orientation)
	: Frame(),
	  orientation_(orientation),
	  vao_(0),
	  prev_size_(0),
	  next_size_(0),
	  nearby_pos_(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 1);
		} else {
			set_size(1, 200);
		}

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::FRAME_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::FRAME_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	FrameSplitterHandle::~FrameSplitterHandle()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	Size FrameSplitterHandle::GetPreferredSize() const
	{
		Size preferred_size(1, 1);

		if(subs_count()) {
			preferred_size = first_subview()->GetPreferredSize();
		}

		return preferred_size;
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

	void FrameSplitterHandle::PerformPositionUpdate(
			const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			set_position(*request.position());

		}

		if(request.source() == this) {
			ReportPositionUpdate (request);
		}
	}

	void FrameSplitterHandle::PerformSizeUpdate(
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			buffer_.bind();
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.reset();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	bool FrameSplitterHandle::PreDraw(const Context* context)
	{
		return visiable();
	}

	ResponseType FrameSplitterHandle::Draw(const Context* context)
	{
		Context::shaders->frame_inner_program()->use();

		glUniform2f(Context::shaders->location(Shaders::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform4f(Context::shaders->location(Shaders::FRAME_INNER_COLOR), 0.105f, 0.105f, 0.105f, 0.75f);

		/*
		if(hover()) {
			glUniform1i(Context::shaders->location(Shaders::FRAME_INNER_GAMMA), 25);
		} else {
			glUniform1i(Context::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);
		}
		*/

		glUniform1i(Context::shaders->location(Shaders::FRAME_INNER_GAMMA), 0);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		return subs_count() ? Ignore : Finish;
	}

	void FrameSplitterHandle::PostDraw(const Context* context)
	{
	}

	void FrameSplitterHandle::PerformFocusOn(const Context* context)
	{

	}

	void FrameSplitterHandle::PerformFocusOff (const Context* context)
	{

	}

	ResponseType FrameSplitterHandle::PerformKeyPress(const Context* context)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::PerformContextMenuPress(
			const Context* context)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::PerformContextMenuRelease(
			const Context* context)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::PerformMousePress(
			const Context* context)
	{
		last_ = position();
		cursor_ = context->cursor_position();

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

	ResponseType FrameSplitterHandle::PerformMouseRelease(
			const Context* context)
	{
		if(!hover()) {
			Context::cursor->PopCursor();
		}

		set_pressed(false);
		return Finish;
	}

	void FrameSplitterHandle::PerformHoverIn(const Context* context)
	{
		Context::cursor->PushCursor();
		if(orientation_ == Horizontal) {
			Context::cursor->SetCursor(SplitVCursor);
		} else {
			Context::cursor->SetCursor(SplitHCursor);
		}

		//RequestRedraw();
	}

	void FrameSplitterHandle::PerformHoverOut(const Context* context)
	{
		if(!pressed_ext())
			Context::cursor->PopCursor();

		//RequestRedraw();
	}

	ResponseType FrameSplitterHandle::DispatchHoverEvent(const Context* context)
	{
		if(Contain(context->cursor_position())) {
			return Finish;
		} else {
			return Ignore;
		}
	}

	ResponseType FrameSplitterHandle::PerformMouseMove(const Context* context)
	{
		if(pressed_ext()) {

			FrameSplitter* splitter = dynamic_cast<FrameSplitter*>(superview());
			assert(splitter);

			if(orientation_ == Horizontal) {

				int offset = context->cursor_position().y() - cursor_.y();
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

				int offset = context->cursor_position().x() - cursor_.x();
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
	: Frame(),
	  orientation_(orientation),
	  hover_frame_(0),
	  focused_frame_(0)
	{
		set_size(500, 500);
	}

	FrameSplitter::~FrameSplitter()
	{

	}

	void FrameSplitter::AddFrame (Frame* frame, SizePolicy policy)
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

	void FrameSplitter::InsertFrame(int index, Frame* frame, SizePolicy policy)
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

	int FrameSplitter::GetFrameIndex (Frame* frame) const
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

	Frame* FrameSplitter::GetFrame (int index) const
	{
		if(subs_count() == 0) return 0;

		int max = (subs_count() + 1) / 2;
		if(index > max) return 0;

		index = index * 2;

		return dynamic_cast<Frame*>(GetWidgetAt(index));
	}

	FrameSplitterHandle* FrameSplitter::GetHandle (int index) const
	{
		if(subs_count() <= 1) return 0;

		int max = (subs_count() - 1) / 2;
		if(index > max) return 0;

		index = index * 2 + 1;

		return dynamic_cast<FrameSplitterHandle*>(GetWidgetAt(index));
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
			MoveSubWidgets(ox, oy);
			set_position(*request.position());
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

	bool FrameSplitter::PreDraw(const Context* context)
	{
		return visiable();
	}

	ResponseType FrameSplitter::Draw(const Context* context)
	{
		DrawSubViewsOnce(context);

		return subs_count() ? Ignore : Finish;
	}

	void FrameSplitter::PostDraw(const Context* context)
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

	void FrameSplitter::PerformFocusOn (const Context* context)
	{
	}

	void FrameSplitter::PerformFocusOff (const Context* context)
	{
	}

	void FrameSplitter::PerformHoverIn(const Context* context)
	{
	}

	void FrameSplitter::PerformHoverOut(const Context* context)
	{
		if(hover_frame_) {
			delegate_mouse_hover_out_event(hover_frame_, context);
			hover_frame_ = nullptr;
		}
	}

	ResponseType FrameSplitter::PerformKeyPress(const Context* context)
	{
		if(focused_frame_) {
			return delegate_key_press_event(focused_frame_, context);
		}

		return Ignore;
	}

	ResponseType FrameSplitter::PerformMousePress(const Context* context)
	{
		ResponseType response = Ignore;
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

	ResponseType FrameSplitter::PerformMouseRelease(const Context* context)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		if(focused_frame_ != nullptr) {
			response = delegate_mouse_release_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType FrameSplitter::PerformMouseMove(const Context* context)
	{
		ResponseType response = Ignore;

		if(pressed_ext() && focused_frame_) {
			response = delegate_mouse_move_event(focused_frame_, context);
		}

		return response;
	}

	ResponseType FrameSplitter::DispatchHoverEvent(const Context* context)
	{
		if(Contain(context->cursor_position())) {

			ResponseType response = Finish;
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

	void FrameSplitter::AddColumn(Frame* frame, SizePolicy policy)
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

	void FrameSplitter::AddRow(Frame* frame, SizePolicy policy)
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

	void FrameSplitter::InsertColumn(int index, Frame* frame, SizePolicy policy)
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

	void FrameSplitter::InsertRow(int index, Frame* frame, SizePolicy policy)
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

    void FrameSplitter::SetFocusedFrame(AbstractFrame* frame, const Context* context)
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

	void FrameSplitter::SetHoveredFrame (const Context* context)
	{
		AbstractFrame* original = hover_frame_;

		if(hover_frame_ != nullptr) {
			if(!hover_frame_->Contain(context->cursor_position())) {

				hover_frame_ = nullptr;
				for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
					if(p->Contain(context->cursor_position())) {
						hover_frame_ = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
				if(p->Contain(context->cursor_position())) {
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
