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
#include <BlendInt/Gui/Widget.hpp>

#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

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

		glEnableVertexAttribArray(Shaders::instance->location(Stock::FRAME_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::FRAME_INNER_COORD), 3,
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
			preferred_size = first_child()->GetPreferredSize();
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

	bool FrameSplitterHandle::PreDraw(Profile& profile)
	{
		return visiable();
	}

	ResponseType FrameSplitterHandle::Draw(Profile& profile)
	{
		Shaders::instance->frame_inner_program()->use();

		glUniform2f(Shaders::instance->location(Stock::FRAME_INNER_POSITION), position().x(), position().y());
		glUniform4f(Shaders::instance->location(Stock::FRAME_INNER_COLOR), 0.6f, 0.05f, 0.05f, 1.f);

		if(hover()) {
			glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 25);
		} else {
			glUniform1i(Shaders::instance->location(Stock::FRAME_INNER_GAMMA), 0);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		return subs_count() ? Ignore : Accept;
	}

	void FrameSplitterHandle::PostDraw(Profile& profile)
	{
	}

	void FrameSplitterHandle::FocusEvent(bool focus)
	{
	}

	ResponseType FrameSplitterHandle::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitterHandle::MousePressEvent(
			const MouseEvent& event)
	{
		last_ = position();
		cursor_ = event.position();

		if(orientation_ == Horizontal) {
			prev_size_ = previous()->size().height();
			next_size_ = next()->size().height();
			nearby_pos_ = previous()->position().y();
		} else {
			prev_size_ = previous()->size().width();
			next_size_ = next()->size().width();
			nearby_pos_ = next()->position().x();
		}

		set_event_frame(event, this);

		return Accept;
	}

	ResponseType FrameSplitterHandle::MouseReleaseEvent(
			const MouseEvent& event)
	{
		return Accept;
	}

	void FrameSplitterHandle::MouseHoverInEvent(const MouseEvent& event)
	{
		Refresh();
	}

	void FrameSplitterHandle::MouseHoverOutEvent(const MouseEvent& event)
	{
		Refresh();
	}

	ResponseType FrameSplitterHandle::DispatchHoverEvent(const MouseEvent& event)
	{
		if(Contain(event.position())) {
			set_event_frame(event, this);
			return Accept;
		} else {
			set_event_frame(event, 0);
			return Ignore;
		}
	}

	ResponseType FrameSplitterHandle::MouseMoveEvent(const MouseEvent& event)
	{
		if(pressed_ext()) {

			FrameSplitter* splitter = dynamic_cast<FrameSplitter*>(parent());
			assert(splitter);

			if(orientation_ == Horizontal) {

				int offset = event.position().y() - cursor_.y();
				int oy1 = prev_size_ - offset;
				int oy2 = next_size_ + offset;

				if((oy1 <= 0) || (oy2 <= 0)) {
					return Accept;
				}

				splitter->SetSubWidgetPosition(this, last_.x(), last_.y() + offset);

				splitter->ResizeSubWidget(previous(), previous()->size().width(), oy1);
				splitter->SetSubWidgetPosition(previous(), previous()->position().x(), nearby_pos_ + offset);
				splitter->ResizeSubWidget(next(), next()->size().width(), oy2);

			} else {

				int offset = event.position().x() - cursor_.x();
				int oy1 = prev_size_ + offset;
				int oy2 = next_size_ - offset;

				if((oy1 <= 0) || (oy2 <= 0)) {
					return Accept;
				}

				splitter->SetSubWidgetPosition(this, last_.x() + offset, last_.y());

				splitter->ResizeSubWidget(previous(), oy1, previous()->size().height());
				splitter->ResizeSubWidget(next(), oy2, next()->size().height());
				splitter->SetSubWidgetPosition(next(), nearby_pos_ + offset, next()->position().y());

			}

			Refresh();
			return Accept;
		}
		return Accept;
	}

	// --------------------------------

	FrameSplitter::FrameSplitter(Orientation orientation)
	: Frame(),
	  orientation_(orientation),
	  hover_(0)
	{
		set_size(500, 500);
	}

	FrameSplitter::~FrameSplitter()
	{

	}

	void FrameSplitter::AddFrame (Frame* frame, bool append)
	{
		if((frame == 0) || (frame->parent() == this)) return;

		if(subs_count() == 0) {
			PushBackSubWidget(frame);
		} else {
			FrameSplitterHandle* handle = 0;
			if(orientation_ == Horizontal) {
				handle = Manage(new FrameSplitterHandle(Vertical));
			} else {
				handle = Manage(new FrameSplitterHandle(Horizontal));
			}

			if(append) {
				PushBackSubWidget(handle);
				PushBackSubWidget(frame);
			} else {
				PushFrontSubWidget(handle);
				PushFrontSubWidget(frame);
			}
		}

		AlignSubFrames(orientation_, size());

		Refresh();
	}

	void FrameSplitter::Insert(int index, Frame* frame)
	{
	}

	bool FrameSplitter::IsExpandX() const
	{
		bool expand = false;

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
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

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
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

		if(first_child() == 0) {
			preferred_size.set_width(400);
			preferred_size.set_height(400);
		} else {
			Size tmp;

			if (orientation_ == Horizontal) {
				for(AbstractWidget* p = first_child(); p; p = p->next())
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
				for(AbstractWidget* p = first_child(); p; p = p->next())
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

	bool FrameSplitter::PreDraw(Profile& profile)
	{
		return visiable();
	}

	ResponseType FrameSplitter::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
	}

	void FrameSplitter::PostDraw(Profile& profile)
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
			for(AbstractWidget* p = first_child(); p; p = p->next())
			{
				if(i % 2 == 0) {
					ResizeSubWidget(p, room, h);
					SetSubWidgetPosition(p, x, y);
					x = x + room;
				} else {
					handler_width = p->GetPreferredSize().width();
					ResizeSubWidget(p, handler_width, h);
					SetSubWidgetPosition(p, x, y);
					x = x + handler_width;
				}
				i++;
			}

		} else {

			int y = position().y() + size.height();
			int w = size.width();

			int i = 0;
			int handler_height = 0;
			for(AbstractWidget* p = first_child(); p; p = p->next())
			{
				if(i % 2 == 0) {
					y = y - room;
					ResizeSubWidget(p, w, room);
					SetSubWidgetPosition(p, x, y);
				} else {
					handler_height = p->GetPreferredSize().height();
					y = y - handler_height;
					ResizeSubWidget(p, w, handler_height);
					SetSubWidgetPosition(p, x, y);
				}

				i++;
			}

		}

	}

	void FrameSplitter::FocusEvent(bool focus)
	{
	}

	void FrameSplitter::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void FrameSplitter::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(hover_) {
			set_widget_mouse_hover_out_event(hover_, event);
			hover_->destroyed().disconnectOne(this, &FrameSplitter::OnHoverFrameDestroyed);
			hover_ = 0;
		}
	}

	ResponseType FrameSplitter::KeyPressEvent(const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType FrameSplitter::MousePressEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		AbstractWidget* p = 0;
		for(p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = call_mouse_press_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType FrameSplitter::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		/*
		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_release_event(p, event);
				if(response == Accept) break;
			}

		}
		*/

		return response;
	}

	ResponseType FrameSplitter::MouseMoveEvent(const MouseEvent& event)
	{
		return subs_count() ? Ignore : Accept;
	}

	ResponseType FrameSplitter::DispatchHoverEvent(const MouseEvent& event)
	{
		if(Contain(event.position())) {

			AbstractFrame* new_hovered = CheckHoveredFrame(hover_, event);

			if(new_hovered != hover_) {

				if(hover_) {
					set_widget_mouse_hover_out_event(hover_, event);
					hover_->destroyed().disconnectOne(this, &FrameSplitter::OnHoverFrameDestroyed);
				}

				if(new_hovered) {
					set_widget_mouse_hover_in_event(new_hovered, event);
					events()->connect(new_hovered->destroyed(), this, &FrameSplitter::OnHoverFrameDestroyed);
				}

				hover_ = new_hovered;
			}

			if(hover_) {
				delegate_dispatch_hover_event(hover_, event);
			}

			// make sure to set event frame in this function, to tell parent frame or context to set this hover flag
			set_event_frame(event, this);

			return Accept;

		} else {
			set_event_frame(event, 0);
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

		if(first_child() == 0) {
			return retval;
		}

		Size prefer;
		int space = 0;
		int sum = 1;

		// get all the total width/height of splitter handlers
		AbstractWidget* p = first_child()->next();
		while (p) {
			prefer = p->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			p = p->next()->next();
			sum += 2;
		}

		retval = retval - space;
		retval = retval / ((sum + 1) / 2);

		return retval;
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
		for(AbstractWidget* p = first_child(); p; p = p->next())
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
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				ResizeSubWidget(p,
								(size().width() - prefer_width_sum) * (*width_it) / widget_width_sum,
								p->size().height());
				width_it++;

			} else {

				ResizeSubWidget(p, *handler_width_it,
								p->size().height());
				handler_width_it++;
			}

			SetSubWidgetPosition(p, x, p->position().y());
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
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				if (p->IsExpandX()) {
					ResizeSubWidget(p,
									(size().width() - prefer_width_sum
													- unexpandable_width_sum)
													* (*exp_width_it)
																	/ widget_width_sum,
									p->size().height());
					exp_width_it++;
				}

			} else {
				ResizeSubWidget(p, *handler_width_it, p->size().height());
				handler_width_it++;
			}

			SetSubWidgetPosition(p, x, p->position().y());
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
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				if(!p->IsExpandX()) {

					ResizeSubWidget(p,
									(size().width() - prefer_width_sum)
													* (*unexp_width_it)
													/ widget_width_sum,
									p->size().height());
					unexp_width_it++;

				}

			} else {
				ResizeSubWidget(p, *handler_width_it, p->size().height());
				handler_width_it++;
			}

			SetSubWidgetPosition(p, x, p->position().y());
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
		for(AbstractWidget* p = first_child(); p; p = p->next())
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

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				ResizeSubWidget(p,
								p->size().width(),
								(size().height() - prefer_height_sum)
												* (*height_it) / widget_height_sum);
				height_it++;

			} else {

				ResizeSubWidget(p, p->size().width(),
								*handler_height_it);
				handler_height_it++;
			}

			y = y - p->size().height();
			SetSubWidgetPosition(p, p->position().x(), y);

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

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				if (p->IsExpandY()) {
					ResizeSubWidget(p,
									p->size().width(),
									(size().height() - prefer_height_sum
													- unexpandable_height_sum)
													* (*exp_height_it)
																	/ widget_height_sum);
					exp_height_it++;
				}

			} else {
				ResizeSubWidget(p, p->size().width(), *handler_height_it);
				handler_height_it++;
			}

			y -= p->size().height();
			SetSubWidgetPosition(p, p->position().x(), y);

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

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			if(i % 2 == 0) {

				if (!p->IsExpandY()) {

					ResizeSubWidget(p, p->size().width(),
									(size().height() - prefer_height_sum)
													* (*unexp_height_it)
													/ widget_height_sum);
					unexp_height_it++;

				}

			} else {
				ResizeSubWidget(p, p->size().width(), *handler_height_it);
				handler_height_it++;
			}

			y -= p->size().width();
			SetSubWidgetPosition(p, p->position().x(), y);

			i++;
		}
	}

	void FrameSplitter::AlignHorizontally()
	{
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			ResizeSubWidget(p, p->size().width(), size().height());
			SetSubWidgetPosition(p, p->position().x(), position().y());
		}
	}

	void FrameSplitter::AlignVertically()
	{
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			ResizeSubWidget(p, size().width(), p->size().height());
			SetSubWidgetPosition(p, position().x(), p->position().y());
		}
	}

	void FrameSplitter::OnHoverFrameDestroyed(AbstractFrame* frame)
	{
		assert(frame->hover());
		assert(hover_ == frame);

		DBG_PRINT_MSG("unset hover status of widget %s", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &FrameSplitter::OnHoverFrameDestroyed);

		hover_ = 0;
	}

}
