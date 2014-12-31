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

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif
#include <BlendInt/Core/Time.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/Context.hpp>

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

		if(success) {
			Time::SaveCurrent();
		}

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
	: AbstractView()
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
		if(superview() != 0) {
			DBG_PRINT_MSG("Error: %s", "Context MUST NOT be in any other superview");
		}
		context_set.erase(this);
	}

	bool Context::AddFrame (AbstractFrame* frame)
	{
		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		if(PushBackSubView(frame)) {

			if(original_last) {
				original_last->set_focus(false);
				original_last->FocusEvent(false);
			}

			frame->set_focus(true);
			frame->FocusEvent(true);

			RequestRedraw();
			return true;
		}

		return false;
	}

	bool Context::InsertFrame(int index, AbstractFrame* frame)
	{
		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		if(InsertSubView(index, frame)) {

			if(original_last != last_subview()) {
				assert(last_subview() == frame);

				if(original_last) {
					original_last->set_focus(false);
					original_last->FocusEvent(false);
				}

				frame->set_focus(true);
				frame->FocusEvent(true);

			}

			RequestRedraw();
			return true;
		}

		return false;
	}

	void Context::MoveFrameToTop(AbstractFrame* frame)
	{
		if(frame == nullptr) return;

		if(frame == last_subview()) return;

		AbstractFrame* original_last = dynamic_cast<AbstractFrame*>(last_subview());

		MoveToLast(frame);

		if(original_last) {
			original_last->set_focus(false);
			original_last->FocusEvent(false);
		}

		frame->set_focus(true);
		frame->FocusEvent(true);
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

		set_refresh(false);
		if(PreDraw(profile_)) {
			Draw(profile_);
			PostDraw(profile_);
		}

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

	void Context::DispatchMouseEvent(int x, int y, const MouseEvent& event)
	{
		cursor_position_.reset(x, size().height() - y);
		const_cast<MouseEvent&>(event).context_ = this;
		const_cast<MouseEvent&>(event).frame_ = 0;

		switch (event.action()) {

			case MouseMove: {
				DispatchHoverEvent(event);
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

	Context* Context::GetContext (AbstractView* widget)
	{
		AbstractView* superview = widget->superview();

		if(superview == 0) {
			return dynamic_cast<Context*>(widget);
		} else {

			while(superview->superview()) {
				superview = superview->superview();
			}

		}

		return dynamic_cast<Context*>(superview);
	}

	bool Context::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Context::PositionUpdateTest (const PositionUpdateRequest& request)
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

	bool Context::PreDraw(Profile& profile)
	{
		return true;
	}

	ResponseType Context::Draw (Profile& profile)
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			p->PreDraw(profile);
			p->Draw(profile);
			p->set_refresh(this->refresh());
			p->PostDraw(profile);
		}

		return Finish;
	}

	void Context::PostDraw(Profile& profile)
	{
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		ResponseType response = Ignore;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->KeyPressEvent(event);
			if(response == Finish) break;
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

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->MousePressEvent(event);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
		{
			response = p->MouseReleaseEvent(event);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType response = Ignore;

		if(pressed_ext()) {

			for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
			{
				response = p->MouseMoveEvent(event);
				if(response == Finish) {
					break;
				}
			}

		}

		return response;
	}

	bool Context::RemoveSubView(AbstractView* view)
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
			new_last->FocusEvent(true);
		}

		return retval;
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
		glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		Shaders::instance->SetFrameProjectionMatrix(projection);
		Shaders::instance->SetFrameViewMatrix(default_view_matrix);
		Shaders::instance->SetFrameModelMatrix(glm::mat3(1.f));

		Shaders::instance->SetWidgetViewMatrix(default_view_matrix);
		Shaders::instance->SetWidgetModelMatrix(glm::mat3(1.f));

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
		ResponseType response = Ignore;
		AbstractFrame* frame = 0;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			frame = dynamic_cast<AbstractFrame*>(p);
			response = frame->DispatchHoverEvent(event);
			if(response == Finish) break;
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
