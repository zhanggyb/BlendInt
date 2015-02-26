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

#include <core/timer.hpp>

#include <opengl/opengl.hpp>

#include <font/fc-pattern.hpp>
#include <font/fc-config.hpp>

#include <gui/abstract-window.hpp>

namespace BlendInt {

	Theme* AbstractWindow::theme = 0;
	Icons* AbstractWindow::icons = 0;
	Shaders* AbstractWindow::shaders = 0;

	glm::mat4 AbstractWindow::default_view_matrix =
		glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));

	AbstractWindow::AbstractWindow(bool visible)
	: AbstractView(),
	  active_frame_(nullptr),
	  focused_frame_(nullptr),
	  stencil_count_(0),
	  current_cursor_shape_(ArrowCursor),
	  floating_frame_count_(0),
	  pressed_(false)
	{
		set_size(640, 480);
		set_refresh(true);
		set_visible(visible);

		events_.reset(new Cpp::ConnectionScope);
	}

	AbstractWindow::AbstractWindow (int width, int height, bool visible)
	: AbstractView(width, height),
	  active_frame_(nullptr),
	  focused_frame_(nullptr),
	  stencil_count_(0),
	  current_cursor_shape_(ArrowCursor),
	  floating_frame_count_(0),
	  pressed_(false)
	{
		set_refresh(true);
		set_visible(visible);

		events_.reset(new Cpp::ConnectionScope);
	}

	AbstractWindow::~AbstractWindow ()
	{
	}

	bool AbstractWindow::AddFrame (AbstractFrame* frame)
	{
		if(frame == nullptr) return false;

		if(frame->floating()) {

			if(PushBackSubView(frame)) {

				if(frame->focusable()) {
					if(focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
					focused_frame_ = frame;
					focused_frame_->PerformFocusOn(this);
				}

				floating_frame_count_++;
				RequestRedraw();
			}

		} else {

			int index = subs_count() - floating_frame_count_ - 1;

			if(index < 0) {	// no normal frame

				if(InsertSubView(0, frame)) {
					if(frame->focusable()) {
						if(focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
						focused_frame_ = frame;
						focused_frame_->PerformFocusOn(this);
					}
					RequestRedraw();
				}

			} else {

				AbstractFrame* top_normal_frame = dynamic_cast<AbstractFrame*>(GetSubViewAt(index));
				if(InsertSiblingAfter(top_normal_frame, frame)) {
					if(frame->focusable()) {
						if(focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
						focused_frame_ = frame;
						focused_frame_->PerformFocusOn(this);
					}
					RequestRedraw();
				}

			}

		}

		return true;
	}

	bool AbstractWindow::SetFocusedFrame(AbstractFrame *frame)
	{
		if(focused_frame_ == frame) return true;

		if(frame == nullptr) {

			if(focused_frame_ != nullptr) {
				assert(focused_frame_->focusable());
				focused_frame_->PerformFocusOff(this);
			}

			return true;
		}

		if(!frame->focusable()) {
			DBG_PRINT_MSG("%s", "Frame is not focusable!");
			return false;
		}

		// if frame is not the root frame in this window, find and switch to it
		if(frame->superview_ != this) {

			AbstractView* root_frame = frame;
			AbstractView* window = root_frame->superview_;

			while(window->superview_ != nullptr) {
				root_frame = root_frame->superview_;
				window = root_frame->superview_;
			}

			if(window != this) {
				DBG_PRINT_MSG("%s", "the frame is not in this window");
				return false;
			}

			frame = dynamic_cast<AbstractFrame*>(root_frame);
			if(frame == nullptr) return false;
		}

		if(frame->floating()) {

			if(focused_frame_ != nullptr) {
				assert(focused_frame_->focusable());
				focused_frame_->PerformFocusOff(this);
			}

			MoveToLast(frame);
			focused_frame_ = frame;
			focused_frame_->PerformFocusOn(this);

			RequestRedraw();
			return true;

		}

		int index = subs_count() - floating_frame_count_ - 1;

		if(index < 0) return false;	// no normal frame

		AbstractFrame* top_regular_frame = dynamic_cast<AbstractFrame*>(GetSubViewAt(index));
		// move the frame to the top
		if(InsertSiblingAfter(top_regular_frame, frame)) {
			if(focused_frame_ != nullptr) focused_frame_->PerformFocusOff(this);
			focused_frame_ = frame;
			focused_frame_->PerformFocusOn(this);
			RequestRedraw();
		}

	 	return true;
	}

	bool AbstractWindow::Contain (const Point& point) const
	{
		return true;
	}

	void AbstractWindow::SetCursor(CursorShape cursor_type)
	{
		current_cursor_shape_ = cursor_type;
	}

	void AbstractWindow::PushCursor ()
	{
		cursor_stack_.push(current_cursor_shape_);
	}

	void AbstractWindow::PopCursor ()
	{
		CursorShape cursor = ArrowCursor;

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

	Point AbstractWindow::GetAbsolutePosition (const AbstractWidget* widget)
	{
#ifdef DEBUG
		assert(widget);
#endif

		AbstractFrame* frame = 0;
		Point pos = widget->position();

		AbstractView* p = widget->superview();
		while(p && (p != this)) {
			frame = dynamic_cast<AbstractFrame*>(p);
			if(frame) break;

			pos = pos + p->position() + p->GetOffset();
			p = p->superview();
		}

		pos = pos + frame->position() + frame->GetOffset();
		return pos;
	}

	Point AbstractWindow::GetRelativePosition (const AbstractWidget* widget)
	{
#ifdef DEBUG
		assert(widget);
#endif

		AbstractFrame* frame = 0;
		Point pos = widget->position();

		AbstractView* p = widget->superview();
		while(p && (p != this)) {

			frame = dynamic_cast<AbstractFrame*>(p);
			if(frame) break;

			pos = pos + p->position() + p->GetOffset();
			p = p->superview();
		}

		return pos;
	}

	AbstractWindow* AbstractWindow::GetWindow (AbstractView* widget)
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

#ifdef DEBUG
		int major, minor;
		GetGLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
		GetGLSLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

		Timer::SaveCurrentTime();
		if (success && InitializeTheme()) {
			DBG_PRINT_MSG("Timer to intialize theme: %g (ms)", Timer::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		Timer::SaveCurrentTime();
		if (success && InitializeShaders()) {
			DBG_PRINT_MSG("Timer to intialize shaders: %g (ms)", Timer::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s",
						  "The Shader Manager is not initialized successfully!");
			success = false;
		}

		Timer::SaveCurrentTime();
		if (success && InitializeIcons()) {
			DBG_PRINT_MSG("Timer to intialize icons: %g (ms)", Timer::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		Timer::SaveCurrentTime();
		// Create Default font: must call this after theme initialized as it read the default_font
		if(success && InitializeFont()) {
			DBG_PRINT_MSG("Timer to intialize font: %g (ms)", Timer::GetIntervalOfMilliseconds());
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize font");
			success = false;
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

	Response AbstractWindow::Draw (AbstractWindow* context)
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

	Response AbstractWindow::PerformKeyPress (AbstractWindow* context)
	{
		Response response = Ignore;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformKeyPress(context);
			if(response == Finish) break;
		}

		return response;
	}

	Response AbstractWindow::PerformContextMenuPress (
	        AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response AbstractWindow::PerformContextMenuRelease (
	        AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response AbstractWindow::PerformMousePress (AbstractWindow* context)
	{
		Response response = Ignore;

		pressed_ = true;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformMousePress(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	Response AbstractWindow::PerformMouseRelease (AbstractWindow* context)
	{
		Response response = Ignore;

		pressed_ = false;

		for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
		{
			response = p->PerformMouseRelease(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	Response AbstractWindow::PerformMouseMove (AbstractWindow* context)
	{
		Response response = Ignore;

		if(pressed_) {

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
		AbstractFrame* frame = dynamic_cast<AbstractFrame*>(view);
		if(frame->floating()) {
			floating_frame_count_--;
			assert(floating_frame_count_ >= 0);
		}

		if(frame == focused_frame_) {

			AbstractView* prev = frame->previous_view_;
			AbstractFrame* previous_frame = dynamic_cast<AbstractFrame*>(prev);

			while((prev != nullptr) && (!previous_frame->focusable())) {
				prev = prev->previous_view_;
				previous_frame = dynamic_cast<AbstractFrame*>(prev);
			}

			focused_frame_ = previous_frame;
			if(focused_frame_ != nullptr) {
				focused_frame_->PerformFocusOn(this);
			}

		}

		return AbstractView::RemoveSubView(view);
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

	void AbstractWindow::PerformMouseHover ()
	{
		Response response = Ignore;
		AbstractFrame* frame = 0;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			frame = dynamic_cast<AbstractFrame*>(p);
			response = frame->PerformMouseHover(this);
			if(response == Finish) break;
		}
	}

}
