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
	: AbstractInteractiveForm(),
	  hovered_frame_(0),
	  focused_frame_(0)
	{
		set_size(640, 480);
		set_refresh(true);

		profile_.context_ = this;

		events_.reset(new Cpp::ConnectionScope);

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
		if(PushBackSubForm(vp)) {
			// TODO:
			Refresh();
		}
	}

	void Context::Draw()
	{
		glClearColor(0.208f, 0.208f, 0.208f, 1.f);
		//glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearStencil(0);
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT |
				GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		/*
		if(refresh()) {

			DBG_PRINT_MSG("%s", "Render to texture once");
			set_refresh(false);
			RenderToBuffer(profile_);
		}

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		Shaders::instance->frame_image_program()->use();
		texture_buffer_.bind();
		glUniform2f(Shaders::instance->location(Stock::FRAME_IMAGE_POSITION), 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::FRAME_IMAGE_TEXTURE), 0);
		glUniform1i(Shaders::instance->location(Stock::FRAME_IMAGE_GAMMA), 0);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		texture_buffer_.reset();
		GLSLProgram::reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*/

		// TODO: mutex lock before render all widgets
		pthread_mutex_lock(&refresh_mutex);

		set_refresh(false);
		if(PreDraw(profile_)) {
			Draw(profile_);
			PostDraw(profile_);
		}

		// TODO: mutex unlock after
		pthread_mutex_unlock(&refresh_mutex);
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

	void Context::SynchronizeWindow()
	{
		// TODO: override this function
	}

	void Context::MakeGLContextCurrent()
	{
		// TODO: override this to support GL Context manipulation in thread
	}

	Context* Context::GetContext (AbstractInteractiveForm* widget)
	{
		AbstractInteractiveForm* parent = widget->parent();

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

			/*
			GLfloat vertices[] = {
					// coord						uv
					0.f, 0.f,						0.f, 0.f,
					(float)size().width(), 0.f,		1.f, 0.f,
					0.f, (float)size().height(),	0.f, 1.f,
					(float)size().width(), (float)size().height(),		1.f, 1.f
			};

			vertex_buffer_.bind();
			vertex_buffer_.set_data(sizeof(vertices), vertices);
			vertex_buffer_.reset();
			*/

			set_refresh(true);

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

	bool Context::PreDraw(Profile& profile)
	{
		return true;
	}

	ResponseType Context::Draw (Profile& profile)
	{
		for(AbstractInteractiveForm* p = first_child(); p; p = p->next())
		{
			p->PreDraw(profile);
			p->Draw(profile);
			p->set_refresh(this->refresh());
			p->PostDraw(profile);
		}

		return Accept;
	}

	void Context::PostDraw(Profile& profile)
	{
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractInteractiveForm* p = last_child(); p; p = p->previous()) {
			response = p->KeyPressEvent(event);
			if(response == Accept) break;
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
		ResponseType response = Ignore;
		assert(event.frame() == 0);

		set_pressed(true);

		/*
		for(AbstractInteractiveForm* p = last_child(); p; p = p->previous()) {

			if(p->Contain(event.position())) {
				response = p->MousePressEvent(event);

				//p->MoveToLast();
				break;
			}
		}
		*/

		if(hovered_frame_ && hovered_frame_->Contain(event.position())) {
			response = hovered_frame_->MousePressEvent(event);
		}

		SetFocusedFrame(event.frame());

		if(focused_frame_) focused_frame_->set_pressed(true);

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType response = Ignore;

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

		/*
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		vertex_buffer_.generate();

		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		vertex_buffer_.bind();
		vertex_buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::FRAME_IMAGE_COORD));
		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::FRAME_IMAGE_UV));
		glVertexAttribPointer (Shaders::instance->location (Stock::FRAME_IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Shaders::instance->location (Stock::FRAME_IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vertex_buffer_.reset();
		*/
	}

	void Context::DispatchHoverEvent(const MouseEvent& event)
	{
		AbstractFrame* original_hover = hovered_frame_;

		ResponseType response = Ignore;
		hovered_frame_ = 0;
		AbstractFrame* temp = 0;

		for(AbstractInteractiveForm* p = last_child(); p; p = p->previous()) {

			temp = dynamic_cast<AbstractFrame*>(p);
			response = temp->DispatchHoverEvent(event);

			if(response == Accept) {
				break;
			}

		}

		hovered_frame_ = event.frame();

		if(original_hover != hovered_frame_) {

			if(original_hover) {
				original_hover->set_hover(false);
				original_hover->MouseHoverOutEvent(event);
				original_hover->destroyed().disconnectOne(this, &Context::OnHoverFrameDestroyed);
			}

			if(hovered_frame_) {
				hovered_frame_->set_hover(true);
				hovered_frame_->MouseHoverInEvent(event);
				events_->connect(hovered_frame_->destroyed(), this, &Context::OnHoverFrameDestroyed);
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
			events_->connect(focused_frame_->destroyed(), this, &Context::OnFocusedFrameDestroyed);
		}
	}

	void Context::OnHoverFrameDestroyed(AbstractFrame* frame)
	{
		assert(frame->hover());
		assert(hovered_frame_ == frame);

		DBG_PRINT_MSG("unset hover status of widget %s", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Context::OnHoverFrameDestroyed);

		hovered_frame_ = 0;

		Refresh();
	}

	void Context::OnFocusedFrameDestroyed(AbstractFrame* frame)
	{
		assert(focused_frame_ == frame);
		DBG_PRINT_MSG("focused frame %s destroyed", frame->name().c_str());
		frame->destroyed().disconnectOne(this, &Context::OnFocusedFrameDestroyed);

		focused_frame_ = 0;

		Refresh();
	}

	/*
    void Context::RenderToBuffer(Profile &profile)
    {
        // Create and set texture to render to.
        GLTexture2D* tex = &texture_buffer_;
        if(!tex->id())
            tex->generate();
        
        tex->bind();
        tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
        tex->SetMinFilter(GL_NEAREST);
        tex->SetMagFilter(GL_NEAREST);
        tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        
        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
        GLFramebuffer* fb = new GLFramebuffer;
        fb->generate();
        fb->bind();
        
        // Set "renderedTexture" as our colour attachement #0
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, tex->id(), 0);
        //fb->Attach(*tex, GL_COLOR_ATTACHMENT0);
        
        // Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
        GLuint rb;
        glGenRenderbuffers(1, &rb);
        
        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
                              size().width(), size().height());
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, rb);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, rb);
        
        if(GLFramebuffer::CheckStatus()) {
            
            fb->bind();

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClearDepth(1.0);
            glClearStencil(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            
            // Draw context:
    		if(PreDraw(profile_)) {
    			Draw(profile_);
    			PostDraw(profile_);
    		}

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
        }

        fb->reset();
        tex->reset();
        
        //delete tex; tex = 0;
        
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &rb);
        
        fb->reset();
        delete fb; fb = 0;
    }
    */
    
}
