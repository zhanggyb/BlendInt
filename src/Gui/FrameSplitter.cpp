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

#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FrameSplitterHandle::FrameSplitterHandle(Orientation orientation)
	: AbstractFrame(),
	  orientation_(orientation),
	  vao_(0),
	  previous_frame_(0),
	  next_frame_(0)
	{
		if(orientation == Horizontal) {
			set_size(200, 1);
		} else {
			set_size(1, 200);
		}

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat4(1.f);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	FrameSplitterHandle::~FrameSplitterHandle()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void FrameSplitterHandle::SetHandleWidget(Widget* widget)
	{
		if(subs_count()) {
			ClearSubWidgets();
		}

		if(PushBackSubWidget(widget)) {
			widget->SetPosition(0, 0);
			widget->Resize(size().width(),
					size().height());
		}
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

			float x = static_cast<float>(position().x()  + offset().x());
			float y = static_cast<float>(position().y()  + offset().y());

			projection_matrix_ = glm::ortho(
				x,
				x + size().width(),
				y,
				y + size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f));

		}

		if(request.source() == this) {
			ReportPositionUpdate (request);
		}
	}

	void FrameSplitterHandle::PerformSizeUpdate(
			const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			float x = static_cast<float>(position().x() + offset().x());
			float y = static_cast<float>(position().y() + offset().y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			buffer_.bind();
			buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.reset();

			if(subs_count()) {
				first_child()->Resize(*request.size());
			}
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void FrameSplitterHandle::PreDraw(Profile& profile)
	{
		glViewport(position().x(), position().y(), size().width(), size().height());
		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		Shaders::instance->SetUIProjectionMatrix(projection_matrix_);
		Shaders::instance->SetUIModelMatrix(model_matrix_);
	}

	ResponseType FrameSplitterHandle::Draw(Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		if(hover()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.9f, 0.05f, 0.05f, 1.f);
		} else {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.6f, 0.05f, 0.05f, 1.f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return Ignore;
	}

	void FrameSplitterHandle::PostDraw(Profile& profile)
	{
		glDisable(GL_SCISSOR_TEST);
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
		return Ignore;
	}

	ResponseType FrameSplitterHandle::MouseReleaseEvent(
			const MouseEvent& event)
	{
		return Ignore;
	}

	void FrameSplitterHandle::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void FrameSplitterHandle::MouseHoverOutEvent(const MouseEvent& event)
	{
	}

	ResponseType FrameSplitterHandle::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

	// --------------------------------

	FrameSplitter::FrameSplitter(Orientation orientation)
	: AbstractFrame(),
	  orientation_(orientation),
	  hover_(0)
	{
		set_size(500, 500);
	}

	FrameSplitter::~FrameSplitter()
	{

	}

	void FrameSplitter::AddFrame (AbstractFrame* frame, bool append)
	{
		if((frame == 0) || (frame->parent() == this)) return;

		if(append) {

			if(first_child() == 0) {
				PushBackSubWidget(frame);
			} else {
				FrameSplitterHandle* handle = 0;

				if(orientation_ == Horizontal) {
					handle = Manage(new FrameSplitterHandle(Vertical));
				} else {
					handle = Manage(new FrameSplitterHandle(Horizontal));
				}

				AbstractWidget* p = last_child();
				PushBackSubWidget(handle);
				handle->previous_frame_ = dynamic_cast<AbstractFrame*>(p);
				handle->next_frame_ = frame;
				PushBackSubWidget(frame);
			}

			AlignSubFrames(orientation_, size());

		} else {

		}
	}

	void FrameSplitter::Insert(int index, AbstractFrame* viewport)
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

	ResponseType FrameSplitter::Draw(Profile& profile)
	{
		for(AbstractWidget* p = first_child(); p; p = p->next()) {
			DispatchDrawEvent (p, profile);
		}

		return subs_count() ? Ignore : Accept;
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

		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_press_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType FrameSplitter::MouseReleaseEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {

				response = assign_mouse_release_event(p, event);
				if(response == Accept) break;
			}

		}

		return response;
	}

	ResponseType FrameSplitter::MouseMoveEvent(const MouseEvent& event)
	{
		ResponseType response = Ignore;

		AbstractFrame* original_hover = hover_;

		if(hover_) {
			if(!hover_->Contain(event.position())) {

				hover_ = 0;
				for(AbstractWidget* p = last_child(); p; p = p->previous()) {
					if(p->Contain(event.position())) {
						hover_ = dynamic_cast<AbstractFrame*>(p);
						break;
					}
				}

			}
		} else {

			for(AbstractWidget* p = last_child(); p; p = p->previous()) {
				if(p->Contain(event.position())) {
					hover_ = dynamic_cast<AbstractFrame*>(p);
					break;
				}
			}

		}

		if(original_hover != hover_) {

			if(original_hover) {
				set_widget_mouse_hover_out_event(original_hover, event);
				original_hover->destroyed().disconnectOne(this, &FrameSplitter::OnHoverFrameDestroyed);
			}

			if(hover_) {
				set_widget_mouse_hover_in_event(hover_, event);
				events()->connect(hover_->destroyed(), this, &FrameSplitter::OnHoverFrameDestroyed);
			}

		}

		if(hover_) {
			response = assign_mouse_move_event(hover_, event);
		}

		return response;
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
		int room = 0;

		if(orientation == Horizontal) {
			room = size.width();
		} else {
			room = size.height();
		}

		if(first_child() == 0) {
			return room;
		}

		Size prefer;
		int space = 0;
		int sum = 0;

		AbstractWidget* p = first_child()->next();
		sum += 1;

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

		room = room - space;
		room = room / ((sum + 1) / 2);

		return room;
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
