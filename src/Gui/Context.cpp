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

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt
{
	using Stock::Shaders;

	glm::mat4 Context::default_view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f));

	std::set<Context*> Context::context_set;

	Context::Context ()
	: AbstractContainer(),
	  focused_widget_(0),
	  custom_focus_widget_(false)
	{
		set_size(640, 480);

		InitializeContext();

		context_set.insert(this);
	}

	Context::~Context ()
	{
		if(container() != 0) {
			DBG_PRINT_MSG("Error: %s", "Context MUST NOT be in any other container");
		}

		if (focused_widget_) {
			focused_widget_->set_focus(false);
			focused_widget_->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;
		}

		context_set.erase(this);
	}

	void Context::AddViewport (Viewport* vp)
	{
		if(PushBackSubWidget(vp)) {
			// TODO: 			
		}
	}

	Section* Context::Append (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		Section* section = Section::GetSection(widget);

		if(section) {

			if(section->container_ == this) {
				DBG_PRINT_MSG("Widget %s is already in context %s",
									widget->name().c_str(),
									name().c_str());
				return section;
			} else {
				if(section->container_) {
					section->container_->RemoveSubWidget(section);
				}
			}

		} else {
			section = Manage(new Section);
			section->Append(widget);
		}

#ifdef DEBUG
		if(!section->managed()) {
			DBG_PRINT_MSG("Warning: the section %s is not set managed", section->name().c_str());
		}

		if(section->first_ == 0) {
			DBG_PRINT_MSG("Warning: trying to add an emptry section %s in a context, it will not be delete automatically", section->name().c_str());
		}

		int count = widget_count();
		char buf[32];
		sprintf(buf, "Section %d", count);
		DBG_SET_NAME(section, buf);
#endif

		PushBackSubWidget(section);

		ResizeSubWidget(section, size());

		return section;
	}

	Section* Context::Remove (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		// if the container is a section, the section will destroy itself if it's empty
		Section* section = Section::GetSection(widget);
		assert(section->container() == this);

		widget->container_->RemoveSubWidget(widget);

		if(widget->focused()) {

			assert(focused_widget_ == widget);

			widget->set_focus(false);
			widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			focused_widget_ = 0;

		}

		if(section->first_ == 0) {
			DBG_PRINT_MSG("no sub widgets, delete this section: %s", section->name().c_str());
			if(section->managed() && (section->reference_count() == 0)) {
				delete section;
				section = 0;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}
		}

		return section;
	}

	bool Context::Contain (const Point& point) const
	{
		return true;
	}

	void Context::SetFocusedWidget (AbstractWidget* widget)
	{
		custom_focus_widget_ = true;

		if(focused_widget_ == widget) {
			return;
		}

		if (focused_widget_) {
			focused_widget_->set_focus(false);
			focused_widget_->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			focused_widget_->FocusEvent(false);
		}

		focused_widget_ = widget;
		if (focused_widget_) {
			focused_widget_->set_focus(true);
			events()->connect(focused_widget_->destroyed(), this, &Context::OnFocusedWidgetDestroyed);
			focused_widget_->FocusEvent(true);
		}
	}

	void Context::SetCursor (int cursor_type)
	{
		// TODO: overwrite this
	}

	void Context::PushCursor (int cursor_type)
	{
		cursor_stack_.push(cursor_type);
	}

	int Context::PopCursor ()
	{
		int cursor = ArrowCursor;

		if(!cursor_stack_.empty()) {
			cursor = cursor_stack_.top();
			cursor_stack_.pop();
		}

		return cursor;
	}

	Context* Context::GetContext (AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();

		if(container == 0) {
			return dynamic_cast<Context*>(widget);
		} else {

			while(container->container()) {
				container = container->container();
			}

		}

		return dynamic_cast<Context*>(container);
	}

	bool Context::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Context::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		return true;
	}

	bool Context::RoundTypeUpdateTest (const RoundTypeUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	bool Context::RoundRadiusUpdateTest (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	void Context::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		// nothing need to do.
	}

	void Context::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.source() == this) {

			glm::mat4 projection = glm::ortho(0.f, (GLfloat) request.size()->width(),
			        0.f, (GLfloat) request.size()->height(), 100.f, -100.f);

			Shaders::instance->SetUIProjectionMatrix(projection);

			/*
			for(AbstractWidget* p = first(); p; p = p->next())
			{
				ResizeSubWidget(p, *request.size());
			}
			*/

			set_size(*request.size());

			resized_.fire(size());

		} else if (request.source()->container() == this) {

		} else {

			// if(request.target()->container()->hover()) {
			//	m_layers[request.source()->z()].m_hover_list_valid = false;
			// }

			//DBG_PRINT_MSG("%s", "get widget geometry change update");

			//DBG_PRINT_MSG("source widget: %s, target widget: %s", request.source()->name().c_str(), request.target()->name().c_str());

		}

	}

	void Context::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
	}

	void Context::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
	}

	ResponseType Context::Draw (Profile& profile)
	{
		//glm::vec3 pos(position().x(), position().y(), z());
		//glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		glClearStencil(0);
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT |
				GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glm::mat4 projection_matrix  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);

		Shaders::instance->SetUIProjectionMatrix(projection_matrix);
		Shaders::instance->SetUIViewMatrix(default_view_matrix);
		Shaders::instance->SetUIModelMatrix(glm::mat4(1.f));
		glViewport(0, 0, size().width(), size().height());

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			p->Draw(profile);

			Shaders::instance->SetUIProjectionMatrix(projection_matrix);
			Shaders::instance->SetUIViewMatrix(default_view_matrix);
			Shaders::instance->SetUIModelMatrix(glm::mat4(1.f));
			glViewport(0, 0, size().width(), size().height());
		}

		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).m_context = this;

		ResponseType response;

		if(focused_widget_) {
			response = focused_widget_->KeyPressEvent(event);
		}

		return response;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_context = this;

		return Ignore;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_context = this;

		return Ignore;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		AbstractWidget* widget = 0;	// widget may be focused

		const_cast<MouseEvent&>(event).m_context = this;

		custom_focus_widget_ = false;
		for (Section::iterator_ptr = last(); Section::iterator_ptr;
		        Section::iterator_ptr = Section::iterator_ptr->previous()) {
			response = Section::iterator_ptr->MousePressEvent(event);

			if (response == Accept)	break;
		}

		if(response == Accept && Section::iterator_ptr) {
			widget = dynamic_cast<Section*>(Section::iterator_ptr)->last_hover_widget_;
		}

		Section::iterator_ptr = 0;

		if(!custom_focus_widget_) {
			SetFocusedWidget(widget);
		}
		custom_focus_widget_ = false;

		/*
		if(focused_widget_) {
			DBG_PRINT_MSG("focus widget: %s", focused_widget_->name().c_str());
		} else {
			DBG_PRINT_MSG("%s", "focus widget unset");
		}
		*/

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		// tell the focused widget first
		if(focused_widget_) {
			response = focused_widget_->MouseReleaseEvent(event);
		}

		if(response == Accept) {
			return response;
		}

		for(AbstractWidget* p = last(); p; p = p->previous())
		{
			response = p->MouseReleaseEvent(event);
			if (response == Accept) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		// tell the focused widget first
		if (focused_widget_) {
			response = focused_widget_->MouseMoveEvent(event);
		}

		if(response == Accept) {

			// still set cursor hover
			for(AbstractWidget* p = last(); p; p = p->previous())
			{
				if(dynamic_cast<Section*>(p)->CheckAndUpdateHoverWidget(event)) break;
			}

			return response;	// return Accept
		}

		for(AbstractWidget* p = last(); p; p = p->previous())
		{
			response = p->MouseMoveEvent(event);

			if (response == Accept) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::FocusEvent (bool focus)
	{
		return Ignore;
	}

	void Context::InitializeContext ()
	{
		glm::mat4 projection = glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f);

		Shaders::instance->SetUIProjectionMatrix(projection);
		Shaders::instance->SetUIViewMatrix(default_view_matrix);
	}

	AbstractWidget* Context::GetWidgetUnderCursor(const MouseEvent& event, AbstractWidget* parent)
	{
		return 0;
	}

	void Context::OnFocusedWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());

		assert(focused_widget_ == widget);

		if(widget->focused()) {
			widget->set_focus(false);
			widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
		}

		focused_widget_ = 0;
	}

}
