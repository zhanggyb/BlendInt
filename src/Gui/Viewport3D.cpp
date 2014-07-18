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

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Interface.hpp>

#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	Viewport3D::Viewport3D ()
	: AbstractWidget(),
	  m_last_x(0),
	  m_last_y(0),
	  m_rX(0.0),
	  m_rY(0.0),
	  m_button_down(MouseButtonNone)
	{
		set_size(600, 500);
		set_drop_shadow(true);

		InitOnce();
	}

	Viewport3D::~Viewport3D ()
	{
		m_cameras.clear();
	}

	ResponseType Viewport3D::CursorEnterEvent (bool entered)
	{
		Context* context = Context::GetContext(this);
		if(!context) return Ignore;

		if(entered) {
			context->PushCursor(context->GetCursor());
			context->SetCursor(CrossCursor);
		} else {
			context->SetCursor(context->PopCursor());
		}

		return Accept;
	}

	ResponseType Viewport3D::KeyPressEvent (const KeyEvent& event)
	{
		if (event.action() == KeyPress) {
			switch (event.key()) {
				case Key_KP_Decimal: {
					// setup camera
					glm::vec3 pos = glm::vec3(5.f);
					glm::vec3 center = glm::vec3(0);
					glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
					m_default_camera->LookAt(pos, center, up);
					m_default_camera->SetPerspective(m_default_camera->fovy(),
					        1.0f * size().width() / size().height());
					Refresh();
					break;
				}

				default:
					break;
			}
		} else if (event.action() == KeyRelease
		        && m_button_down == MouseButtonMiddle) {

			// currently does nothing
			switch (event.key()) {
				case Key_LeftShift:
				case Key_RightShift:
					break;

				case Key_LeftAlt:
				case Key_RightAlt:
					break;

				default:
					break;
			}

		}

		return Accept;
	}

	ResponseType Viewport3D::MousePressEvent (const MouseEvent& event)
	{
		m_button_down = event.button();

		m_last_x = event.position().x();
		m_last_y = event.position().y();

		if (m_button_down == MouseButtonMiddle) {

			if (event.modifiers() == ModifierNone) {

				m_default_camera->SaveCurrentPosition();
				m_default_camera->SaveCurrentCenter();

			} else if (event.modifiers() == ModifierShift) {

				m_default_camera->SaveCurrentPosition();
				m_default_camera->SaveCurrentCenter();

			} else if (event.modifiers() == ModifierControl) {

				m_default_camera->SaveCurrentPosition();

			}

		} else if (m_button_down == MouseButtonScrollUp) {

			std::cout << "scroll up" << std::endl;

			m_default_camera->SaveCurrentPosition();

			m_default_camera->Zoom(5.f);

		} else if (m_button_down == MouseButtonScrollDown) {

			std::cout << "scroll down" << std::endl;

			m_default_camera->SaveCurrentPosition();

			m_default_camera->Zoom(-5.f);

		}

		return Accept;
	}

	ResponseType Viewport3D::MouseReleaseEvent (const MouseEvent& event)
	{
		m_button_down = MouseButtonNone;

		return Accept;
	}

	ResponseType Viewport3D::MouseMoveEvent (const MouseEvent& event)
	{
		switch (m_button_down) {
			case MouseButtonLeft: {
				break;
			}

			case MouseButtonMiddle: {

				if (event.modifiers() == ModifierShift) {

					float dx = static_cast<float>(m_last_x
					        - event.position().x());
					float dy = static_cast<float>(m_last_y
					        - event.position().y());
					m_default_camera->Pan(dx, dy);

				} else if (event.modifiers() == ModifierControl) {

					m_default_camera->Zoom(m_last_y - event.position().y());

				} else if (event.modifiers() == ModifierNone) {
					float dx = static_cast<float>(m_last_x
					        - event.position().x());
					float dy = static_cast<float>(m_last_y
					        - event.position().y());
					m_default_camera->Orbit(dx, dy);
				}

				Refresh();

				break;
			}

			case MouseButtonRight: {
				break;
			}

			default:
				break;
		}

		return Accept;
	}

	void Viewport3D::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			m_default_camera->SetPerspective(m_default_camera->fovy(),
			        1.f * request.size()->width() / request.size()->height());

			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	void Viewport3D::Render ()
	{
		/* Clear the buffer, clear the matrix */
		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_grid->Render(m_default_camera->projection(), m_default_camera->view());

		for(std::deque<RefPtr<AbstractPrimitive> >::iterator it = m_primitives.begin(); it != m_primitives.end(); it++)
		{
			(*it)->Render(m_default_camera->projection(), m_default_camera->view());
		}
	}

	ResponseType Viewport3D::Draw (const RedrawEvent& event)
	{
        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_SCISSOR_TEST);
        glScissor(position().x(), position().y(), size().width(),
                size().height());

        glViewport(position().x(), position().y(), size().width(),
                size().height());
        // --------------------------------------------------------------------------------
        Render();
        // --------------------------------------------------------------------------------
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);

        glViewport(vp[0], vp[1], vp[2], vp[3]);

		return Accept;
	}
	
	Size Viewport3D::GetPreferredSize () const
	{
		return Size(2560, 2560);
	}

	bool Viewport3D::IsExpandX() const
	{
		return true;
	}

	bool Viewport3D::IsExpandY() const
	{
		return true;
	}

	ResponseType Viewport3D::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Viewport3D::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport3D::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	void Viewport3D::PushBack (const RefPtr<AbstractPrimitive>& primitive)
	{
		m_primitives.push_back(primitive);
	}

	void Viewport3D::InitOnce ()
	{
		m_default_camera.reset(new NavigationCamera);

		// setup camera
		glm::vec3 pos = glm::vec3(5.f);
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
		m_default_camera->LookAt(pos, center, up);

		m_default_camera->SetPerspective(m_default_camera->fovy(),
		        1.f * size().width() / size().height());
		m_default_camera->Update();

		m_grid.reset(new Grid);
	}

}
