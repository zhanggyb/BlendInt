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

	inline bool ContainerProxy::RequestSizeUpdateTest(AbstractContainer* container, const SizeUpdateRequest& request)
	{
		if(container) {
			return container->SizeUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool ContainerProxy::RequestPositionUpdateTest(AbstractContainer* container, const PositionUpdateRequest& request)
	{
		if(container) {
			return container->PositionUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool ContainerProxy::RequestRoundTypeUpdateTest(AbstractContainer* container, const RoundTypeUpdateRequest& request)
	{
		if(container) {
			return container->RoundTypeUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool ContainerProxy::RequestRoundRadiusUpdateTest(AbstractContainer* container, const RoundRadiusUpdateRequest& request)
	{
		if(container) {
			return container->RoundRadiusUpdateTest(request);
		} else {
			return true;
		}
	}

	inline bool ContainerProxy::RequestVisibilityUpdateTest(AbstractContainer* container, const VisibilityUpdateRequest& request)
	{
		if(container) {
			return container->VisibilityUpdateTest(request);
		} else {
			return true;
		}
	}

	inline void ContainerProxy::RequestSizeUpdate(AbstractContainer* container, const SizeUpdateRequest& request)
	{
		if(container) {
			container->PerformSizeUpdate(request);
		}
	}

	inline void ContainerProxy::RequestPositionUpdate(AbstractContainer* container, const PositionUpdateRequest& request)
	{
		if(container) {
			container->PerformPositionUpdate(request);
		}
	}

	inline void ContainerProxy::RequestRoundTypeUpdate(AbstractContainer* container, const RoundTypeUpdateRequest& request)
	{
		if(container) {
			container->PerformRoundTypeUpdate(request);
		}
	}

	inline void ContainerProxy::RequestRoundRadiusUpdate(AbstractContainer* container, const RoundRadiusUpdateRequest& request)
	{
		if(container) {
			container->PerformRoundRadiusUpdate(request);
		}
	}

	inline void ContainerProxy::RequestVisibilityUpdate(AbstractContainer* container, const VisibilityUpdateRequest& request)
	{
		if(container) {
			container->PerformVisibilityUpdate(request);
		}
	}

	inline void ContainerProxy::RequestRefresh(AbstractContainer* container, const RefreshRequest& request)
	{
		if(container) {
			container->PerformRefresh(request);
		}
	}

	// --------------------------------------------------------------------

	float AbstractWidget::default_border_width = 1.f;

	AbstractWidget::AbstractWidget ()
	: Object(),
	  m_flags(0),
	  m_round_radius(5),
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

		Size new_size (width, height);
		SizeUpdateRequest request(this, this, &new_size);

		if(ContainerProxy::RequestSizeUpdateTest(m_container, request) && SizeUpdateTest(request)) {
			PerformSizeUpdate(request);
			set_size(width, height);
		}
	}

	void AbstractWidget::Resize (const Size& size)
	{
		if(AbstractWidget::size() == size) return;

		SizeUpdateRequest request(this, this, &size);

		if(ContainerProxy::RequestSizeUpdateTest(m_container, request) && SizeUpdateTest(request)) {
			PerformSizeUpdate(request);
			set_size(size);
		}
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		PositionUpdateRequest request(this, this, &new_pos);

		if(ContainerProxy::RequestPositionUpdateTest(m_container, request) && PositionUpdateTest(request)) {
			PerformPositionUpdate(request);
			set_position(x, y);
		}
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		if(position() == pos) return;

		PositionUpdateRequest request(this, this, &pos);

		if(ContainerProxy::RequestPositionUpdateTest(m_container, request) && PositionUpdateTest(request)) {
			PerformPositionUpdate(request);
			set_position(pos);
		}
	}

	void AbstractWidget::SetRoundCornerType(int type)
	{
		if(round_type() == type) return;

		RoundTypeUpdateRequest request(this, this, &type);

		if(ContainerProxy::RequestRoundTypeUpdateTest(m_container, request) && RoundTypeUpdateTest(request)) {
			PerformRoundTypeUpdate(request);
			set_round_type(type);
		}
	}

	void AbstractWidget::SetRoundCornerRadius(float radius)
	{
		if(m_round_radius == radius) return;

		RoundRadiusUpdateRequest request(this, this, &radius);

		if(ContainerProxy::RequestRoundRadiusUpdateTest(m_container, request) && RoundRadiusUpdateTest(request)) {
			PerformRoundRadiusUpdate(request);
			m_round_radius = radius;
		}
	}

	void AbstractWidget::SetVisible (bool visible)
	{
		if(this->visiable() == visible)
			return;

		VisibilityUpdateRequest request(this, this, &visible);

		if(ContainerProxy::RequestVisibilityUpdateTest(m_container, request) && VisibilityUpdateTest(request)) {
			PerformVisibilityUpdate(request);
			set_visible(visible);
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
		RefreshRequest request (this, m_container);

		ContainerProxy::RequestRefresh(m_container, request);
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

			if(container == 0) return false;	// if a widget hovered was removed from any container.

			while(container) {

				if(!container->Contain(cursor))
					return false;

				container = container->container();
			}

			return true;
		}

		return false;
	}

	void AbstractWidget::ReportRefresh(const RefreshRequest& request)
	{
		ContainerProxy::RequestRefresh(m_container, request);
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

	bool AbstractWidget::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractWidget::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		return true;
	}

	void AbstractWidget::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	void AbstractWidget::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			set_position(*request.position());
		}

		ReportPositionUpdate(request);
	}

	bool AbstractWidget::RoundTypeUpdateTest(const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool AbstractWidget::RoundRadiusUpdateTest(const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	bool AbstractWidget::VisibilityUpdateTest(const VisibilityUpdateRequest& request)
	{
		return true;
	}

	void AbstractWidget::PerformRoundTypeUpdate(const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_round_type(*request.round_type());
		}

		ReportRoundTypeUpdate(request);
	}

	void AbstractWidget::PerformRoundRadiusUpdate(const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			set_round_radius(*request.round_radius());
		}

		ReportRoundRadiusUpdate(request);
	}

	void AbstractWidget::PerformVisibilityUpdate(const VisibilityUpdateRequest& request)
	{
		if(request.target() == this) {
			set_visible(*request.visibility());
		}

		ReportVisibilityRequest(request);
	}

	void AbstractWidget::PerformRefresh(const RefreshRequest& request)
	{
		ReportRefresh(request);
	}

	void AbstractWidget::ReportSizeUpdate(const SizeUpdateRequest& request)
	{
		ContainerProxy::RequestSizeUpdate(m_container, request);
	}

	void AbstractWidget::ReportPositionUpdate(const PositionUpdateRequest& request)
	{
		ContainerProxy::RequestPositionUpdate(m_container, request);
	}

	void AbstractWidget::ReportRoundTypeUpdate(const RoundTypeUpdateRequest& request)
	{
		ContainerProxy::RequestRoundTypeUpdate(m_container, request);
	}

	void AbstractWidget::ReportRoundRadiusUpdate(const RoundRadiusUpdateRequest& request)
	{
		ContainerProxy::RequestRoundRadiusUpdate(m_container, request);
	}

	void AbstractWidget::ReportVisibilityRequest(const VisibilityUpdateRequest& request)
	{
		ContainerProxy::RequestVisibilityUpdate(m_container, request);
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

