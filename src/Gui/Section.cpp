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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/Section.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/AbstractStackLayout.hpp>

namespace BlendInt {

	using Stock::Shaders;

	AbstractWidget* Section::iterator_ptr = 0;

	Section::Section ()
	: AbstractContainer(),
	  m_focused_widget(0),
	  last_hover_widget_(0),
	  mode_(Normal)
	{
		set_size(800, 600);
	}

	Section::~Section ()
	{
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			p->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);
		}

		// unset hover status
		ClearHoverWidgets();

		// avoid calling deleted Section pointer in Context::MousePressEvent
		if(iterator_ptr == this) {
			iterator_ptr = 0;
		}
	}

	void Section::PushFront(AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {

			events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyedInSection);

		}
	}

	void Section::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {

			events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyedInSection);

		}
	}

	void Section::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);

			if(widget == last_hover_widget_) {

				assert(widget->hover());

				widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				widget->set_hover(false);
				widget = 0;

			}

			if(first() == 0) {

				if(managed() && (reference_count() == 0)) {
					DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
					delete this;
				} else {
					DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
							", it will not be deleted automatically");
				}

			}

		}
	}

	bool Section::Contain (const Point& point) const
	{
		return true;
	}

	Section* Section::GetSection (AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();
		AbstractWidget* section = 0;

		if(container == 0) {
			return dynamic_cast<Section*>(widget);
		} else {

			while(container->container()) {
				section = container;
				container = container->container();
			}

		}

		return dynamic_cast<Section*>(section);
	}

	void Section::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			set_position (*request.position());
		}
	}

	void Section::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size (*request.size());
		}
	}

	void Section::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		// Do nothing
	}

	void Section::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		// Do nothing
	}

	ResponseType Section::Draw (Profile& profile)
	{
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			DispatchDrawEvent(p, profile);
		}

		return Accept;
	}

	ResponseType Section::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Section::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Section::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Section::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_section = this;

		return Ignore;
	}

	ResponseType Section::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_section = this;

		return Ignore;
	}

	ResponseType Section::MousePressEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {

			retval = DispatchMousePressEvent(last_hover_widget_, event);

		}

		if(mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Section::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {

			retval = DispatchMouseReleaseEvent(last_hover_widget_, event);

		}

		if(mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	ResponseType Section::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType retval = Ignore;

		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {
			retval = last_hover_widget_->MouseMoveEvent(event);
		}

		if(mode_ == Modal) {
			retval = Accept;
		}

		return retval;
	}

	bool Section::CheckAndUpdateHoverWidget (const MouseEvent& event)
	{
		if (last_hover_widget_) {

			if(IsHoverThrough(last_hover_widget_->container_, event.position())) {

				if (AbstractStackLayout* parent =
						dynamic_cast<AbstractStackLayout*>(last_hover_widget_->container_)) {
					// StackLayout is a special container which arranges sub widgets vertically (back on top)

					AbstractWidget* orig = last_hover_widget_;

					last_hover_widget_ = 0;
					for(AbstractWidget* p = parent->last_; p; p = p->previous_)
					{
						if(p->visiable() && p->Contain(event.position())) {
							last_hover_widget_ = p;
							break;
						}
					}

					if(last_hover_widget_) {

						if(orig != last_hover_widget_) {
							orig->set_hover(false);
							orig->CursorEnterEvent(false);

							last_hover_widget_->set_hover(true);
							last_hover_widget_->CursorEnterEvent(true);
						}

						UpdateHoverWidgetSubs(event);

						if(orig != last_hover_widget_) {
							orig->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
							events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
						}

					} else {

						orig->destroyed().disconnectOne(this,
						        &Section::OnHoverWidgetDestroyed);
						orig->set_hover(false);
						orig->CursorEnterEvent(false);

						last_hover_widget_ = parent;

						if (last_hover_widget_->Contain(event.position())) {

							//last_hover_widget_->set_hover(true);
							//last_hover_widget_->CursorEnterEvent(true);

							events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);

						} else {

							last_hover_widget_->set_hover(false);
							last_hover_widget_->CursorEnterEvent(false);

							// find which contianer contains cursor position
							while (last_hover_widget_->container()) {

								if (last_hover_widget_->container() == this) {	// FIXME: the widget may be mvoed to another context
									last_hover_widget_ = 0;
									break;
								} else {
									last_hover_widget_ = last_hover_widget_->container();

									if (last_hover_widget_->Contain(event.position())) {
										break;
									}
								}
							}

							if (last_hover_widget_) {
								UpdateHoverWidgetSubs(event);
								events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
							}
						}

					}

				} else {

					if (last_hover_widget_->Contain(event.position())) {

						AbstractWidget* orig = last_hover_widget_;
						UpdateHoverWidgetSubs(event);

						if(orig != last_hover_widget_) {
							orig->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
							events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
						}

					} else {

						last_hover_widget_->destroyed().disconnectOne(this,
						        &Section::OnHoverWidgetDestroyed);
						last_hover_widget_->set_hover(false);
						last_hover_widget_->CursorEnterEvent(false);

						// find which contianer contains cursor position
						while (last_hover_widget_->container()) {

							if (last_hover_widget_->container() == this) {	// FIXME: the widget may be mvoed to another context
								last_hover_widget_ = 0;
								break;
							} else {
								last_hover_widget_ = last_hover_widget_->container();

								if (last_hover_widget_->Contain(event.position())) {
									break;
								}
							}
						}

						if (last_hover_widget_) {
							UpdateHoverWidgetSubs(event);
							events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
						}
					}

				}

			} else {

				last_hover_widget_->destroyed().disconnectOne(this,
				        &Section::OnHoverWidgetDestroyed);
				last_hover_widget_->set_hover(false);
				last_hover_widget_->CursorEnterEvent(false);

				// find which contianer contains cursor position
				while (last_hover_widget_->container()) {

					if (last_hover_widget_->container() == this) {	// FIXME: the widget may be mvoed to another context
						last_hover_widget_ = 0;
						break;
					} else {
						last_hover_widget_ = last_hover_widget_->container();

						if (IsHoverThrough(last_hover_widget_, event.position())) {
							break;
						}
					}
				}

				if (last_hover_widget_) {
					UpdateHoverWidgetSubs(event);
					events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
				}

			}

		} else {

			for(AbstractWidget* p = last(); p; p = p->previous())
			{
				if (p->visiable() && p->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					last_hover_widget_ = p;
					last_hover_widget_->set_hover(true);
					last_hover_widget_->CursorEnterEvent(true);

					break;
				}
			}

			if(last_hover_widget_) {

				UpdateHoverWidgetSubs(event);

				events()->connect(last_hover_widget_->destroyed(), this,
				        &Section::OnHoverWidgetDestroyed);
			}

		}

		return last_hover_widget_ != 0;
	}

	void Section::UpdateHoverWidgetSubs (const MouseEvent& event)
	{
		AbstractContainer* parent = dynamic_cast<AbstractContainer*>(last_hover_widget_);

		if (parent) {

			for(AbstractWidget* p = parent->last(); p; p = p->previous())
			{
				if(p->visiable() && p->Contain(event.position())) {

					last_hover_widget_ = p;
					last_hover_widget_->set_hover(true);
					last_hover_widget_->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void Section::OnSubWidgetDestroyedInSection(AbstractWidget* widget)
	{
		widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);

		if(first() == 0) {

			if(managed() && (reference_count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

			return;
		}

		if((first() == widget) && (last() == widget)) {

			if(managed() && (reference_count() == 0)) {
				DBG_PRINT_MSG("the last widget is removed, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

		}
	}

	ResponseType Section::DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				return widget->MousePressEvent(event);
			}

		}
	}

	ResponseType Section::DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMouseReleaseEvent(widget->container(), event) == Ignore) {
					return widget->MouseReleaseEvent(event);
				} else {
					return Accept;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MouseReleaseEvent(event);
			}

		}
	}

	void Section::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(last_hover_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

		last_hover_widget_ = 0;
	}

	bool Section::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Section::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		return true;
	}

	bool Section::RoundTypeUpdateTest (const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool Section::RoundRadiusUpdateTest (
	        const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void Section::ClearHoverWidgets()
	{
		if(last_hover_widget_) {

			last_hover_widget_->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

			while (last_hover_widget_ && last_hover_widget_ != this) {
				last_hover_widget_->set_hover(false);
				last_hover_widget_ = last_hover_widget_->container();
			}

			if(last_hover_widget_ == this)
				last_hover_widget_ = 0;

		}
	}

}
