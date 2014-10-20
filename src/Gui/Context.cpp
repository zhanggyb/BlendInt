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

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Cursor.hpp>

namespace BlendInt
{
	using Stock::Shaders;

	glm::mat4 Context::default_view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f));

	std::set<Context*> Context::context_set;

	bool Context::Initialize()
	{
		using Stock::Icons;
		using Stock::Shaders;

		bool success = true;

#ifdef DEBUG
		int major, minor;
		GetGLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
		GetGLSLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

#ifdef USE_FONTCONFIG
		if (success && FontConfig::initialize()) {

			/*
			 FontConfig* ftconfig = FontConfig::instance();
			 if (!ftconfig->loadDefaultFontToMem()) {
			 cerr << "Cannot load default font into memory" << endl;
			 success = false;
			 }
			 */

		} else {

			DBG_PRINT_MSG("%s", "Cannot initialize FontConfig");
			success = false;

		}
#endif

		if (success && Theme::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && Shaders::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "The Shader Manager is not initialized successfully!");
			success = false;
		}

		if (success && Icons::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		if (success && Cursor::Initialize()) {
			// do nothing;
		} else {
			DBG_PRINT_MSG ("%s", "Cannot initilize Cursor");
			success = false;
		}

#ifdef USE_FONTCONFIG

#ifdef __APPLE__

		// Create a default font
		//FontCache::Create("Sans-Serif", 9, 96, false, false);

#endif

#ifdef __LINUX__

		// Create a default font
		//FontCache::Create("Sans", 9, 96, false, false);

#endif

#endif

		return success;
	}

	void Context::Release()
	{
		using Stock::Icons;
		using Stock::Shaders;

		while(context_set.size()) {
			std::set<Context*>::iterator it = context_set.begin();

			if((*it)->managed() && ((*it)->reference_count() == 0)) {
				DBG_PRINT_MSG("%s", "Delete context");
				delete (*it);	// this will erase the context from the set
			} else {
				context_set.erase(it);
			}
		}

		Icons::Release();
		Shaders::Release();
		Theme::Release();
		FontCache::ReleaseAll();
		Cursor::Release();

#ifdef USE_FONTCONFIG
		FontConfig::release();
#endif
	}

	Context::Context ()
	: AbstractWidget(),
	  hovered_frame_(0),
	  focused_frame_(0)
	{
		set_size(640, 480);
		profile_.context_ = this;
		InitializeContext();

		context_set.insert(this);
	}

	Context::~Context ()
	{
		if(parent() != 0) {
			DBG_PRINT_MSG("Error: %s", "Context MUST NOT be in any other parent");
		}
		context_set.erase(this);
	}

	void Context::AddFrame (AbstractFrame* vp)
	{
		if(PushBackSubWidget(vp)) {
			// TODO: 			
		}
	}

	void Context::Draw()
	{
		PreDraw(profile_);
		Draw(profile_);
		PostDraw(profile_);
	}

	void Context::DispatchKeyEvent(const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).context_ = this;
		const_cast<KeyEvent&>(event).frame_ = 0;

		switch (event.action()) {

			case KeyPress: {
				KeyPressEvent(event);
				break;
			}

			case KeyRelease: {
				// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
				//cm->m_focus->KeyReleaseEvent(event);
				break;
			}

			case KeyRepeat: {
				// item->KeyRepeatEvent(&event);
				break;
			}

			default:
			break;
		}
	}

	void Context::DispatchMouseEvent(const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).context_ = this;
		const_cast<MouseEvent&>(event).frame_ = 0;

		switch (event.action()) {

			case MouseMove: {
				DispatchHoverEvent(event);
				if(hovered_frame_) {
					hovered_frame_->DispatchHoverEvent(event);
				}

				MouseMoveEvent(event);
				return;
			}

			case MousePress: {
				MousePressEvent(event);
				return;
			}

			case MouseRelease: {
				MouseReleaseEvent(event);
				return;
			}

			default:
				break;
		}
	}

	bool Context::Contain (const Point& point) const
	{
		return true;
	}

	Context* Context::GetContext (AbstractWidget* widget)
	{
		AbstractWidget* parent = widget->parent();

		if(parent == 0) {
			return dynamic_cast<Context*>(widget);
		} else {

			while(parent->parent()) {
				parent = parent->parent();
			}

		}

		return dynamic_cast<Context*>(parent);
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
		return true;
	}

	bool Context::RoundRadiusUpdateTest (
	        const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void Context::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
	}

	void Context::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size(*request.size());

			glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
			Shaders::instance->SetFrameProjectionMatrix(projection);

			resized_.fire(size());
		}
	}

	void Context::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
	}

	void Context::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
	}

	void Context::PreDraw(Profile& profile)
	{

	}

	ResponseType Context::Draw (Profile& profile)
	{
		//glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearStencil(0);
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT |
				GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		set_refresh(false);
		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			p->PreDraw(profile);
			p->Draw(profile);
			p->PostDraw(profile);
			p->set_refresh(this->refresh());
		}

		return Accept;
	}

	void Context::PostDraw(Profile& profile)
	{
		set_refresh(false);
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		ResponseType response;

		if(last_child()) {
			response = last_child()->KeyPressEvent(event);
		}

		return response;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).context_ = this;

		return Ignore;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).context_ = this;

		return Ignore;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		ResponseType response;

		set_pressed(true);
		for(AbstractWidget* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {
				response = p->MousePressEvent(event);

				//p->MoveToLast();
				break;
			}
		}

		SetFocusedFrame(event.frame());

		if(focused_frame_) focused_frame_->set_pressed(true);

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType response;

		set_pressed(false);
		if(focused_frame_) {
			focused_frame_->set_pressed(false);
			response = focused_frame_->MouseReleaseEvent(event);
		}

		return response;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType response = Ignore;

		if(pressed_ext() && focused_frame_) {
			response = focused_frame_->MouseMoveEvent(event);
		}

		return response;
	}

	void Context::GetGLVersion (int *major, int *minor)
	{
		const char* verstr = (const char*) glGetString(GL_VERSION);
		if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
			*major = *minor = 0;
			fprintf(stderr, "Invalid GL_VERSION format!!!\n");
		}
	}

	void Context::GetGLSLVersion (int *major, int *minor)
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

	void Context::InitializeContext ()
	{
		glm::mat4 identity = glm::mat4(1.f);

		glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		Shaders::instance->SetFrameProjectionMatrix(projection);
		Shaders::instance->SetFrameViewMatrix(default_view_matrix);
		Shaders::instance->SetFrameModelMatrix(identity);

		Shaders::instance->SetWidgetViewMatrix(default_view_matrix);
		Shaders::instance->SetWidgetModelMatrix(identity);
	}

	void Context::DispatchHoverEvent(const MouseEvent& event)
	{
		AbstractFrame* original_hover = hovered_frame_;

		hovered_frame_ = 0;
		for(AbstractWidget* p = last_child(); p; p = p->previous()) {
			if(p->Contain(event.position())) {
				hovered_frame_ = dynamic_cast<AbstractFrame*>(p);
				break;
			}
		}

		if(original_hover != hovered_frame_) {

			if(original_hover) {
				original_hover->set_hover(false);
				original_hover->MouseHoverOutEvent(event);
				original_hover->destroyed().disconnectOne(this, &Context::OnHoverFrameDestroyed);
			}

			if(hovered_frame_) {
				hovered_frame_->set_hover(true);
				hovered_frame_->MouseHoverInEvent(event);
				events()->connect(hovered_frame_->destroyed(), this, &Context::OnHoverFrameDestroyed);
			}

		}
	}

	void Context::FocusEvent(bool focus)
	{
	}

	void Context::MouseHoverInEvent(const MouseEvent& event)
	{
	}

	void Context::MouseHoverOutEvent(const MouseEvent& event)
	{
	}

	void Context::OnHoverFrameDestroyed(AbstractFrame* frame)
	{
		assert(frame->hover());
		assert(hovered_frame_ == frame);

		DBG_PRINT_MSG("unset hover status of widget %s", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Context::OnHoverFrameDestroyed);

		hovered_frame_ = 0;
	}

	void Context::SetFocusedFrame(AbstractFrame* frame)
	{
		if(focused_frame_ == frame) return;

		if(focused_frame_) {
			focused_frame_->set_focus(false);
			focused_frame_->FocusEvent(false);
			focused_frame_->destroyed().disconnectOne(this, &Context::OnFocusedFrameDestroyed);
		}

		focused_frame_ = frame;
		if(focused_frame_) {
			focused_frame_->set_focus(true);
			focused_frame_->FocusEvent(true);
			events()->connect(focused_frame_->destroyed(), this, &Context::OnFocusedFrameDestroyed);
		}
	}

	void Context::OnFocusedFrameDestroyed(AbstractFrame* frame)
	{
		assert(focused_frame_ == frame);
		DBG_PRINT_MSG("focused frame %s destroyed", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Context::OnFocusedFrameDestroyed);

		focused_frame_ = 0;
	}

}
