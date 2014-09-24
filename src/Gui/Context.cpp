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
	: AbstractContainer(),
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

	Section* Context::Append (AbstractWidget* widget)
	{
		/*
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
		*/

		return 0;
	}

	Section* Context::Remove (AbstractWidget* widget)
	{
		/*
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
		*/
		return 0;
	}

	void Context::Draw()
	{
		Draw(profile_);
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

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			p->PreDraw(profile);
			p->Draw(profile);
			p->PostDraw(profile);
		}

		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).context_ = this;

		ResponseType response;

		if(last()) {
			response = last()->KeyPressEvent(event);
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

		for(AbstractWidget* p = last(); p; p = p->previous()) {

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

		for(AbstractWidget* p = last(); p; p = p->previous()) {
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
		for(AbstractWidget* p = last(); p; p = p->previous()) {
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

		if(last()) {
			response = last()->MouseMoveEvent(event);
		}

		if(hover_ && hover_ != last()) {
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
