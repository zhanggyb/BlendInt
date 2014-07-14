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

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Interface.hpp>

namespace BlendInt {

	ContainerProxy::ContainerProxy ()
	{
	}

	ContainerProxy::~ContainerProxy ()
	{
	}

	inline bool ContainerProxy::RequestGeometryTest(AbstractContainer* container, const GeometryUpdateRequest& request)
	{
		if(container) {
			return container->UpdateGeometryTest(request);
		} else {
			return true;
		}
	}

	inline void ContainerProxy::RequestGeometryUpdate(AbstractContainer* container, const GeometryUpdateRequest& request)
	{
		if(container) {
			container->UpdateGeometry(request);
		}
	}

	inline void ContainerProxy::RequestContainerUpdate(AbstractContainer* container, const ContainerUpdateRequest& request)
	{
		if(container) {
			container->UpdateContainer(request);
		}
	}

	// --------------------------------------------------------------------

	float AbstractWidget::default_border_width = 1.f;

	AbstractWidget::AbstractWidget ()
	: Object(),
	  m_flags(0),
	  m_round_corner_radius(5),
	  m_container(0)
	{
		m_events.reset(new Cpp::ConnectionScope);

		SETBIT(m_flags, WidgetFlagVisibility);
	}

	AbstractWidget::~AbstractWidget ()
	{
		m_destroyed.fire(this);
	}

	Size AbstractWidget::GetPreferredSize() const
	{
		return Size(200, 200);
	}

	void AbstractWidget::Resize (int width, int height)
	{
		if(size().width() == width && size().height() == height) return;

		bool broadcast = false;

		Size new_size (width, height);
		GeometryUpdateRequest request(this, this, WidgetSize, &new_size);

		if(ContainerProxy::RequestGeometryTest(m_container, request) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_size(width, height);
			//ContainerProxy::RequestGeometryUpdate(m_container, request);

			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::Resize (const Size& size)
	{
		if(AbstractWidget::size() == size) return;
		bool broadcast = false;

		GeometryUpdateRequest request(this, this, WidgetSize, &size);

		if(ContainerProxy::RequestGeometryTest(m_container, request) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_size(size);
			//ContainerProxy::RequestGeometryUpdate(m_container, request);

			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;
		bool broadcast = false;

		Point new_pos (x, y);
		GeometryUpdateRequest request(this, this, WidgetPosition, &new_pos);

		if(ContainerProxy::RequestGeometryTest(m_container, request) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_position(x, y);
			//ContainerProxy::RequestGeometryUpdate(m_container, request);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		if(position() == pos) return;
		bool broadcast = false;

		GeometryUpdateRequest request(this, this, WidgetPosition, &pos);

		if(ContainerProxy::RequestGeometryTest(m_container, request) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_position(pos);
			//ContainerProxy::RequestGeometryUpdate(m_container, request);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetRoundCornerType(int type)
	{
		if(round_corner_type() == type) return;
		bool broadcast = false;

		GeometryUpdateRequest request(this, this, WidgetRoundCornerType, &type);

		if(UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_round_corner_type(type);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetRoundCornerRadius(float radius)
	{
		if(m_round_corner_radius == radius) return;
		bool broadcast = false;

		GeometryUpdateRequest request(this, this, WidgetRoundCornerRadius, &radius);

		if(UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			m_round_corner_radius = radius;
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetVisible (bool visible)
	{
		if(this->visiable() == visible)
			return;

		bool broadcast = false;

		GeometryUpdateRequest request(this, this, WidgetVisibility, &visible);

		if(UpdateGeometryTest (request)) {
			UpdateGeometry(request);
			set_visible(visible);
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	bool AbstractWidget::Contain(const Point& point) const
	{
		if(point.x() < m_position.x() ||
				point.y() < m_position.y() ||
				point.x() > static_cast<int>(m_position.x() + size().width()) ||
				point.y() > static_cast<int>(m_position.y() + size().height()))
		{
			return false;
		}

		return true;
	}

	void AbstractWidget::Refresh()
	{
		ContainerUpdateRequest request (this, m_container);
		request.set_type(ContainerRefresh);
		request.set_data(0);

		ContainerProxy::RequestContainerUpdate(m_container, request);
	}

	void AbstractWidget::RenderToTexture (int border, GLTexture2D* texture)
	{
		if(!texture) return;

		GLsizei width = size().width() + border * 2;
		GLsizei height = size().height() + border * 2;

		// Create and set texture to render to.
		GLTexture2D* tex = texture;
		if(!tex->texture())
			tex->Generate();

		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->texture(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				width, height);

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		if(GLFramebuffer::CheckStatus()) {

			fb->Bind();

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_BLEND);

			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}

		fb->Reset();
		tex->Reset();

		//delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;
	}

	void AbstractWidget::RenderToFile(const char* filename, unsigned int border)
	{
		GLsizei width = size().width() + border * 2;
		GLsizei height = size().height() + border * 2;

		// Create and set texture to render to.
		GLTexture2D* tex = new GLTexture2D;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->texture(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				width, height);

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		if(GLFramebuffer::CheckStatus()) {

			fb->Bind();

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_BLEND);

			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);

			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(vp[0], vp[1], vp[2], vp[3]);

			// ---------------------------------------------
			tex->WriteToFile(filename);
		}

		fb->Reset();

		tex->Reset();
		delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;
	}

	void AbstractWidget::SetDefaultBorderWidth(float border)
	{	
		default_border_width = border;
	}

	float AbstractWidget::DefaultBorderWidth()
	{	
		return default_border_width;
	}

	void AbstractWidget::SetEmboss(bool emboss)
	{
		if(this->emboss() == emboss)
			return;

		set_emboss(emboss);
		Refresh();
	}

	bool AbstractWidget::IsHoverThrough(const AbstractWidget* widget, const Point& cursor)
	{
		if(widget->Contain(cursor)) {

			AbstractContainer* container = widget->m_container;

			while(container) {

				if(!container->Contain(cursor))
					return false;

				container = container->container();
			}

			return true;
		}

		return false;
	}

	void AbstractWidget::CheckSubWidgetAddedInContainer (AbstractWidget* sub_widget)
	{
		ContainerUpdateRequest request (this, m_container);
		request.set_type(ContainerSubWidgetAdded);
		request.set_data(sub_widget);

		ContainerProxy::RequestContainerUpdate(m_container, request);
	}

	void AbstractWidget::CheckSubWidgetRemovedInContainer (AbstractWidget* sub_widget)
	{
		ContainerUpdateRequest request (this, m_container);
		request.set_type(ContainerSubWidgetRemoved);
		request.set_data(sub_widget);

		ContainerProxy::RequestContainerUpdate(m_container, request);
	}

	bool AbstractWidget::QueryGeometryUpdateTest(const GeometryUpdateRequest& request)
	{
		return ContainerProxy::RequestGeometryTest(m_container, request);
	}

	void AbstractWidget::ReportContainerUpdate(const ContainerUpdateRequest& request)
	{
		ContainerProxy::RequestContainerUpdate(m_container, request);
	}

	void AbstractWidget::ReportGeometryUpdate(const GeometryUpdateRequest& request)
	{
		ContainerProxy::RequestGeometryUpdate(m_container, request);
	}

	int AbstractWidget::GetOutlineVertices (int round_type) const
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 4 - count + count * WIDGET_CURVE_RESOLU;
	}

	int AbstractWidget::GetHalfOutlineVertices(int round_type) const
	{
		round_type = round_type & (RoundBottomLeft | RoundBottomRight);
		int count = 0;
		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 2 - count + count * WIDGET_CURVE_RESOLU;
	}

} /* namespace BlendInt */

