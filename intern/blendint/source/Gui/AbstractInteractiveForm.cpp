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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/AbstractInteractiveForm.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	bool IsContained (AbstractInteractiveForm* container, AbstractInteractiveForm* widget)
	{
		bool retval = false;

		AbstractInteractiveForm* p = widget->parent();
		while(p) {
			if(p == container) {
				retval = true;
				break;
			}
			p = p->parent();
		}

		return retval;
	}

	float AbstractInteractiveForm::border_width = 1.f;

	pthread_mutex_t AbstractInteractiveForm::refresh_mutex = PTHREAD_MUTEX_INITIALIZER;

	const float AbstractInteractiveForm::cornervec[WIDGET_CURVE_RESOLU][2] = {
        { 0.0, 0.0 },
        { 0.195, 0.02 },
        { 0.383, 0.067 },
        { 0.55,	0.169 },
        { 0.707, 0.293 },
        { 0.831, 0.45 },
        { 0.924, 0.617 },
        { 0.98, 0.805 },
        { 1.0, 1.0 }
    };

	AbstractInteractiveForm::AbstractInteractiveForm ()
	: Object(),
	  flags_(0),
	  subs_count_(0),
	  parent_(0),
	  previous_(0),
	  next_(0),
	  first_child_(0),
	  last_child_(0)
	{
		set_visible(true);
		//set_refresh(true);
	}

	AbstractInteractiveForm::~AbstractInteractiveForm ()
	{
		ClearSubForms();

		if(parent_) {

			if(previous_) {
				previous_->next_ = next_;
			} else {
				assert(parent_->first_child_ == this);
				parent_->first_child_ = next_;
			}

			if(next_) {
				next_->previous_ = previous_;
			} else {
				assert(parent_->last_child_ == this);
				parent_->last_child_ = previous_;
			}

			parent_->subs_count_--;
			assert(parent_->subs_count_ >= 0);

			previous_ = 0;
			next_ = 0;
			parent_ = 0;

		} else {
			assert(previous_ == 0);
			assert(next_ == 0);
		}
	}

	Point AbstractInteractiveForm::GetGlobalPosition () const
	{
		Point retval = position_;;

		AbstractInteractiveForm* p = parent_;
		while(p) {
			retval.reset(
					retval.x() + p->position().x() + p->offset().x(),
					retval.y() + p->position().y() + p->offset().y());
			p = p->parent_;
		}

		return retval;
	}

	Size AbstractInteractiveForm::GetPreferredSize() const
	{
		return Size(200, 200);
	}

	void AbstractInteractiveForm::Resize (int width, int height)
	{
		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);
		SizeUpdateRequest request(this, this, &new_size);

		if(parent_) {
			if(parent_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
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

	void AbstractInteractiveForm::Resize (const Size& size)
	{
		if(AbstractInteractiveForm::size() == size) return;

		SizeUpdateRequest request(this, this, &size);

		if(parent_) {
			if(parent_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
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

	void AbstractInteractiveForm::MoveTo (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		PositionUpdateRequest request(this, this, &new_pos);

		if(parent_) {
			if(parent_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
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

	void AbstractInteractiveForm::MoveTo (const Point& pos)
	{
		if(position() == pos) return;

		PositionUpdateRequest request(this, this, &pos);

		if(parent_) {
			if(parent_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
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

	void AbstractInteractiveForm::SetRoundType(int type)
	{
		if(round_type() == type) return;

		PerformRoundTypeUpdate(type);
	}

	void AbstractInteractiveForm::SetVisible (bool visible)
	{
		if(this->visiable() == visible)
			return;

		VisibilityUpdateRequest request(this, this, &visible);

		if(parent_) {
			if(parent_->VisibilityUpdateTest(request) && VisibilityUpdateTest(request)) {
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

	bool AbstractInteractiveForm::Contain(const Point& point) const
	{
		if(point.x() < position_.x() ||
				point.y() < position_.y() ||
				point.x() > (position_.x() + size_.width()) ||
				point.y() > (position_.y() + size_.height()))
		{
			return false;
		}

		return true;
	}

	void AbstractInteractiveForm::RequestRedraw()
	{
		if(pthread_mutex_lock(&refresh_mutex) != 0) {
			DBG_PRINT_MSG("%s", "Fail to lock mutex");
		}

		if(!refresh()) {

			AbstractInteractiveForm* root = this;
			AbstractInteractiveForm* p = parent();

			/*
			while(p) {
				DBG_PRINT_MSG("parent name: %s, refresh flag: %s", p->name().c_str(), p->refresh() ? "True":"False");
				p = p->parent();
			}
			p = parent();
			*/

			while(p && (!p->refresh()) && (p->visiable())) {
				root = p;
				p->set_refresh(true);
				p = p->parent();
			}

			if(root->parent() == 0) {
				Context* context = dynamic_cast<Context*>(root);
				if(context) {
					context->SynchronizeWindow();
				}
			}

			set_refresh(true);
		}

		if(pthread_mutex_unlock(&refresh_mutex) != 0) {
			DBG_PRINT_MSG("%s", "Fail to unlock mutex");
		}
	}

	bool AbstractInteractiveForm::TryRequestRedraw()
	{
		if(pthread_mutex_trylock(&refresh_mutex) == 0) {

			if(!refresh()) {

				AbstractInteractiveForm* root = this;
				AbstractInteractiveForm* p = parent();

				/*
				while(p) {
					DBG_PRINT_MSG("parent name: %s, refresh flag: %s", p->name().c_str(), p->refresh() ? "True":"False");
					p = p->parent();
				}
				p = parent();
				*/

				while(p && (!p->refresh()) && (p->visiable())) {
					root = p;
					p->set_refresh(true);
					p = p->parent();
				}

				if(root->parent() == 0) {
					Context* context = dynamic_cast<Context*>(root);
					if(context) {
						context->SynchronizeWindow();
					}
				}

				set_refresh(true);
			}

			// TODO: mutex unlock
			pthread_mutex_unlock(&refresh_mutex);

			return true;
		} else {
			DBG_PRINT_MSG("%s", "cannot lock mutex");
		}

		return false;
	}

	void AbstractInteractiveForm::MoveBackward()
	{
		if(parent_) {

			if(previous_) {

				AbstractInteractiveForm* tmp = previous_;

				tmp->next_ = next_;
				if(next_) {
					next_->previous_ = tmp;
				} else {
					assert(parent_->last_child_ == this);
					parent_->last_child_ = tmp;
				}

				next_ = tmp;
				previous_ = tmp->previous_;
				if(tmp->previous_) {
					tmp->previous_->next_ = this;
				}
				tmp->previous_ = this;

				if(previous_ == 0) {
					assert(parent_->first_child_ == tmp);
					parent_->first_child_ = this;
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
				assert(parent_->first_child_ == this);
			}

		}
	}

	void AbstractInteractiveForm::MoveForward()
	{
		if(parent_) {

			if(next_) {

				AbstractInteractiveForm* tmp = next_;

				tmp->previous_ = previous_;
				if(previous_) {
					previous_->next_ = tmp;
				} else {
					assert(parent_->first_child_ == this);
					parent_->first_child_ = tmp;
				}

				previous_ = tmp;
				next_ = tmp->next_;
				if(tmp->next_) {
					tmp->next_->previous_ = this;
				}
				tmp->next_ = this;

				if(next_ == 0) {
					assert(parent_->last_child_ == tmp);
					parent_->last_child_ = this;
				}

				if(previous_) {
					assert(previous_->next_ == this);
				}
				if(next_) {
					assert(next_->previous_ == this);
				}

			} else {
				assert(parent_->last_child_ == this);
			}

		}
	}

	void AbstractInteractiveForm::MoveToFirst()
	{
		if(parent_) {

			if(parent_->first_child_ == this) {
				assert(previous_ == 0);
				return;	// already at first
			}

			previous_->next_ = next_;
			if(next_) {
				next_->previous_ = previous_;
			} else {
				assert(parent_->last_child_ == this);
				parent_->last_child_ = previous_;
			}

			previous_ = 0;
			next_ = parent_->first_child_;
			parent_->first_child_->previous_ = this;
			parent_->first_child_ = this;

		}
	}

	void AbstractInteractiveForm::MoveToLast()
	{
		if(parent_) {

			if(parent_->last_child_ == this) {
				assert(next_ == 0);
				return;	// already at last
			}

			next_->previous_ = previous_;

			if(previous_) {
				previous_->next_ = next_;
			} else {
				assert(parent_->first_child_ == this);
				parent_->first_child_ = next_;
			}

			next_ = 0;
			previous_ = parent_->last_child_;
			parent_->last_child_->next_ = this;
			parent_->last_child_ = this;

		}
	}

	void AbstractInteractiveForm::SetDefaultBorderWidth(float border)
	{
		border_width = border;
	}

	void AbstractInteractiveForm::SetEmboss(bool emboss)
	{
		if(this->emboss() == emboss)
			return;

		set_emboss(emboss);
		RequestRedraw();
	}

	bool AbstractInteractiveForm::IsHoverThrough(const AbstractInteractiveForm* widget, const Point& cursor)
	{
		AbstractInteractiveForm* container = widget->parent_;
		if(container == 0) return false;	// if a widget hovered was removed from any container.

		if(widget->visiable() && widget->Contain(cursor)) {

			while(container) {
				if((!container->visiable()) || (!container->Contain(cursor)))
					return false;

				container = container->parent();
			}

			return true;

		}

		return false;
	}

	bool AbstractInteractiveForm::IsHoverThroughExt (const AbstractInteractiveForm* widget, const Point& global_cursor_position)
	{
		Point global_position = widget->GetGlobalPosition();

		if(global_cursor_position.x() < global_position.x() ||
				global_cursor_position.y() < global_position.y() ||
				global_cursor_position.x() > (global_position.x() + widget->size().width()) ||
				global_cursor_position.y() > (global_position.y() + widget->size().height()))
		{
			return false;
		}

		return true;
	}

	int AbstractInteractiveForm::GetOutlineVertices (int round_type)
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 4 - count + count * WIDGET_CURVE_RESOLU;
	}

	void AbstractInteractiveForm::DispatchDrawEvent (AbstractInteractiveForm* widget,
	        Profile& profile)
	{
#ifdef DEBUG
		assert(widget != 0);
#endif

		if (widget->PreDraw(profile)) {

			ResponseType response = widget->Draw(profile);
			widget->set_refresh(widget->parent_->refresh());

			if(response == Ignore) {
				for(AbstractInteractiveForm* sub = widget->first_child(); sub; sub = sub->next())
				{
					DispatchDrawEvent(sub, profile);
				}
			}

			widget->PostDraw(profile);
		}
	}

	bool AbstractInteractiveForm::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}	}

	bool AbstractInteractiveForm::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source()->parent() == this) {
			return false;
		} else {
			return true;
		}	}

	void AbstractInteractiveForm::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void AbstractInteractiveForm::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			set_position(*request.position());
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	bool AbstractInteractiveForm::VisibilityUpdateTest(const VisibilityUpdateRequest& request)
	{
		return true;
	}

	void AbstractInteractiveForm::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);
	}

	void AbstractInteractiveForm::PerformVisibilityUpdate(const VisibilityUpdateRequest& request)
	{
		if(request.target() == this) {
			set_visible(*request.visibility());
		}

		if(request.source() == this) {
			ReportVisibilityRequest(request);
		}
	}

	void AbstractInteractiveForm::ReportSizeUpdate(const SizeUpdateRequest& request)
	{
		if(parent_) {
			parent_->PerformSizeUpdate(request);
		}
	}

	void AbstractInteractiveForm::ReportPositionUpdate(const PositionUpdateRequest& request)
	{
		if(parent_) {
			parent_->PerformPositionUpdate(request);
		}
	}

	void AbstractInteractiveForm::ReportVisibilityRequest(const VisibilityUpdateRequest& request)
	{

		if(parent_) {
			parent_->PerformVisibilityUpdate(request);
		}
	}

	int AbstractInteractiveForm::GetHalfOutlineVertices(int round_type) const
	{
		round_type = round_type & (RoundBottomLeft | RoundBottomRight);
		int count = 0;
		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 2 - count + count * WIDGET_CURVE_RESOLU;
	}

	/*

	void AbstractInteractiveForm::GenerateVertices(std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		float border = default_border_width * Theme::instance->pixel();

		float rad = round_radius_ * Theme::instance->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size_.width();
		float maxy = size_.height();

		float minxi = minx + border;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border; 	// U.pixelsize;
		float minyi = miny + border;		// U.pixelsize;
		float maxyi = maxy - border;		// U.pixelsize;

		int minsize = 0;
		int corner = round_type();
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type();

		minsize = std::min(size_.width() * hnum, size_.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			((*inner_ptr))[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;
			(*inner_ptr)[2] = 0.f;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}

			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			(*inner_ptr)[count * 3] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = 0.f;
		}

		if(outer) {

			if(border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = minx + vec[i][1];
						edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
					}
				} else {
					edge_vertices[count * 2] = minx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// corner right-bottom
				if (corner & RoundBottomRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = maxx - rad + vec[i][0];
						edge_vertices[count * 2 + 1] = miny + vec[i][1];
					}
				} else {
					edge_vertices[count * 2] = maxx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// m_half = count;

				// corner right-top
				if (corner & RoundTopRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = maxx - vec[i][1];
						edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
					}
				} else {
					edge_vertices[count * 2] = maxx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				// corner left-top
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}
	}

	void AbstractInteractiveForm::GenerateVertices(Orientation shadedir, short shadetop,
			short shadedown, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		float border = default_border_width * Theme::instance->pixel();

		float rad = round_radius_ * Theme::instance->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size_.width();
		float maxy = size_.height();

		float minxi = minx + border;
		float maxxi = maxx - border;
		float minyi = miny + border;
		float maxyi = maxy - border;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int corner = round_type();
		int minsize = 0;
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		float offset = 0.f;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type();

		minsize = std::min(size_.width() * hnum, size_.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			(*inner_ptr)[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				offset = make_shaded_offset(shadetop, shadedown,
								facyi * ((*inner_ptr)[1] - minyi));
			} else {
				offset = make_shaded_offset(shadetop, shadedown,
								facxi * ((*inner_ptr)[0] - minxi));
			}
			(*inner_ptr)[2] = offset;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}

					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {

				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			(*inner_ptr)[count * 3 + 0] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = (*inner_ptr)[3 + 2];

		}

		if(outer) {

			if (border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = minx + vec[i][1];
						edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
					}
				} else {
					edge_vertices[count * 2 + 0] = minx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// corner right-bottom
				if (corner & RoundBottomRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = maxx - rad + vec[i][0];
						edge_vertices[count * 2 + 1] = miny + vec[i][1];
					}
				} else {
					edge_vertices[count * 2 + 0] = maxx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// m_half = count;

				// corner right-top
				if (corner & RoundTopRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = maxx - vec[i][1];
						edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
					}
				} else {
					edge_vertices[count * 2 + 0] = maxx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				// corner left-top
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2 + 0] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}

	}

	*/

	void AbstractInteractiveForm::GenerateTriangleStripVertices(
			const std::vector<GLfloat>* inner,
			const std::vector<GLfloat>* edge,
			unsigned int num,
			std::vector<GLfloat>* strip)
	{
		if (num > edge->size() / 2) {
			DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld",
					num, edge->size() / 2);
			return;
		}

		if (strip->size() != (num * 2 + 2) * 2) {
			strip->resize((num * 2 + 2) * 2);
		}

		size_t count = 0;
		for (int i = 0, j = 0; count < num * 2; count++) {
			if (count % 2 == 0) {
				(*strip)[count * 2] = (*inner)[3 + i];
				(*strip)[count * 2 + 1] = (*inner)[3 + i + 1];
				i += 3;
			} else {
				(*strip)[count * 2] = (*edge)[j];
				(*strip)[count * 2 + 1] = (*edge)[j + 1];
				j += 2;
			}
		}

		(*strip)[count * 2] = (*inner)[3 + 0];
		(*strip)[count * 2 + 1] = (*inner)[3 + 1];
		(*strip)[count * 2 + 2] = (*edge)[0];
		(*strip)[count * 2 + 3] = (*edge)[1];
	}

	void AbstractInteractiveForm::GenerateVertices(const Size& size, float border,
			int round_type, float radius, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		border *= Theme::instance->pixel();

		float rad = radius * Theme::instance->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border;		// U.pixelsize; // boundbox inner
		float maxxi = maxx - border; 	// U.pixelsize;
		float minyi = miny + border;		// U.pixelsize;
		float maxyi = maxy - border;		// U.pixelsize;

		int minsize = 0;
		int corner = round_type;
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight | RoundBottomLeft)) == (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border;	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices
			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			((*inner_ptr))[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;
			(*inner_ptr)[2] = 0.f;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}
			} else {
				(*inner_ptr)[count * 3] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];
					(*inner_ptr)[count * 3 + 2] = 0.f;
				}

			} else {
				(*inner_ptr)[count * 3] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;
				(*inner_ptr)[count * 3 + 2] = 0.f;
				count++;
			}

			(*inner_ptr)[count * 3] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = 0.f;
		}

		if(outer) {

			if(border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = minx + vec[i][1];
						edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
					}
				} else {
					edge_vertices[count * 2] = minx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// corner right-bottom
				if (corner & RoundBottomRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = maxx - rad + vec[i][0];
						edge_vertices[count * 2 + 1] = miny + vec[i][1];
					}
				} else {
					edge_vertices[count * 2] = maxx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// m_half = count;

				// corner right-top
				if (corner & RoundTopRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = maxx - vec[i][1];
						edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
					}
				} else {
					edge_vertices[count * 2] = maxx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				// corner left-top
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}
	}

	void AbstractInteractiveForm::GenerateVertices(const Size& size, float border,
			int round_type, float radius, Orientation shadedir, short shadetop,
			short shadedown, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == 0 && outer == 0) return;

		std::vector<GLfloat>* inner_ptr = 0;

		if(inner == 0) {
			inner_ptr = new std::vector<GLfloat>;
		} else {
			inner_ptr = inner;
		}

		border *= Theme::instance->pixel();

		float rad = radius * Theme::instance->pixel();
		float radi = rad - border;

		float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

		float minx = 0.0;
		float miny = 0.0;
		float maxx = size.width();
		float maxy = size.height();

		float minxi = minx + border;
		float maxxi = maxx - border;
		float minyi = miny + border;
		float maxyi = maxy - border;

		float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
		float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

		int corner = round_type;
		int minsize = 0;
		const int hnum = (
                (corner & (RoundTopLeft | RoundTopRight)) == (RoundTopLeft | RoundTopRight)
                ||
                (corner & (RoundBottomRight	| RoundBottomLeft))	== (RoundBottomRight | RoundBottomLeft)
                ) ? 1 : 2;
		const int vnum = (
                (corner & (RoundTopLeft | RoundBottomLeft)) == (RoundTopLeft | RoundBottomLeft)
                ||
                (corner & (RoundTopRight | RoundBottomRight)) == (RoundTopRight | RoundBottomRight)
                ) ? 1 : 2;

		float offset = 0.f;

		int count = 0;
		while (corner != 0) {
			count += corner & 0x1;
			corner = corner >> 1;
		}
		unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
		corner = round_type;

		minsize = std::min(size.width() * hnum, size.height() * vnum);

		if (2.0f * rad > minsize)
			rad = 0.5f * minsize;

		if (2.0f * (radi + border) > minsize)
			radi = 0.5f * minsize - border * Theme::instance->pixel();	// U.pixelsize;

		// mult
		for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			veci[i][0] = radi * cornervec[i][0];
			veci[i][1] = radi * cornervec[i][1];
			vec[i][0] = rad * cornervec[i][0];
			vec[i][1] = rad * cornervec[i][1];
		}

		{	// generate inner vertices

			if(inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
				inner_ptr->resize((outline_vertex_number + 2) * 3);
			}

			// inner_ptr[0, 0] is the center of a triangle fan
			(*inner_ptr)[0] = minxi + (maxxi - minxi) / 2.f;
			(*inner_ptr)[1] = minyi + (maxyi - minyi) / 2.f;

			if (shadedir == Vertical) {
				offset = make_shaded_offset(shadetop, shadedown,
								facyi * ((*inner_ptr)[1] - minyi));
			} else {
				offset = make_shaded_offset(shadetop, shadedown,
								facxi * ((*inner_ptr)[0] - minxi));
			}
			(*inner_ptr)[2] = offset;

			count = 1;

			// corner left-bottom
			if (corner & RoundBottomLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + veci[i][1];
					(*inner_ptr)[count * 3 + 1] = minyi + radi - veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}

					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-bottom
			if (corner & RoundBottomRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - radi + veci[i][0];
					(*inner_ptr)[count * 3 + 1] = minyi + veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = minyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner right-top
			if (corner & RoundTopRight) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = maxxi - veci[i][1];
					(*inner_ptr)[count * 3 + 1] = maxyi - radi + veci[i][0];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {
				(*inner_ptr)[count * 3 + 0] = maxxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			// corner left-top
			if (corner & RoundTopLeft) {
				for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
					(*inner_ptr)[count * 3 + 0] = minxi + radi - veci[i][0];
					(*inner_ptr)[count * 3 + 1] = maxyi - veci[i][1];

					if (shadedir == Vertical) {
						offset = make_shaded_offset(shadetop, shadedown,
										facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
					} else {
						offset = make_shaded_offset(shadetop, shadedown,
										facxi * ((*inner_ptr)[count * 3 + 0] - minxi));
					}
					(*inner_ptr)[count * 3 + 2] = offset;
				}
			} else {

				(*inner_ptr)[count * 3 + 0] = minxi;
				(*inner_ptr)[count * 3 + 1] = maxyi;

				if (shadedir == Vertical) {
					offset = make_shaded_offset(shadetop, shadedown, 1.0f);
				} else {
					offset = make_shaded_offset(shadetop, shadedown, 0.0f);
				}
				(*inner_ptr)[count * 3 + 2] = offset;

				count++;
			}

			(*inner_ptr)[count * 3 + 0] = (*inner_ptr)[3 + 0];
			(*inner_ptr)[count * 3 + 1] = (*inner_ptr)[3 + 1];
			(*inner_ptr)[count * 3 + 2] = (*inner_ptr)[3 + 2];

		}

		if(outer) {

			if (border > 0.f) {

				std::vector<GLfloat> edge_vertices(outline_vertex_number * 2);

				count = 0;

				// corner left-bottom
				if (corner & RoundBottomLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = minx + vec[i][1];
						edge_vertices[count * 2 + 1] = miny + rad - vec[i][0];
					}
				} else {
					edge_vertices[count * 2 + 0] = minx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// corner right-bottom
				if (corner & RoundBottomRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = maxx - rad + vec[i][0];
						edge_vertices[count * 2 + 1] = miny + vec[i][1];
					}
				} else {
					edge_vertices[count * 2 + 0] = maxx;
					edge_vertices[count * 2 + 1] = miny;
					count++;
				}

				// m_half = count;

				// corner right-top
				if (corner & RoundTopRight) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = maxx - vec[i][1];
						edge_vertices[count * 2 + 1] = maxy - rad + vec[i][0];
					}
				} else {
					edge_vertices[count * 2 + 0] = maxx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				// corner left-top
				if (corner & RoundTopLeft) {
					for (int i = 0; i < WIDGET_CURVE_RESOLU; i++, count++) {
						edge_vertices[count * 2 + 0] = minx + rad - vec[i][0];
						edge_vertices[count * 2 + 1] = maxy - vec[i][1];
					}
				} else {
					edge_vertices[count * 2 + 0] = minx;
					edge_vertices[count * 2 + 1] = maxy;
					count++;
				}

				GenerateTriangleStripVertices(inner_ptr, &edge_vertices, count, outer);

			} else {

				outer->clear();

			}

		}

		if(inner == 0) {
			delete inner_ptr;
		}

	}

	AbstractInteractiveForm* AbstractInteractiveForm::operator [](int i) const
	{
		if((i < 0) || (i >= subs_count_)) return 0;

		AbstractInteractiveForm* widget = 0;

		if(i < ((subs_count_ + 1)/ 2)) {

			widget = first_child_;
			while(i > 0) {
				widget = widget->next_;
				i--;
			}

		} else {

			widget = last_child_;
			int max = subs_count_ - 1;
			while(i < max) {
				widget = widget->previous_;
				i++;
			}

		}

		//assert(widget != 0);

		return widget;
	}

	AbstractInteractiveForm* AbstractInteractiveForm::GetWidgetAt(int i) const
	{
		if((i < 0) || (i >= subs_count_)) return 0;

		AbstractInteractiveForm* widget = 0;

		if(i < ((subs_count_ + 1)/ 2)) {

			widget = first_child_;
			while(i > 0) {
				widget = widget->next_;
				i--;
			}

		} else {

			widget = last_child_;
			int max = subs_count_ - 1;
			while(i < max) {
				widget = widget->previous_;
				i++;
			}

		}

		//assert(widget != 0);

		return widget;
	}

	bool AbstractInteractiveForm::PushFrontSubForm(AbstractInteractiveForm* widget)
	{
		if (!widget)
			return false;

		if (widget->parent_) {

			if (widget->parent_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->parent_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->parent_->RemoveSubForm(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->parent_ == 0);

		if(first_child_) {
			first_child_->previous_ = widget;
			widget->next_ = first_child_;
		} else {
			assert(last_child_ == 0);
			widget->next_ = 0;
			last_child_ = widget;
		}
		first_child_ = widget;

		widget->previous_ = 0;
		widget->parent_ = this;
		subs_count_++;

		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractInteractiveForm::InsertSubForm(int index, AbstractInteractiveForm* widget)
	{
		if (!widget)
			return false;

		if (widget->parent_) {

			if (widget->parent_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->parent_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->parent_->RemoveSubForm(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->parent_ == 0);

		if(first_child_ == 0) {
			assert(last_child_ == 0);

			widget->next_ = 0;
			last_child_ = widget;
			first_child_ = widget;
			widget->previous_ = 0;

		} else {

			AbstractInteractiveForm* p = first_child_;

			if(index > 0) {

				while(p && (index > 0)) {
					if(p->next_ == 0)
						break;

					p = p->next_;
					index--;
				}

				if(index == 0) {	// insert

					widget->previous_ = p->previous_;
					widget->next_ = p;
					p->previous_->next_ = widget;
					p->previous_ = widget;

				} else {	// same as push back

					assert(p == last_child_);
					last_child_->next_ = widget;
					widget->previous_ = last_child_;
					last_child_ = widget;
					widget->next_ = 0;

				}

			} else {	// same as push front

				first_child_->previous_ = widget;
				widget->next_ = first_child_;
				first_child_ = widget;
				widget->previous_ = 0;

			}

		}

		widget->parent_ = this;
		subs_count_++;
		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractInteractiveForm::PushBackSubForm(AbstractInteractiveForm* widget)
	{
		if (!widget)
			return false;

		if (widget->parent_) {

			if (widget->parent_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								widget->name_.c_str(),
								widget->parent_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				widget->parent_->RemoveSubForm(widget);
			}

		}

		assert(widget->previous_ == 0);
		assert(widget->next_ == 0);
		assert(widget->parent_ == 0);

		if(last_child_) {
			last_child_->next_ = widget;
			widget->previous_ = last_child_;
		} else {
			assert(first_child_ == 0);
			widget->previous_ = 0;
			first_child_ = widget;
		}
		last_child_ = widget;

		widget->next_ = 0;
		widget->parent_ = this;
		subs_count_++;

		//events()->connect(widget->destroyed(), this,
		//				&AbstractContainer::OnSubWidgetDestroyed);

		return true;
	}

	bool AbstractInteractiveForm::RemoveSubForm(AbstractInteractiveForm* widget)
	{
		if (!widget)
			return false;

		assert(widget->parent_ == this);

		//widget->destroyed().disconnectOne(this,
		//        &AbstractContainer::OnSubWidgetDestroyed);

		if (widget->previous_) {
			widget->previous_->next_ = widget->next_;
		} else {
			assert(first_child_ == widget);
			first_child_ = widget->next_;
		}

		if (widget->next_) {
			widget->next_->previous_ = widget->previous_;
		} else {
			assert(last_child_ == widget);
			last_child_ = widget->previous_;
		}

		widget->previous_ = 0;
		widget->next_ = 0;
		widget->parent_ = 0;
		subs_count_--;

		if(widget->hover()) {
			widget->set_hover(false);
		}

		return true;
	}

	void AbstractInteractiveForm::ClearSubForms()
	{
		AbstractInteractiveForm* widget = first_child_;
		AbstractInteractiveForm* next = 0;

		while(widget) {

			next = widget->next_;

			widget->previous_ = 0;
			widget->next_ = 0;
			widget->parent_ = 0;

			if(widget->managed() && widget->reference_count() == 0) {
				delete widget;
			} else {
				DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", widget->name_.c_str());
			}

			widget = next;
		}

		subs_count_ = 0;
		first_child_ = 0;
		last_child_ = 0;
	}

	void AbstractInteractiveForm::ResizeSubForm(AbstractInteractiveForm* sub, int width,
			int height)
	{
		if(!sub || sub->parent() != this) return;

		if(sub->size().width() == width &&
				sub->size().height() == height)
			return;

		Size new_size (width, height);
		SizeUpdateRequest request(this, sub, &new_size);

		if(sub->SizeUpdateTest(request)) {
			sub->PerformSizeUpdate(request);
			sub->set_size(width, height);
		}
	}

	void AbstractInteractiveForm::ResizeSubForm(AbstractInteractiveForm* sub, const Size& size)
	{
		if (!sub || sub->parent() != this)
			return;

		if (sub->size() == size)
			return;

		SizeUpdateRequest request(this, sub, &size);

		if(sub->SizeUpdateTest(request)) {
			sub->PerformSizeUpdate(request);
			sub->set_size(size);
		}
	}

	void AbstractInteractiveForm::MoveSubFormTo(AbstractInteractiveForm* sub, int x, int y)
	{
		if (!sub || sub->parent() != this)
			return;

		if (sub->position().x() == x && sub->position().y() == y)
			return;

		Point new_pos(x, y);

		PositionUpdateRequest request(this, sub, &new_pos);

		if(sub->PositionUpdateTest(request)) {
			sub->PerformPositionUpdate(request);
			sub->set_position(x, y);
		}
	}

	void AbstractInteractiveForm::MoveSubFormTo(AbstractInteractiveForm* sub,
			const Point& pos)
	{
		if(!sub || sub->parent() != this) return;

		if(sub->position() == pos) return;

		PositionUpdateRequest request (this, sub, &pos);

		if(sub->PositionUpdateTest(request)) {
			sub->PerformPositionUpdate(request);
			sub->set_position(pos);
		}
	}

	void AbstractInteractiveForm::SetSubFormVisibility(AbstractInteractiveForm* sub,
			bool visible)
	{
		if(!sub || sub->parent() != this) return;

		if(sub->visiable() == visible) return;

		VisibilityUpdateRequest request (this, sub, &visible);

		if(sub->VisibilityUpdateTest(request)) {
			sub->PerformVisibilityUpdate(request);
			sub->set_visible(visible);
		}
	}

	void AbstractInteractiveForm::MoveSubWidgets(int move_x, int move_y)
	{
		for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
			MoveSubFormTo(p, p->position().x() + move_x,
			        p->position().y() + move_y);
		}
	}

	void AbstractInteractiveForm::ResizeSubWidgets(const Size& size)
	{
		for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
			ResizeSubForm(p, size);
		}
	}

	void AbstractInteractiveForm::ResizeSubWidgets(int w, int h)
	{
		for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
			ResizeSubForm(p, w, h);
		}
	}

	void AbstractInteractiveForm::FillSingleWidget(int index, const Size& size,
			const Margin& margin)
	{
		int x = margin.left();
		int y = margin.bottom();

		int w = size.width() - margin.hsum();
		int h = size.height() - margin.vsum();

		FillSingleWidget(index, x, y, w, h);
	}

	void AbstractInteractiveForm::FillSingleWidget(int index, const Point& pos,
			const Size& size)
	{
		FillSingleWidget(index, pos.x(), pos.y(), size.width(), size.height());
	}

	void AbstractInteractiveForm::FillSingleWidget(int index, int left, int bottom,
			int width, int height)
	{
		AbstractInteractiveForm* widget = GetWidgetAt(index);

		if (widget) {
			ResizeSubForm(widget, width, height);
			MoveSubFormTo(widget, left, bottom);

			if (widget->size().width() < width) {
				MoveSubFormTo(widget,
				        left + (width - widget->size().width()) / 2, bottom);
			}

			if (widget->size().height() < height) {
				MoveSubFormTo(widget, left,
				        bottom + (height - widget->size().height() / 2));
			}
		}
	}

	void AbstractInteractiveForm::FillSubWidgetsAveragely(const Point& out_pos,
			const Size& out_size, const Margin& margin, Orientation orientation,
			int alignment, int space)
	{
		if(first_child_ == 0) return;

		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int width = out_size.width() - margin.left() - margin.right();
		int height = out_size.height() - margin.top() - margin.bottom();

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	void AbstractInteractiveForm::FillSubWidgetsAveragely(const Point& pos,
			const Size& size, Orientation orientation, int alignment, int space)
	{
		if(first_child_ == 0) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(pos.x(), size.width(), space);
			AlignHorizontally(pos.y(), size.height(), alignment);
		} else {
			DistributeVertically(pos.y(), size.height(), space);
			AlignVertically(pos.x(), size.width(), alignment);
		}
	}

	void AbstractInteractiveForm::FillSubWidgetsAveragely(int x, int y, int width,
			int height, Orientation orientation, int alignment, int space)
	{
		if(first_child_ == 0) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	float AbstractInteractiveForm::make_shaded_offset (short shadetop, short shadedown, float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

	void AbstractInteractiveForm::DistributeHorizontally(int x, int width, int space)
	{
		int sum = subs_count();

		if (sum) {
			int average_width = (width - (sum - 1)* space) / sum;

			if (average_width > 0) {

				for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
					ResizeSubForm(p, average_width, p->size().height());
					MoveSubFormTo(p, x, p->position().y());
					x += average_width + space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractInteractiveForm::DistributeVertically(int y, int height, int space)
	{
		int sum = subs_count();

		y = y + height;
		if (sum) {
			int average_height = (height - (sum - 1)* space) / sum;

			if (average_height > 0) {

				for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
					ResizeSubForm(p, p->size().width(), average_height);
					y -= average_height;
					MoveSubFormTo(p, p->position().x(), y);
					y -= space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractInteractiveForm::AlignHorizontally(int y, int height, int alignment)
	{
		for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
			if(p->IsExpandY()) {
				ResizeSubForm(p, p->size().width(), height);
				MoveSubFormTo(p, p->position().x(), y);
			} else {

				if (alignment & AlignTop) {
					MoveSubFormTo(p, p->position().x(),
					        y + (height - p->size().height()));
				} else if (alignment & AlignBottom) {
					MoveSubFormTo(p, p->position().x(), y);
				} else if (alignment & AlignHorizontalCenter) {
					MoveSubFormTo(p, p->position().x(),
					        y + (height - p->size().height()) / 2);
				}

			}
		}
	}

	void AbstractInteractiveForm::AlignVertically(int x, int width, int alignment)
	{
		for (AbstractInteractiveForm* p = first_child_; p; p = p->next_) {
			if (p->IsExpandX()) {
				ResizeSubForm(p, width, p->size().height());
				MoveSubFormTo(p, x, p->position().y());
			} else {

				if (alignment & AlignLeft) {
					MoveSubFormTo(p, x, p->position().y());
				} else if (alignment & AlignRight) {
					MoveSubFormTo(p, x + (width - p->size().width()), p->position().y());
				} else if (alignment & AlignVerticalCenter) {
					MoveSubFormTo(p, x + (width - p->size().width()) / 2, p->position().y());
				}

			}
		}
	}

} /* namespace BlendInt */
