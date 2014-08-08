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

	bool IsContained (AbstractContainer* container, AbstractWidget* widget)
	{
		bool retval = false;

		AbstractContainer* p = widget->container();
		while(p) {
			if(p == container) {
				retval = true;
				break;
			}
			p = p->container();
		}

		return retval;
	}

	float AbstractWidget::default_border_width = 1.f;

	AbstractWidget::AbstractWidget ()
	: Object(),
	  flags_(0),
	  round_radius_(5),
	  container_(0),
	  previous_(0),
	  next_(0)
	{
		events_.reset(new Cpp::ConnectionScope);

		SETBIT(flags_, WidgetFlagVisibility);
	}

	AbstractWidget::~AbstractWidget ()
	{
		if(previous_) {
			assert(container_);
			previous_->next_ = next_;
		} else {
			if(container_) {
				assert(container_->first_ == this);
				container_->first_ = next_;
			}
		}

		if(next_) {
			assert(container_);
			next_->previous_ = previous_;
		} else {
			if(container_) {
				assert(container_->last_ == this);
				container_->last_ = previous_;
			}
		}

		previous_ = 0;
		next_ = 0;
		container_ = 0;

		destroyed_.fire(this);

		DBG_PRINT_MSG("Widget %s destroyed", name_.c_str());
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

		if(container_) {
			if(container_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
				PerformSizeUpdate(request);
				set_size(width, height);
			}
		} else {
			if(SizeUpdateTest(request)) {
				PerformSizeUpdate(request);
				set_size(width, height);
			}
		}
	}

	void AbstractWidget::Resize (const Size& size)
	{
		if(AbstractWidget::size() == size) return;

		SizeUpdateRequest request(this, this, &size);

		if(container_) {
			if(container_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
				PerformSizeUpdate(request);
				set_size(size);
			}
		} else {
			if(SizeUpdateTest(request)) {
				PerformSizeUpdate(request);
				set_size(size);
			}
		}
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		PositionUpdateRequest request(this, this, &new_pos);

		if(container_) {
			if(container_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
				PerformPositionUpdate(request);
				set_position(x, y);
			}
		} else {
			if(PositionUpdateTest(request)) {
				PerformPositionUpdate(request);
				set_position(x, y);
			}
		}
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		if(position() == pos) return;

		PositionUpdateRequest request(this, this, &pos);

		if(container_) {
			if(container_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
				PerformPositionUpdate(request);
				set_position(pos);
			}
		} else {
			if(PositionUpdateTest(request)) {
				PerformPositionUpdate(request);
				set_position(pos);
			}
		}
	}

	void AbstractWidget::SetRoundCornerType(int type)
	{
		if(round_type() == type) return;

		RoundTypeUpdateRequest request(this, this, &type);

		if(container_) {
			if(container_->RoundTypeUpdateTest(request) && RoundTypeUpdateTest(request)) {
				PerformRoundTypeUpdate(request);
				set_round_type(type);
			}
		} else {
			if(RoundTypeUpdateTest(request)) {
				PerformRoundTypeUpdate(request);
				set_round_type(type);
			}
		}
	}

	void AbstractWidget::SetRoundCornerRadius(float radius)
	{
		if(round_radius_ == radius) return;

		RoundRadiusUpdateRequest request(this, this, &radius);

		if(container_) {
			if(container_->RoundRadiusUpdateTest(request) && RoundRadiusUpdateTest(request)) {
				PerformRoundRadiusUpdate(request);
				round_radius_ = radius;
			}
		} else {
			if(RoundRadiusUpdateTest(request)) {
				PerformRoundRadiusUpdate(request);
				round_radius_ = radius;
			}
		}
	}

	void AbstractWidget::SetVisible (bool visible)
	{
		if(this->visiable() == visible)
			return;

		VisibilityUpdateRequest request(this, this, &visible);

		if(container_) {
			if(container_->VisibilityUpdateTest(request) && VisibilityUpdateTest(request)) {
				PerformVisibilityUpdate(request);
				set_visible(visible);
			}
		} else {
			if(VisibilityUpdateTest(request)) {
				PerformVisibilityUpdate(request);
				set_visible(visible);
			}
		}
	}

	bool AbstractWidget::Contain(const Point& point) const
	{
		if(point.x() < position_.x() ||
				point.y() < position_.y() ||
				point.x() > static_cast<int>(position_.x() + size().width()) ||
				point.y() > static_cast<int>(position_.y() + size().height()))
		{
			return false;
		}

		return true;
	}

	void AbstractWidget::Refresh()
	{
		RefreshRequest request (this, container_);

		if(container_) {
			container_->PerformRefresh(request);
		}
	}

	void AbstractWidget::MoveBackward()
	{
		if(container_) {

			if(previous_) {

				AbstractWidget* tmp = previous_;

				tmp->next_ = next_;
				if(next_) {
					next_->previous_ = tmp;
				} else {
					assert(container_->last_ == this);
					container_->last_ = tmp;
				}

				next_ = tmp;
				previous_ = tmp->previous_;
				if(tmp->previous_) {
					tmp->previous_->next_ = this;
				}
				tmp->previous_ = this;

				if(previous_ == 0) {
					assert(container_->first_ == tmp);
					container_->first_ = this;
				}

				DBG_PRINT_MSG("this: %s", name_.c_str());
				if(previous_) {
					DBG_PRINT_MSG("previous: %s", previous_->name_.c_str());
					assert(previous_->next_ == this);
				}
				if(next_) {
					DBG_PRINT_MSG("next: %s", next_->name_.c_str());
					assert(next_->previous_ == this);
				}

			} else {
				assert(container_->first_ == this);
			}

		}
	}

	void AbstractWidget::MoveForward()
	{
		if(container_) {

			if(next_) {

				AbstractWidget* tmp = next_;

				tmp->previous_ = previous_;
				if(previous_) {
					previous_->next_ = tmp;
				} else {
					assert(container_->first_ == this);
					container_->first_ = tmp;
				}

				previous_ = tmp;
				next_ = tmp->next_;
				if(tmp->next_) {
					tmp->next_->previous_ = this;
				}
				tmp->next_ = this;

				if(next_ == 0) {
					assert(container_->last_ == tmp);
					container_->last_ = this;
				}

				if(previous_) {
					assert(previous_->next_ == this);
				}
				if(next_) {
					assert(next_->previous_ == this);
				}

			} else {
				assert(container_->last_ == this);
			}

		}
	}

	void AbstractWidget::MoveToFirst()
	{
		if(container_) {

			if(container_->first_ == this) {
				assert(previous_ == 0);
				return;	// already at first
			}

			previous_->next_ = next_;
			if(next_) {
				next_->previous_ = previous_;
			} else {
				assert(container_->last_ == this);
				container_->last_ = previous_;
			}

			previous_ = 0;
			next_ = container_->first_;
			container_->first_->previous_ = this;
			container_->first_ = this;

		}
	}

	void AbstractWidget::MoveToLast()
	{
		if(container_) {

			if(container_->last_ == this) {
				assert(next_ == 0);
				return;	// already at last
			}

			next_->previous_ = previous_;

			if(previous_) {
				previous_->next_ = next_;
			} else {
				assert(container_->first_ == this);
				container_->first_ = next_;
			}

			next_ = 0;
			previous_ = container_->last_;
			container_->last_->next_ = this;
			container_->last_ = this;

		}
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

			AbstractContainer* container = widget->container_;

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
		if(container_) {
			container_->PerformRefresh(request);
		}
	}

	int AbstractWidget::GetOutlineVertices (int round_type)
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
		if(container_) {
			container_->PerformSizeUpdate(request);
		}
	}

	void AbstractWidget::ReportPositionUpdate(const PositionUpdateRequest& request)
	{
		if(container_) {
			container_->PerformPositionUpdate(request);
		}
	}

	void AbstractWidget::ReportRoundTypeUpdate(const RoundTypeUpdateRequest& request)
	{
		if(container_) {
			container_->PerformRoundTypeUpdate(request);
		}
	}

	void AbstractWidget::ReportRoundRadiusUpdate(const RoundRadiusUpdateRequest& request)
	{
		if(container_) {
			container_->PerformRoundRadiusUpdate(request);
		}
	}

	void AbstractWidget::ReportVisibilityRequest(const VisibilityUpdateRequest& request)
	{

		if(container_) {
			container_->PerformVisibilityUpdate(request);
		}
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
