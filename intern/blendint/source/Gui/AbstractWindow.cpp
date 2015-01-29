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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Time.hpp>

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Font/FcPattern.hpp>
#include <BlendInt/Font/FcConfig.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	Theme* AbstractWindow::theme = 0;
	Icons* AbstractWindow::icons = 0;
	Shaders* AbstractWindow::shaders = 0;

	glm::mat4 AbstractWindow::default_view_matrix =
		glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));

	AbstractWindow::AbstractWindow()
	: AbstractView(),
	  active_frame_(nullptr),
	  stencil_count_(0),
	  current_cursor_(ArrowCursor)
	{
		set_size(640, 480);
		set_refresh(true);

		events_.reset(new Cpp::ConnectionScope);

		//InitializeMatrices();
	}

	AbstractWindow::AbstractWindow (int width, int height)
	: AbstractView(width, height),
	  active_frame_(nullptr),
	  stencil_count_(0),
	  current_cursor_(ArrowCursor)
	{
		set_refresh(true);

		events_.reset(new Cpp::ConnectionScope);

		//InitializeMatrices();
	}

	AbstractWindow::~AbstractWindow ()
	{
	}

	bool AbstractWindow::AddFrame (AbstractFrame* frame, bool focus)
	{
		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		if(PushBackSubView(frame)) {

			if(focus) {

				if(original_last) {
					original_last->set_focus(false);
					original_last->PerformFocusOff(this);
				}

				frame->set_focus(true);
				frame->PerformFocusOn(this);

			}

			RequestRedraw();
			return true;
		}

		return false;
	}

	bool AbstractWindow::InsertFrame (int index, AbstractFrame* frame, bool focus)
	{
		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		if(InsertSubView(index, frame)) {

			if(focus) {

				if(original_last != last_subview()) {
					assert(last_subview() == frame);

					if(original_last) {
						original_last->set_focus(false);
						original_last->PerformFocusOff(this);
					}

					frame->set_focus(true);
					frame->PerformFocusOn(this);

				}

			}

			RequestRedraw();
			return true;
		}

		return false;
	}

	void AbstractWindow::MoveFrameToTop (AbstractFrame* frame, bool focus)
	{
		if(frame == nullptr) return;

		if(frame == last_subview()) return;

		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		MoveToLast(frame);

		if(focus) {
			if(original_last) {
				original_last->set_focus(false);
				original_last->PerformFocusOff(this);
			}

			frame->set_focus(true);
			frame->PerformFocusOn(this);
		}
	}

	bool AbstractWindow::Contain (const Point& point) const
	{
		return true;
	}

	void AbstractWindow::SetCursor(int cursor_type)
	{
		current_cursor_ = cursor_type;
	}

	void AbstractWindow::PushCursor ()
	{
		cursor_stack_.push(current_cursor_);
	}

	void AbstractWindow::PopCursor ()
	{
		int cursor = ArrowCursor;

		if(!cursor_stack_.empty()) {
			cursor = cursor_stack_.top();
			cursor_stack_.pop();
		}

		SetCursor(cursor);
	}

	void AbstractWindow::BeginPushStencil ()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		if(stencil_count_ == 0) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NEVER, 1, 0xFF);	// GL_NEVER: always fails
			glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
		} else {
			glStencilFunc(GL_LESS, stencil_count_, 0xFF);
			glStencilOp(GL_INCR, GL_KEEP, GL_KEEP); // increase 1s on test fail (always)
		}

		stencil_count_++;
	}

	void AbstractWindow::EndPushStencil ()
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void AbstractWindow::BeginPopStencil ()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_LESS, stencil_count_, 0xFF);
		glStencilOp(GL_DECR, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
	}

	void AbstractWindow::EndPopStencil ()
	{
		if(stencil_count_ > 0) {
			stencil_count_--;

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			if(stencil_count_ == 0) {
				glDisable(GL_STENCIL_TEST);
			}

		}
	}

	AbstractWindow* AbstractWindow::GetContext (AbstractView* widget)
	{
		AbstractView* superview = widget->superview();

		if(superview == 0) {
			return dynamic_cast<AbstractWindow*>(widget);
		} else {

			while(superview->superview()) {
				superview = superview->superview();
			}

		}

		return dynamic_cast<AbstractWindow*>(superview);
	}

	bool AbstractWindow::InitializeGLContext ()
	{
		bool success = true;

		Time::SaveCurrent();

#ifdef DEBUG
		int major, minor;
		GetGLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
		GetGLSLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

		if (success && InitializeTheme()) {
			DBG_PRINT_MSG("Time to intialize theme: %llu (ms)", Time::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && InitializeShaders()) {
			DBG_PRINT_MSG("Time to intialize shaders: %llu (ms)", Time::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s",
						  "The Shader Manager is not initialized successfully!");
			success = false;
		}

		if (success && InitializeIcons()) {
			DBG_PRINT_MSG("Time to intialize icons: %llu (ms)", Time::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		// Create Default font: must call this after theme initialized as it read the default_font
		if(success && InitializeFont()) {
			DBG_PRINT_MSG("Time to intialize font: %llu (ms)", Time::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize font");
			success = false;
		}

		if(success) {
			Time::SaveCurrent();
		}

		return success;

	}

	void AbstractWindow::ReleaseGLContext ()
	{
		ReleaseFont();
		ReleaseIcons();
		ReleaseShaders();
		ReleaseTheme();
	}

	bool AbstractWindow::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractWindow::PositionUpdateTest (
	        const PositionUpdateRequest& request)
	{
		return true;
	}

	bool AbstractWindow::PreDraw (AbstractWindow* context)
	{
		return true;
	}

	ResponseType AbstractWindow::Draw (AbstractWindow* context)
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			p->PreDraw(context);
			p->Draw(context);
			p->set_refresh(this->refresh());
			p->PostDraw(context);
		}

		return Finish;
	}

	void AbstractWindow::PostDraw (AbstractWindow* context)
	{
	}

	void AbstractWindow::PerformFocusOn (AbstractWindow* context)
	{
	}

	void AbstractWindow::PerformFocusOff (AbstractWindow* context)
	{
	}

	void AbstractWindow::PerformHoverIn (AbstractWindow* context)
	{
	}

	void AbstractWindow::PerformHoverOut (AbstractWindow* context)
	{
	}

	ResponseType AbstractWindow::PerformKeyPress (AbstractWindow* context)
	{
		ResponseType response = Ignore;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformKeyPress(context);
			if(response == Finish) break;
		}

		return response;
	}

	ResponseType AbstractWindow::PerformContextMenuPress (
	        AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformContextMenuRelease (
	        AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformMousePress (AbstractWindow* context)
	{
		ResponseType response = Ignore;

		set_pressed(true);

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformMousePress(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType AbstractWindow::PerformMouseRelease (AbstractWindow* context)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
		{
			response = p->PerformMouseRelease(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType AbstractWindow::PerformMouseMove (AbstractWindow* context)
	{
		ResponseType response = Ignore;

		if(pressed_ext()) {

			for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
			{
				response = p->PerformMouseMove(context);
				if(response == Finish) {
					break;
				}
			}

		}

		return response;
	}

	bool AbstractWindow::RemoveSubView (AbstractView* view)
	{
		AbstractFrame* new_last = nullptr;
		AbstractFrame* frame = dynamic_cast<AbstractFrame*>(view);

		if(view->next_view() == nullptr) {
			new_last = dynamic_cast<AbstractFrame*>(view->previous_view());

			if(frame != nullptr) {
				frame->set_focus(false);
			}
		}

		bool retval = AbstractView::RemoveSubView(view);

		if(new_last != nullptr) {
			DBG_PRINT_MSG("%s", "call focus event");
			new_last->set_focus(true);
			new_last->PerformFocusOn(this);
		}

		return retval;
	}

	bool AbstractWindow::InitializeTheme ()
	{
		if (!theme) {
			theme = new Theme;
			theme->Reset();
		}

		return true;
	}

	bool AbstractWindow::InitializeIcons ()
	{
		if (!icons)
			icons = new Icons;

		return true;
	}

	bool AbstractWindow::InitializeShaders ()
	{
		bool ret = true;

		if (!shaders) {
			shaders = new Shaders;

			if (shaders) {
				ret = shaders->Setup();
			} else {
				ret = false;
			}
		}

		return ret;
	}

	bool AbstractWindow::InitializeFont ()
	{
		bool retval = true;

		if(FontCache::kDefaultFontHash == 0) {

	        Fc::Pattern p = Fc::Pattern::name_parse((const FcChar8*)theme->default_font());

			Fc::Config::substitute(0, p, FcMatchPattern);
			p.default_substitute();

			FcResult result;
			Fc::Pattern match = Fc::Config::match(0, p, &result);

			if(match) {
				FontCache::Create(match);
				FontCache::kDefaultFontHash = match.hash();
			} else {
				retval = false;
			}

		}

		return retval;
	}

	void AbstractWindow::ReleaseTheme ()
	{
		if (theme) {
			delete theme;
			theme = 0;
		}
	}

	void AbstractWindow::ReleaseIcons ()
	{
		if (icons) {
			delete icons;
			icons = 0;
		}
	}

	void AbstractWindow::ReleaseShaders ()
	{
		if (shaders) {
			delete shaders;
			shaders = 0;
		}
	}

	void AbstractWindow::ReleaseFont ()
	{
		FontCache::ReleaseAll();
	}

	void AbstractWindow::GetGLVersion (int* major, int* minor)
	{
		const char* verstr = (const char*) glGetString(GL_VERSION);
		if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
			*major = *minor = 0;
			fprintf(stderr, "Invalid GL_VERSION format!!!\n");
		}
	}

	void AbstractWindow::GetGLSLVersion (int* major, int* minor)
	{
		int gl_major, gl_minor;
		GetGLVersion(&gl_major, &gl_minor);

		*major = *minor = 0;
		if(gl_major == 1) {
			/* GL v1.x can provide GLSL v1.00 only as an extension */
			const char* extstr = (const char*)glGetString(GL_EXTENSIONS);
			if((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
			{
				*major = 1;
				*minor = 0;
			}
		} else if (gl_major >= 2) {
			/* GL v2.0 and greater must parse the version string */
			const char* verstr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2))
			{
				*major = *minor = 0;
				fprintf(stderr, "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
			}
		}
	}

	void AbstractWindow::DispatchHoverEvent ()
	{
		ResponseType response = Ignore;
		AbstractFrame* frame = 0;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			frame = dynamic_cast<AbstractFrame*>(p);
			response = frame->DispatchHoverEvent(this);
			if(response == Finish) break;
		}
	}

}
