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
	  hover_(0)
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
		context_set.erase(this);
	}

	void Context::AddScreen (AbstractScreen* vp)
	{
		if(PushBackSubWidget(vp)) {
			// TODO: 			
		}
	}

	void Context::Draw()
	{
		PreDraw(profile_);
		Draw(profile_);
		set_refresh(false);
		PostDraw(profile_);
	}

	void Context::DispatchKeyEvent(const KeyEvent& event)
	{
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
		switch (event.action()) {

			case MouseMove: {
				//DispatchCursorMoveEvent(event);
				MouseMoveEvent(event);
				return;
			}

			case MousePress: {
				//DispatchMousePressEvent(event);
				MousePressEvent(event);
				return;
			}

			case MouseRelease: {
				//DispatchMouseReleaseEvent(event);
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
		AbstractWidget* container = widget->container();

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
	}

	void Context::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.source() == this) {
			set_size(*request.size());
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

	ResponseType Context::Draw (Profile& profile)
	{
		glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		glClearStencil(0);
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT |
				GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		for(AbstractWidget* p = first_sub_widget(); p; p = p->next())
		{
			p->PreDraw(profile);
			p->Draw(profile);
			p->set_refresh(false);
			p->PostDraw(profile);
		}

		return Accept;
	}

	void Context::PostDraw(Profile& profile)
	{
		set_refresh(false);
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).context_ = this;

		ResponseType response;

		if(last_sub_widget()) {
			response = last_sub_widget()->KeyPressEvent(event);
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
		const_cast<MouseEvent&>(event).context_ = this;

		ResponseType response;

		for(AbstractWidget* p = last_sub_widget(); p; p = p->previous()) {

			if(p->Contain(event.position())) {
				response = p->MousePressEvent(event);

				p->MoveToLast();
				break;
			}
		}

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).context_ = this;

		ResponseType response;

		for(AbstractWidget* p = last_sub_widget(); p; p = p->previous()) {
			response = p->MouseReleaseEvent(event);

			if(response == Accept) break;
		}

		return response;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).context_ = this;

		ResponseType response = Ignore;

		AbstractScreen* original_hover = hover_;

		hover_ = 0;
		for(AbstractWidget* p = last_sub_widget(); p; p = p->previous()) {
			if(p->Contain(event.position())) {
				hover_ = dynamic_cast<AbstractScreen*>(p);
				break;
			}
		}

		if(original_hover != hover_) {

			if(original_hover) {
				original_hover->set_hover(false);
				original_hover->CursorEnterEvent(false);
				original_hover->destroyed().disconnectOne(this, &Context::OnHoverViewportDestroyed);
			}

			if(hover_) {
				hover_->set_hover(true);
				hover_->CursorEnterEvent(true);
				events()->connect(hover_->destroyed(), this, &Context::OnHoverViewportDestroyed);
			}

		}

		if(last_sub_widget()) {
			response = last_sub_widget()->MouseMoveEvent(event);
		}

		if(hover_ && hover_ != last_sub_widget()) {
			hover_->MouseMoveEvent(event);
		}

		return response;
	}

	ResponseType Context::FocusEvent (bool focus)
	{
		return Ignore;
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
		glm::mat4 projection = glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f);

		Shaders::instance->SetUIProjectionMatrix(projection);
		Shaders::instance->SetUIViewMatrix(default_view_matrix);
	}

	void Context::OnHoverViewportDestroyed(AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(hover_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Context::OnHoverViewportDestroyed);

		hover_ = 0;
	}

}
