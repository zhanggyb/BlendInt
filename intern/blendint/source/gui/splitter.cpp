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

#include <boost/smart_ptr.hpp>

#include <opengl/glarraybuffer.hpp>

#include <gui/splitter.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	SplitterHandle::SplitterHandle (Orientation orientation)
	: AbstractRoundWidget(),
	  orientation_(orientation),
	  vao_(0),
	  highlight_(false),
	  pressed_(false),
	  prev_size_(0),
	  next_size_(0),
	  nearby_pos_(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 1);
		} else {
			set_size(1, 200);
		}

		glGenVertexArrays(1, &vao_);
		buffer_.reset(new GLArrayBuffer);

		glBindVertexArray(vao_);

		buffer_->generate();
		buffer_->bind();

		std::vector<GLfloat> vertices(8, 0.f);

		if(orientation == Horizontal) {
			vertices[2] = 200.f;
			//vertices[3] = 0.f;

			//vertices[4] = 0.f;
			vertices[5] = 1.f;

			vertices[6] = 200.f;
			vertices[7] = 1.f;
		} else {
			vertices[2] = 1.f;
			//vertices[3] = 0.f;

			//vertices[4] = 0.f;
			vertices[5] = 200.f;

			vertices[6] = 1.f;
			vertices[7] = 200.f;
		}

		buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 2,
		        GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		buffer_->reset();
	}

	SplitterHandle::~SplitterHandle ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	Size SplitterHandle::GetPreferredSize () const
	{
		Size preferred_size(1, 1);

		if(orientation_ == Horizontal) {
			preferred_size.set_width(200);
		} else {
			preferred_size.set_height(200);
		}

		return preferred_size;
	}

	bool SplitterHandle::IsExpandX () const
	{
		if(orientation_ == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool SplitterHandle::IsExpandY () const
	{
		if(orientation_ == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	bool SplitterHandle::Contain (const Point& point) const
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

	void SplitterHandle::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			std::vector<GLfloat> vertices(8, 0.f);

			vertices[2] = (GLfloat)request.size()->width();
			//vertices[3] = 0.f;

			//vertices[4] = 0.f;
			vertices[5] = (GLfloat)request.size()->height();

			vertices[6] = (GLfloat)request.size()->width();
			vertices[7] = (GLfloat)request.size()->height();

			buffer_->bind();
			buffer_->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
			buffer_->reset();

			set_size(*request.size());
			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	Response SplitterHandle::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_triangle_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION), 0.f, 0.f);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 1);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);

		if(highlight_) {
			//glUniform1i(AbstractWindow::shaders->location(Shaders::TRIANGLE_GAMMA), 50);
			glVertexAttrib4f(AttributeColor, 0.85f, 0.15f, 0.15f, 0.6f);
		} else {
			//glUniform1i(AbstractWindow::shaders->location(Shaders::TRIANGLE_GAMMA), 0);
			glVertexAttrib4f(AttributeColor, 0.15f, 0.15f, 0.15f, 0.6f);
		}
		//glVertexAttrib4f(AbstractWindow::shaders->location(Shaders::TRIANGLE_COLOR), 0.15f, 0.15f, 0.15f, 0.6f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		GLSLProgram::reset();

		return Finish;
	}

	void SplitterHandle::PerformHoverIn(AbstractWindow* context)
	{
		highlight_ = true;
		context->PushCursor();
		context->SetCursor(orientation_ == Horizontal ? SplitVCursor : SplitHCursor);
		RequestRedraw();
	}

	void SplitterHandle::PerformHoverOut(AbstractWindow* context)
	{
		highlight_ = false;
		context->PopCursor();
		RequestRedraw();
	}

	Response SplitterHandle::PerformMousePress (AbstractWindow* context)
	{
		last_ = position();
		cursor_ = context->GetGlobalCursorPosition();
		pressed_ = true;

		if(orientation_ == Horizontal) {
			prev_size_ = previous_view()->size().height();
			next_size_ = next_view()->size().height();
			nearby_pos_ = previous_view()->position().y();
		} else {
			prev_size_ = previous_view()->size().width();
			next_size_ = next_view()->size().width();
			nearby_pos_ = next_view()->position().x();
		}

		return Finish;
	}

	Response SplitterHandle::PerformMouseRelease (AbstractWindow* context)
	{
		if (pressed_) {
			pressed_ = false;
		}

		return Finish;
	}

	Response SplitterHandle::PerformMouseMove (AbstractWindow* context)
	{
		if(pressed_) {

			Splitter* splitter = dynamic_cast<Splitter*>(superview());

			if(orientation_ == Horizontal) {

				int offset = context->GetGlobalCursorPosition().y() - cursor_.y();
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

				int offset = context->GetGlobalCursorPosition().x() - cursor_.x();
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

	Splitter::Splitter(Orientation orientation)
	: AbstractRoundWidget(),
	  orientation_(orientation)
	{
		set_size(400, 400);
	}

	Splitter::~Splitter ()
	{

	}
	
	void Splitter::Prepend (AbstractWidget* widget)
	{
		if(widget && widget->superview() != this) {

			if(first_subview() == 0) {
				PushFrontSubView(widget);
			} else {
				SplitterHandle* handle = 0;
				if(orientation_ == Horizontal) {
					handle = Manage(new SplitterHandle(Vertical));
				} else {
					handle = Manage(new SplitterHandle(Horizontal));
				}

				PushFrontSubView(handle);
				PushFrontSubView(widget);
			}

			AlignSubWidgets(orientation_, size());

			// TODO: connect widget's destroyed event
		}
	}

	void Splitter::Append (AbstractWidget* widget)
	{
		if(widget && widget->superview() != this) {

			if(first_subview() == 0) {
				PushBackSubView(widget);
			} else {
				SplitterHandle* handle = 0;
				if(orientation_ == Horizontal) {
					handle = Manage(new SplitterHandle(Vertical));
				} else {
					handle = Manage(new SplitterHandle(Horizontal));
				}

				PushBackSubView(handle);
				PushBackSubView(widget);
			}

			AlignSubWidgets(orientation_, size());

			// TODO: connect widget's destroyed event
		}
	}

	void Splitter::Insert(int index, AbstractWidget* widget)
	{

	}

	void Splitter::Remove (AbstractWidget* widget)
	{
		if(RemoveSubView(widget)) {
			if(orientation_ == Horizontal) {
				FillSubWidgetsInSplitter(size(), orientation_);
			} else {
				FillSubWidgetsInSplitter(size(), orientation_);
			}
		}
	}

	Size Splitter::GetPreferredSize() const
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

	bool Splitter::IsExpandX() const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandX()) {
				expand = true;
				break;
			}
		}

		return expand;

	}

	bool Splitter::IsExpandY() const
	{
		bool expand = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p->IsExpandY()) {
				expand = true;
				break;
			}
		}

		return expand;
	}


	int Splitter::GetWidgetIndex (AbstractWidget* widget) const
	{
		int index = 0;
		if(widget->superview() != this) return -1;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(p == widget) break;

			index++;
			p = p->next_view()->next_view();
		}

		return index;
	}

	int Splitter::GetHandleIndex (SplitterHandle* handle) const
	{
		int index = 0;
		if(handle->superview() != this) return -1;

		for(AbstractView* p = first_subview()->next_view(); p; p = p->next_view())
		{
			if(p == handle) break;

			index++;
			p = p->next_view()->next_view();
		}

		return index;
	}

	AbstractWidget* Splitter::GetWidget (int index) const
	{
		if(first_subview() == 0) return 0;

		int sum = subs_count();

		int max = (sum + 1) / 2;
		if(index > max) return 0;

		index = index * 2;

		return dynamic_cast<AbstractWidget*>(GetSubViewAt(index));
	}

	SplitterHandle* Splitter::GetHandle (int index) const
	{
		int sum = subs_count();

		if(sum <= 1) return 0;

		int max = (sum - 1) / 2;
		if(index > max) return 0;

		index = index * 2 + 1;

		return dynamic_cast<SplitterHandle*>(GetSubViewAt(index));
	}

	int Splitter::GetWidgetCount () const
	{
		int sum = subs_count();
		return (sum / 2 + 1);
	}

	void Splitter::MoveHandle (int index, int x, int y)
	{
	}

	void Splitter::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			FillSubWidgetsInSplitter(*request.size(), orientation_);

			set_size(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Splitter::AlignSubWidgets (Orientation orientation, const Size& out_size)
	{
		int room = GetAverageRoom(orientation, out_size);
		int x = position().x();

		if(orientation == Horizontal) {

			int y = position().y();
			int h = out_size.height();

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

			int y = position().y() + out_size.height();
			int w = out_size.width();

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

	void Splitter::FillSubWidgetsInSplitter (const Size& out_size, Orientation orientation)
	{
		int x = 0;
		int y = 0;
		int width = out_size.width();
		int height = out_size.height();

		FillSubWidgetsInSplitter(x, y, width, height, orientation);
	}

	void Splitter::FillSubWidgetsInSplitter (int x, int y, int width,
	        int height, Orientation orientation)
	{
		if(orientation == Horizontal) {

			DistributeHorizontally(x, width);
			AlignHorizontally(y, height);

		} else {

			DistributeVertically(y, height);
			AlignVertically(x, width);

		}
	}

	void Splitter::DistributeHorizontally (int x, int width)
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

			DistributeHorizontallyInProportion(x, width, unexpandable_widths.get(), unexpandable_width_sum,
							handler_prefer_widths.get(), handlers_width_sum);

		} else if(unexpandable_widths->size() == 0) {

			DistributeHorizontallyInProportion(x, width, expandable_widths.get(), expandable_width_sum,
							handler_prefer_widths.get(), handlers_width_sum);

		} else {

			int exp_width = width - handlers_width_sum - unexpandable_width_sum;

			if(exp_width <= 0) {

				DistributeUnexpandableWidgetsHorizontally(x, width,
								unexpandable_widths.get(),
								unexpandable_width_sum,
								handler_prefer_widths.get(),
								handlers_width_sum);

			} else {

				DistributeExpandableWidgetsHorizontally(x, width,
								unexpandable_width_sum, expandable_widths.get(),
								expandable_width_sum,
								handler_prefer_widths.get(),
								handlers_width_sum);

			}

		}

	}

	void Splitter::DistributeVertically (int y, int height)
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

			DistributeVerticallyInProportion(y, height, unexpandable_heights.get(), unexpandable_height_sum,
							handler_prefer_heights.get(), handlers_height_sum);

		} else if(unexpandable_heights->size() == 0) {

			DistributeVerticallyInProportion(y, height, expandable_heights.get(), expandable_height_sum,
							handler_prefer_heights.get(), handlers_height_sum);

		} else {

			int exp_height = height - handlers_height_sum - unexpandable_height_sum;

			if(exp_height <= 0) {

				DistributeUnexpandableWidgetsVertically(y, height,
								unexpandable_heights.get(),
								unexpandable_height_sum,
								handler_prefer_heights.get(),
								handlers_height_sum);

			} else {

				DistributeExpandableWidgetsVertically(y, height,
								unexpandable_height_sum, expandable_heights.get(),
								expandable_height_sum,
								handler_prefer_heights.get(),
								handlers_height_sum);

			}

		}

	}

	void Splitter::AlignHorizontally (int y, int height)
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, p->size().width(), height);
			MoveSubViewTo(p, p->position().x(), y);
		}
	}

	void Splitter::AlignVertically (int x, int width)
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			ResizeSubView(p, width, p->size().height());
			MoveSubViewTo(p, x, p->position().y());
		}
	}
	
	void Splitter::DistributeHorizontallyInProportion (int x, int width,
					std::deque<int>* widget_deque, int widget_width_sum,
					std::deque<int>* prefer_deque, int prefer_width_sum)
	{
		int i = 0;
		std::deque<int>::iterator width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				ResizeSubView(p,
								(width - prefer_width_sum) * (*width_it) / widget_width_sum,
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
	
	void Splitter::DistributeExpandableWidgetsHorizontally (int x, int width,
					int unexpandable_width_sum,
					std::deque<int>* widget_deque, int widget_width_sum,
					std::deque<int>* prefer_deque, int prefer_width_sum)
	{
		int i = 0;
		std::deque<int>::iterator exp_width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (p->IsExpandX()) {
					ResizeSubView(p,
									(width - prefer_width_sum
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
	
	void Splitter::DistributeUnexpandableWidgetsHorizontally (int x, int width,
					std::deque<int>* widget_deque, int widget_width_sum,
					std::deque<int>* prefer_deque, int prefer_width_sum)
	{
		int i = 0;
		std::deque<int>::iterator unexp_width_it = widget_deque->begin();
		std::deque<int>::iterator handler_width_it = prefer_deque->begin();
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if(!p->IsExpandX()) {

					ResizeSubView(p,
									(width - prefer_width_sum)
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

	int Splitter::GetAverageRoom (Orientation orientation, const Size& out_size)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = out_size.width();
		} else {
			room = out_size.height();
		}

		if(first_subview() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		int sum = 0;

		AbstractView* p = first_subview()->next_view();
		sum += 1;

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

		room = room - space;
		room = room / ((sum + 1) / 2);

		return room;
	}
	
	void Splitter::DistributeVerticallyInProportion (int y, int height,
					std::deque<int>* widget_deque, int widget_height_sum,
					std::deque<int>* prefer_deque, int prefer_height_sum)
	{
		int i = 0;
		std::deque<int>::iterator height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();

		y = y + height;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				ResizeSubView(p,
								p->size().width(),
								(height - prefer_height_sum)
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
	
	void Splitter::DistributeExpandableWidgetsVertically (int y, int height,
					int unexpandable_height_sum, std::deque<int>* widget_deque,
					int widget_height_sum, std::deque<int>* prefer_deque,
					int prefer_height_sum)
	{
		int i = 0;
		std::deque<int>::iterator exp_height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();
		y = y + height;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (p->IsExpandY()) {
					ResizeSubView(p,
									p->size().width(),
									(height - prefer_height_sum
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
	
	void Splitter::DistributeUnexpandableWidgetsVertically (int y, int height,
					std::deque<int>* widget_deque, int widget_height_sum,
					std::deque<int>* prefer_deque, int prefer_height_sum)
	{
		int i = 0;
		std::deque<int>::iterator unexp_height_it = widget_deque->begin();
		std::deque<int>::iterator handler_height_it = prefer_deque->begin();
		y = y + height;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			if(i % 2 == 0) {

				if (!p->IsExpandY()) {

					ResizeSubView(p, p->size().width(),
									(height - prefer_height_sum)
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

	int Splitter::GetWidgetsRoom (Orientation orientation, const Size& out_size)
	{
		int room = 0;

		if(orientation == Horizontal) {
			room = out_size.width();
		} else {
			room = out_size.height();
		}

		if(first_subview() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;

		AbstractView* p = first_subview()->next_view();
		while (p) {
			prefer = p->GetPreferredSize();
			if(orientation == Horizontal) {
				space = prefer.width();
			} else {
				space = prefer.height();
			}

			p = p->next_view()->next_view();
		}

		room = room - space;

		return room;
	}

}
