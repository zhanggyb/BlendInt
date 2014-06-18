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

	SubWidgetProxy::SubWidgetProxy (AbstractWidget* source, AbstractWidget* target)
	: m_request(source, target)
	{

	}

	SubWidgetProxy::~SubWidgetProxy ()
	{
	}

	void SubWidgetProxy::Resize (AbstractWidget* widget, const Size& size)
	{
		if(widget->size() == size) return;

		m_request.set_type(WidgetSize);
		m_request.set_data(&size);

		if(widget->UpdateGeometryTest(m_request)) {
			widget->UpdateGeometry(m_request);
			widget->set_size(size);
		}
	}

	void SubWidgetProxy::Resize (AbstractWidget* widget, int width, int height)
	{
		if(widget->size().width() == width &&
						widget->size().height() == height)
			return;

		Size new_size (width, height);
		m_request.set_type(WidgetSize);
		m_request.set_data(&new_size);

		if(widget->UpdateGeometryTest(m_request)) {
			widget->UpdateGeometry(m_request);
			widget->set_size(width, height);
		}
	}

	void SubWidgetProxy::SetPosition (AbstractWidget* widget, int x, int y)
	{
		if(widget->position().x() == x &&
						widget->position().y() == y)
			return;

		Point new_pos (x, y);
		m_request.set_type(WidgetPosition);
		m_request.set_data(&new_pos);

		if(widget->UpdateGeometryTest(m_request)) {
			widget->UpdateGeometry(m_request);
			widget->set_position(x, y);
		}
	}

	void SubWidgetProxy::SetPosition (AbstractWidget* widget, const Point& position)
	{
		if(m_request.target()->position() == position) return;
		m_request.set_type(WidgetPosition);
		m_request.set_data(&position);

		if(m_request.target()->UpdateGeometryTest(m_request)) {
			m_request.target()->UpdateGeometry(m_request);
			m_request.target()->set_position(position);
		}
	}

	ContainerProxy::ContainerProxy (AbstractWidget* source, AbstractWidget* target)
	: m_request(source, target)
	{
	}

	ContainerProxy::~ContainerProxy ()
	{
	}

	void ContainerProxy::RequestRefresh (AbstractContainer* container)
	{
		m_request.set_type(ContainerRefresh);
		m_request.set_data(0);

		container->UpdateContainer(m_request);
	}

	bool ContainerProxy::SubwidgetPositionUpdateTest(AbstractContainer* container, const Point& pos)
	{
		m_request.set_type(WidgetPosition);
		m_request.set_data(&pos);

		return container->UpdateGeometryTest(m_request);
	}

	bool ContainerProxy::SubWidgetSizeUpdateTest(AbstractContainer* container, const Size& size)
	{
		m_request.set_type(WidgetSize);
		m_request.set_data(&size);

		return container->UpdateGeometryTest(m_request);
	}

	void ContainerProxy::SubWidgetPositionUpdate (AbstractContainer* container, const Point& pos)
	{
		m_request.set_type(WidgetPosition);
		m_request.set_data(&pos);

		container->UpdateGeometry(m_request);
	}

	void ContainerProxy::SubWidgetSizeUpdate(AbstractContainer* container, const Size& size)
	{
		m_request.set_type(WidgetSize);
		m_request.set_data(&size);

		container->UpdateGeometry(m_request);
	}

	// --------------------------------------------------------------------

	float AbstractWidget::default_border_width = 1.f;

	AbstractWidget::AbstractWidget ()
	: Object(),
	  m_z(0),
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
		WidgetUpdateRequest request(this, this, WidgetSize, &new_size);

		if(ResizeTestInContainer(new_size) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_size(width, height);
			ResizeUpdateInContainer(new_size);
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

		WidgetUpdateRequest request(this, this, WidgetSize, &size);

		if(ResizeTestInContainer(size) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_size(size);
			ResizeUpdateInContainer(size);
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
		WidgetUpdateRequest request(this, this, WidgetPosition, &new_pos);

		if(PositionTestInContainer(new_pos) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_position(x, y);
			PositionUpdateInContainer(new_pos);
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

		WidgetUpdateRequest request(this, this, WidgetPosition, &pos);

		if(PositionTestInContainer(pos) && UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			set_position(pos);
			PositionUpdateInContainer(pos);
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

		WidgetUpdateRequest request(this, this, WidgetRoundCornerType, &type);

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

		WidgetUpdateRequest request(this, this, WidgetRoundCornerRadius, &radius);

		if(UpdateGeometryTest(request)) {
			UpdateGeometry(request);
			m_round_corner_radius = radius;
			broadcast = true;
		}

		if(broadcast) {
			BroadcastUpdate(request);
		}
	}

	void AbstractWidget::SetLayer (int z)
	{
		if(m_z == z) return;
		bool broadcast = false;

		WidgetUpdateRequest request(this, this, WidgetLayer, &z);

		if(UpdateGeometryTest (request)) {
			UpdateGeometry(request);

			Context* context = GetContext();
			if(context) {
				context->Remove(this);
				m_z = z;
				context->Add(this);
			} else {
				m_z = z;
			}

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

		WidgetUpdateRequest request(this, this, WidgetVisibility, &visible);

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

	bool AbstractWidget::Contain(int x, int y) const
	{
		if(x < m_position.x() ||
				y < m_position.y() ||
				x > static_cast<int>(m_position.x() + size().width()) ||
				y > static_cast<int>(m_position.y() + size().height()))
		{
			return false;
		}

		return true;
	}

	void AbstractWidget::Refresh()
	{
		if(m_container) {
			ContainerProxy proxy(this, m_container);
			proxy.RequestRefresh(m_container);
		}
	}

	void AbstractWidget::RenderToTexture (size_t border, GLTexture2D* texture)
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

			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;
			event.set_projection_matrix(projection);
			event.set_view_matrix(view * offset);

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}

		fb->Reset();

		tex->Reset();
		delete tex; tex = 0;

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

			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;
			event.set_projection_matrix(projection);
			event.set_view_matrix(view * offset);

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

	void AbstractWidget::DrawTriangleFan(const GLint attrib, const GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);

		buffer->Reset();
	}

	void AbstractWidget::DrawShadedTriangleFan(const GLint coord, const GLint color, GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(coord, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(0)					 // offset of first element
							  );

		glVertexAttribPointer(color, // attribute
							  4,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(2 * sizeof(GLfloat))					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_corner_type()) + 2);

		buffer->Reset();
	}

	void AbstractWidget::DrawTriangleStrip(const GLint attrib, GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(round_corner_type()) * 2 + 2);

		buffer->Reset();
	}

	void AbstractWidget::SetEmboss(bool emboss)
	{
		if(this->emboss() == emboss)
			return;

		set_emboss(emboss);
		Refresh();
	}

	void AbstractWidget::DispatchRender(AbstractWidget* other)
	{
		//other->Draw();
	}

	ResponseType AbstractWidget::dispatch_key_press_event (AbstractWidget* obj,
			const KeyEvent& event)
	{
		return obj->KeyPressEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_move_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MouseMoveEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_press_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MousePressEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_release_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MouseReleaseEvent(event);
	}

	bool AbstractWidget::ResizeTestInContainer (const Size& size)
	{
		if(m_container) {
			ContainerProxy test(this, this);

			return test.SubWidgetSizeUpdateTest(m_container, size);
		} else {
			return true;
		}
	}

	bool AbstractWidget::PositionTestInContainer(const Point& point)
	{
		if(m_container) {

			ContainerProxy test(this, this);
			return test.SubwidgetPositionUpdateTest(m_container, point);

		} else {
			return true;
		}
	}

	void AbstractWidget::ResizeUpdateInContainer(const Size& size)
	{
		if(m_container) {
			ContainerProxy proxy(this, this);
			proxy.SubWidgetSizeUpdate(m_container, size);
		}
	}

	void AbstractWidget::PositionUpdateInContainer(const Point& pos)
	{
		if(m_container) {
			ContainerProxy proxy(this, this);
			proxy.SubWidgetPositionUpdate(m_container, pos);
		}
	}

	Context* AbstractWidget::GetContext()
	{
		AbstractContainer* container = m_container;

		if(container == 0) {
			return dynamic_cast<Context*>(this);
		} else {

			while(container->container()) {
				container = container->container();
			}

		}

		return dynamic_cast<Context*>(container);
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
