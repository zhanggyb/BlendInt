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
	BlendInt::Cursor* AbstractWindow::cursor = 0;

	glm::mat4 AbstractWindow::default_view_matrix =
		glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));

	AbstractWindow::AbstractWindow()
	: AbstractView(),
	  active_frame_(nullptr),
	  stencil_count_(0)
	{
		set_size(640, 480);
		set_refresh(true);

		events_.reset(new Cpp::ConnectionScope);

		//InitializeMatrices();
	}

	AbstractWindow::AbstractWindow (int width, int height)
	: AbstractView(width, height),
	  active_frame_(nullptr),
	  stencil_count_(0)
	{
		set_refresh(true);

		events_.reset(new Cpp::ConnectionScope);

		//InitializeMatrices();
	}

	AbstractWindow::~AbstractWindow ()
	{
	}

	bool AbstractWindow::Contain (const Point& point) const
	{
		return true;
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

		if (success && Fc::Config::init()) {
			// do nothing
		} else {

			DBG_PRINT_MSG("%s", "Cannot initialize FontConfig");
			success = false;

		}

		if (success && InitializeTheme()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && InitializeShaders()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s",
						  "The Shader Manager is not initialized successfully!");
			success = false;
		}

		if (success && InitializeIcons()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		if (success && InitializeCursor()) {
			// do nothing;
		} else {
			DBG_PRINT_MSG ("%s", "Cannot initilize Cursor");
			success = false;
		}

		// Create Default font: must call this after theme initialized as it read the default_font
		if(success && InitializeFont()) {
			// do nothing
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
		ReleaseCursor();
		Fc::Config::fini();
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

	void AbstractWindow::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
	}

	void AbstractWindow::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
	}

	bool AbstractWindow::PreDraw (const Context* context)
	{
		return true;
	}

	ResponseType AbstractWindow::Draw (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	void AbstractWindow::PostDraw (const Context* context)
	{
	}

	void AbstractWindow::PerformFocusOn (const Context* context)
	{
	}

	void AbstractWindow::PerformFocusOff (const Context* context)
	{
	}

	void AbstractWindow::PerformHoverIn (const Context* context)
	{
	}

	void AbstractWindow::PerformHoverOut (const Context* context)
	{
	}

	ResponseType AbstractWindow::PerformKeyPress (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformContextMenuPress (
	        const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformContextMenuRelease (
	        const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformMousePress (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformMouseRelease (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWindow::PerformMouseMove (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	bool AbstractWindow::InitializeTheme ()
	{
		bool result = false;

		if (!theme) {
			theme = new Theme;
		}

		if (theme) {
			theme->Reset();
			result = true;
		}

		return result;
	}

	bool AbstractWindow::InitializeIcons ()
	{
		if (!icons)
			icons = new Icons;

		return true;
	}

	bool AbstractWindow::InitializeShaders ()
	{
		bool ret = false;

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

	bool AbstractWindow::InitializeCursor ()
	{
		if(!cursor) {
			cursor = new Cursor;
			cursor->RegisterCursorType(new BlankCursorTheme);
		}

		return true;
	}

	bool AbstractWindow::InitializeFont ()
	{
        Fc::Pattern p = Fc::Pattern::name_parse((const FcChar8*)theme->default_font());

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		if(match) {
			FontCache::Create(match);
			FontCache::kDefaultFontHash = match.hash();
			return true;
		} else {
			return false;
		}
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

	void AbstractWindow::ReleaseCursor ()
	{
		if(cursor) {
			delete cursor;
			cursor = nullptr;
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

	void AbstractWindow::InitializeMatrices ()
	{
		glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		shaders->SetFrameProjectionMatrix(projection);
		shaders->SetFrameViewMatrix(default_view_matrix);
		shaders->SetFrameModelMatrix(glm::mat3(1.f));

		shaders->SetWidgetViewMatrix(default_view_matrix);
		shaders->SetWidgetModelMatrix(glm::mat3(1.f));
	}

}
