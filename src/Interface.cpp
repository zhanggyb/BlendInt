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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/Interface.hpp>

#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/FontConfig.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/MouseEvent.hpp>
#include <BlendInt/Window/ContextMenuEvent.hpp>
#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Service/StockItems.hpp>

#include "Intern/ScreenBuffer.hpp"

namespace BlendInt {

	Interface* Interface::instance = 0;

	bool Interface::Initialize ()
	{
		/*
		 glewExperimental = true;	// Needed in core profile
		 if (glewInit() != GLEW_OK) {
		 std::cerr << "Fail to initialize GLEW" << endl;
		 return false;
		 }

		 if (!GLEW_VERSION_2_0) {
		 std::cerr << "No support for OpenGL 2.0 found" << std::endl;
		 return false;
		 }
		 */

		bool success = true;

		if (!instance) {
			instance = new Interface();
		}

		if (!instance)
		success = false;

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

		if (success && ThemeManager::initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && ShaderManager::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Shader Manager");
			success = false;
		}

		if (success && StockItems::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		if (success && ContextManager::Initialize()) {

		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Context Manager");
			success = false;
		}

		return success;
	}

	void Interface::Release ()
	{
		ContextManager::Release();
		StockItems::Release();
		ShaderManager::Release();
		ThemeManager::release();
		FontCache::releaseAll();
		FontConfig::release();

		if (instance) {
			delete instance;
			instance = 0;
		}
	}

	Interface::Interface ()
	: m_focus_style(FocusOnClick)
	{
		m_events.reset(new Cpp::ConnectionScope);
	}

	Interface::~Interface ()
	{
	}

	const Size& Interface::size () const
	{
		return ContextManager::instance->size();
	}

	void Interface::Resize (const Size& size)
	{
		ContextManager::instance->ResizeFromInterface(size);
	}

	void Interface::Resize (unsigned int width, unsigned int height)
	{
		ContextManager::instance->ResizeFromInterface(width, height);
	}

	void Interface::Draw ()
	{
		ContextManager::instance->Draw();
	}

	void Interface::SetMainWidget(AbstractWidget* widget)
	{
	}

	void Interface::DispatchKeyEvent (KeyEvent* event)
	{
		if(!event) return;

		if(AbstractWidget::focused_widget) {
			switch (event->action()) {

				case KeyPress: {
#ifdef DEBUG
					if(event->key() == Key_F6 && event->text().empty()) {
						//DrawToOffScreen();
						//RenderToImage();
					}
#endif
					AbstractWidget::focused_widget->KeyPressEvent(event);
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
	}

	void Interface::DispatchMouseEvent (MouseEvent* event)
	{
		if (!event)
		return;

		switch (event->action()) {

			case MouseMove: {
				DispatchCursorMoveEvent(event);
				return;
			}

			case MousePress: {
				DispatchMousePressEvent(event);
				return;
			}

			case MouseRelease: {
				DispatchMouseReleaseEvent(event);
				return;
			}

			default:
			break;
		}
	}

	void Interface::RenderToImage()
	{
		GLsizei width = ContextManager::instance->size().width();
		GLsizei height = ContextManager::instance->size().height();

		// Create and set texture to render to.
		GLTexture2D* tex = new GLTexture2D;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(width, height, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->id(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				width, height);

		//-------------------------

//		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		//-------------------------
		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
			DBG_PRINT_MSG("%s", "good");
			break;
			default:
			DBG_PRINT_MSG("%s", "Fail to check framebuffer status");
			break;
		}

		//-------------------------
		//and now render to GL_TEXTURE_2D
		fb->Bind();

		Draw();

		//Bind 0, which means render to back buffer
		fb->Reset();

		tex->WriteToFile("output.png");
		tex->Reset();

		//Delete resources
		delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		//Bind 0, which means render to back buffer, as a result, fb is unbound
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		fb->Reset();
		delete fb; fb = 0;

		//Draw();
	}

	void Interface::DispatchCursorMoveEvent (MouseEvent* event)
	{
		AbstractWidget* widget = 0;
		ContextManager* cm = ContextManager::instance;

		// build a stack contians the mouse cursor
		if (cm->m_hover_deque->size()) {

			// search which widget in stack contains the cursor
			while (cm->m_hover_deque->size()) {

				if (cm->m_hover_deque->back()->contain(event->position())) {
					widget = cm->m_hover_deque->back();
					break;
				} else {
					cm->m_hover_deque->back()->CursorEnterEvent(false);
					cm->m_hover_deque->back()->m_flag.reset(AbstractWidget::WidgetFlagContextHoverList);
				}

				cm->m_hover_deque->pop_back();
			}
		}

		BuildWidgetListAtCursorPoint(event->position(), widget);

		for (std::deque<AbstractWidget*>::iterator it =
				cm->m_hover_deque->begin(); it != cm->m_hover_deque->end();
				it++)
		{
			DBG_PRINT_MSG("cursor on: %s", (*it)->name().c_str());
		}


		for (std::deque<AbstractWidget*>::reverse_iterator it =
				cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend();
				it++) {
			(*it)->MouseMoveEvent(event);
		}

		if (event->accepted()) {
			// TODO: do sth
		}
	}

	void Interface::DispatchMousePressEvent(MouseEvent* event)
	{
		ContextManager* cm = ContextManager::instance;

		for(std::deque<AbstractWidget*>::reverse_iterator it = cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend(); it++)
		{
			(*it)->MousePressEvent(event);

			if(event->accepted()) {
				cm->SetFocusedWidget(*it);
				DBG_PRINT_MSG("widget is focused: %s", (*it)->name().c_str());;
				break;
			}
		}
	}

	void Interface::DispatchMouseReleaseEvent(MouseEvent* event)
	{
		ContextManager* cm = ContextManager::instance;

		for(std::deque<AbstractWidget*>::reverse_iterator it = cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend(); it++)
		{
			(*it)->MouseReleaseEvent(event);

			if(event->accepted()) break;
		}
	}

	void Interface::BuildWidgetListAtCursorPoint (const Point& cursor_point,
			AbstractWidget* parent)
	{
		if (parent) {
			parent->m_flag.set(AbstractWidget::WidgetFlagContextHoverList);

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(parent);

			if(p) {
				for (std::deque<AbstractWidget*>::iterator it =
						p->m_sub_widgets.begin(); it != p->m_sub_widgets.end();
						it++) {
					if ((*it)->contain(cursor_point)) {
						ContextManager::instance->m_hover_deque->push_back(*it);
						ContextManager::instance->m_hover_deque->back()->CursorEnterEvent(true);
						BuildWidgetListAtCursorPoint(cursor_point, *it);
						break;	// if break or continue the loop?
					}
				}
			} else {

			}
		} else {
			ContextManager::instance->m_hover_deque->clear();

			map<int, ContextLayer>::reverse_iterator map_it;
			set<AbstractWidget*>::iterator set_it;
			set<AbstractWidget*>* set_p = 0;

			bool stop = false;

			for (map_it = ContextManager::instance->m_layers.rbegin(); map_it != ContextManager::instance->m_layers.rend();
					map_it++) {
				set_p = map_it->second.widgets;
				for (set_it = set_p->begin(); set_it != set_p->end();
						set_it++) {
					if ((*set_it)->contain(cursor_point)) {
						ContextManager::instance->m_hover_deque->push_back(*set_it);
						ContextManager::instance->m_hover_deque->back()->CursorEnterEvent(true);
						BuildWidgetListAtCursorPoint(cursor_point, *set_it);
						stop = true;
					}

					if (stop)
					break;
				}
				if (stop)
				break;
			}
		}
	}

}

