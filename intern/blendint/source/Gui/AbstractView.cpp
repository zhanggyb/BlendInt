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

#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	bool IsContained (AbstractView* container, AbstractView* widget)
	{
		bool retval = false;

		AbstractView* p = widget->superview();
		while(p) {
			if(p == container) {
				retval = true;
				break;
			}
			p = p->superview();
		}

		return retval;
	}

	float AbstractView::kBorderWidth = 1.f;

	const float AbstractView::cornervec[WIDGET_CURVE_RESOLU][2] = {
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

	AbstractView::AbstractView ()
	: Object(),
	  flags_(0),
	  subs_count_(0),
	  superview_(0),
	  previous_view_(0),
	  next_view_(0),
	  first_subview_(0),
	  last_subview_(0)
	{
		set_visible(true);
		//set_refresh(true);
	}

	AbstractView::~AbstractView ()
	{
		ClearSubViews();

		if(superview_) {

			superview_->RemoveSubView(this);

			/*
			if(previous_view_) {
				previous_view_->next_view_ = next_view_;
			} else {
				assert(superview_->first_subview_ == this);
				superview_->first_subview_ = next_view_;
			}

			if(next_view_) {
				next_view_->previous_view_ = previous_view_;
			} else {
				assert(superview_->last_subview_ == this);
				superview_->last_subview_ = previous_view_;
			}

			superview_->subs_count_--;
			assert(superview_->subs_count_ >= 0);

			previous_view_ = 0;
			next_view_ = 0;
			superview_ = 0;
			*/

		} else {
			assert(previous_view_ == 0);
			assert(next_view_ == 0);
		}
	}

	Point AbstractView::GetGlobalPosition () const
	{
		Point retval = position_;;

		AbstractView* p = superview_;
		while(p) {
			retval.reset(
					retval.x() + p->position().x() + p->offset().x(),
					retval.y() + p->position().y() + p->offset().y());
			p = p->superview_;
		}

		return retval;
	}

	Size AbstractView::GetPreferredSize() const
	{
		return Size(200, 200);
	}

	void AbstractView::Resize (int width, int height)
	{
		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);
		SizeUpdateRequest request(this, this, &new_size);

		if(superview_) {
			if(superview_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
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

	void AbstractView::Resize (const Size& size)
	{
		if(AbstractView::size() == size) return;

		SizeUpdateRequest request(this, this, &size);

		if(superview_) {
			if(superview_->SizeUpdateTest(request) && SizeUpdateTest(request)) {
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

	void AbstractView::MoveTo (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		PositionUpdateRequest request(this, this, &new_pos);

		if(superview_) {
			if(superview_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
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

	void AbstractView::MoveTo (const Point& pos)
	{
		if(position() == pos) return;

		PositionUpdateRequest request(this, this, &pos);

		if(superview_) {
			if(superview_->PositionUpdateTest(request) && PositionUpdateTest(request)) {
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

	void AbstractView::SetRoundType(int type)
	{
		if(round_type() == type) return;

		PerformRoundTypeUpdate(type);
	}

	void AbstractView::SetVisible (bool visible)
	{
		if(this->visiable() == visible)
			return;

		VisibilityUpdateRequest request(this, this, &visible);

		if(superview_) {
			if(superview_->VisibilityUpdateTest(request) && VisibilityUpdateTest(request)) {
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

	bool AbstractView::Contain(const Point& point) const
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

	void AbstractView::RequestRedraw()
	{
		if(!refresh()) {

			AbstractView* root = this;
			AbstractView* p = superview();

			/*
			while(p) {
				DBG_PRINT_MSG("superview name: %s, refresh flag: %s", p->name().c_str(), p->refresh() ? "True":"False");
				p = p->superview();
			}
			p = superview();
			*/

			while(p && (!p->refresh()) && (p->visiable())) {
				root = p;
				p->set_refresh(true);
				p = p->superview();
			}

			if(root->superview() == 0) {
				Context* context = dynamic_cast<Context*>(root);
				if(context) {
					//DBG_PRINT_MSG("Call %s", "virtual Context::SynchronizeWindow()");
					context->SynchronizeWindow();
				}
			}

			set_refresh(true);
		}
	}

	void AbstractView::SetDefaultBorderWidth(float border)
	{
		kBorderWidth = border;
	}

	void AbstractView::SetEmboss(bool emboss)
	{
		if(this->emboss() == emboss)
			return;

		set_emboss(emboss);
		RequestRedraw();
	}

	void AbstractView::MoveToFirst(AbstractView* view)
	{
		if(view->superview_) {

			if(view->superview_->first_subview_ == view) {
				assert(view->previous_view_ == 0);
				return;	// already at first
			}

			view->previous_view_->next_view_ = view->next_view_;
			if(view->next_view_) {
				view->next_view_->previous_view_ = view->previous_view_;
			} else {
				assert(view->superview_->last_subview_ == view);
				view->superview_->last_subview_ = view->previous_view_;
			}

			view->previous_view_ = 0;
			view->next_view_ = view->superview_->first_subview_;
			view->superview_->first_subview_->previous_view_ = view;
			view->superview_->first_subview_ = view;

			view->superview_->RequestRedraw();
		}
	}

	void AbstractView::MoveToLast(AbstractView* view)
	{
		if(view->superview_) {

			if(view->superview_->last_subview_ == view) {
				assert(view->next_view_ == 0);
				return;	// already at last
			}

			view->next_view_->previous_view_ = view->previous_view_;

			if(view->previous_view_) {
				view->previous_view_->next_view_ = view->next_view_;
			} else {
				assert(view->superview_->first_subview_ == view);
				view->superview_->first_subview_ = view->next_view_;
			}

			view->next_view_ = 0;
			view->previous_view_ = view->superview_->last_subview_;
			view->superview_->last_subview_->next_view_ = view;
			view->superview_->last_subview_ = view;

			view->superview_->RequestRedraw();
		}
	}

	void AbstractView::MoveForward(AbstractView* view)
	{
		if(view->superview_) {

			if(view->next_view_) {

				AbstractView* tmp = view->next_view_;

				tmp->previous_view_ = view->previous_view_;
				if(view->previous_view_) {
					view->previous_view_->next_view_ = tmp;
				} else {
					assert(view->superview_->first_subview_ == view);
					view->superview_->first_subview_ = tmp;
				}

				view->previous_view_ = tmp;
				view->next_view_ = tmp->next_view_;
				if(tmp->next_view_) {
					tmp->next_view_->previous_view_ = view;
				}
				tmp->next_view_ = view;

				if(view->next_view_ == 0) {
					assert(view->superview_->last_subview_ == tmp);
					view->superview_->last_subview_ = view;
				}

				if(view->previous_view_) {
					assert(view->previous_view_->next_view_ == view);
				}
				if(view->next_view_) {
					assert(view->next_view_->previous_view_ == view);
				}

				view->superview_->RequestRedraw();

			} else {
				assert(view->superview_->last_subview_ == view);
			}

		}
	}

	void AbstractView::MoveBackward(AbstractView* view)
	{
		if(view->superview_) {

			if(view->previous_view_) {

				AbstractView* tmp = view->previous_view_;

				tmp->next_view_ = view->next_view_;
				if(view->next_view_) {
					view->next_view_->previous_view_ = tmp;
				} else {
					assert(view->superview_->last_subview_ == view);
					view->superview_->last_subview_ = tmp;
				}

				view->next_view_ = tmp;
				view->previous_view_ = tmp->previous_view_;
				if(tmp->previous_view_) {
					tmp->previous_view_->next_view_ = view;
				}
				tmp->previous_view_ = view;

				if(view->previous_view_ == 0) {
					assert(view->superview_->first_subview_ == tmp);
					view->superview_->first_subview_ = view;
				}

				DBG_PRINT_MSG("this: %s", view->name_.c_str());
				if(view->previous_view_) {
					DBG_PRINT_MSG("previous_view: %s", view->previous_view_->name_.c_str());
					assert(view->previous_view_->next_view_ == view);
				}
				if(view->next_view_) {
					DBG_PRINT_MSG("next_view: %s", view->next_view_->name_.c_str());
					assert(view->next_view_->previous_view_ == view);
				}

				view->superview_->RequestRedraw();

			} else {
				assert(view->superview_->first_subview_ == view);
			}

		}
	}

	int AbstractView::GetOutlineVertices (int round_type)
	{
		round_type = round_type & RoundAll;
		int count = 0;

		while (round_type != 0) {
			count += round_type & 0x1;
			round_type = round_type >> 1;
		}

		return 4 - count + count * WIDGET_CURVE_RESOLU;
	}

	void AbstractView::DrawSubViewsOnce(const Context* context)
	{
		bool refresh_record = false;

		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			set_refresh(false);	// allow pass to superview in RequestRedraw()
			if (p->PreDraw(context)) {

				ResponseType response = p->Draw(context);
				p->set_refresh(refresh());

				if(response == Ignore) {
					for(AbstractView* sub = p->first_subview(); sub; sub = sub->next_view())
					{
						DispatchDrawEvent(sub, context);
					}
				}

				p->PostDraw(context);
			}

			if(refresh()) refresh_record = true;
		}

		set_refresh(refresh_record);
	}

	void AbstractView::DispatchDrawEvent (AbstractView* widget,
	        const Context* context)
	{
#ifdef DEBUG
		assert(widget != 0);
#endif

		if (widget->PreDraw(context)) {

			ResponseType response = widget->Draw(context);
			widget->set_refresh(widget->superview_->refresh());

			if(response == Ignore) {
				for(AbstractView* sub = widget->first_subview(); sub; sub = sub->next_view())
				{
					DispatchDrawEvent(sub, context);
				}
			}

			widget->PostDraw(context);
		}
	}

	bool AbstractView::SizeUpdateTest(const SizeUpdateRequest& request)
	{
		if(request.source()->superview() == this) {
			return false;
		} else {
			return true;
		}	}

	bool AbstractView::PositionUpdateTest(const PositionUpdateRequest& request)
	{
		if(request.source()->superview() == this) {
			return false;
		} else {
			return true;
		}	}

	void AbstractView::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			set_size(*request.size());
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void AbstractView::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			set_position(*request.position());
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	bool AbstractView::VisibilityUpdateTest(const VisibilityUpdateRequest& request)
	{
		return true;
	}

	void AbstractView::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);
	}

	void AbstractView::PerformVisibilityUpdate(const VisibilityUpdateRequest& request)
	{
		if(request.target() == this) {
			set_visible(*request.visibility());
		}

		if(request.source() == this) {
			ReportVisibilityRequest(request);
		}
	}

	void AbstractView::ReportSizeUpdate(const SizeUpdateRequest& request)
	{
		if(superview_) {
			superview_->PerformSizeUpdate(request);
		}
	}

	void AbstractView::ReportPositionUpdate(const PositionUpdateRequest& request)
	{
		if(superview_) {
			superview_->PerformPositionUpdate(request);
		}
	}

	void AbstractView::ReportVisibilityRequest(const VisibilityUpdateRequest& request)
	{

		if(superview_) {
			superview_->PerformVisibilityUpdate(request);
		}
	}

	int AbstractView::GetHalfOutlineVertices(int round_type) const
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

	void AbstractView::GenerateVertices(std::vector<GLfloat>* inner,
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

	void AbstractView::GenerateVertices(Orientation shadedir, short shadetop,
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

	void AbstractView::GenerateTriangleStripVertices(
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

	void AbstractView::GenerateVertices(const Size& size, float border,
			int round_type, float radius, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == nullptr && outer == nullptr) return;

		std::vector<GLfloat>* inner_ptr = nullptr;

		if(inner == nullptr) {
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

		if(outer != nullptr) {

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

		if(inner == nullptr) {
			delete inner_ptr;
		}
	}

	void AbstractView::GenerateVertices(const Size& size, float border,
			int round_type, float radius, Orientation shadedir, short shadetop,
			short shadedown, std::vector<GLfloat>* inner,
			std::vector<GLfloat>* outer)
	{
		if(inner == nullptr && outer == nullptr) return;

		std::vector<GLfloat>* inner_ptr = nullptr;

		if(inner == nullptr) {
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

		if(outer != nullptr) {

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

		if(inner == nullptr) {
			delete inner_ptr;
		}

	}

	AbstractView* AbstractView::operator [](int i) const
	{
		if((i < 0) || (i >= subs_count_)) return 0;

		AbstractView* widget = 0;

		if(i < ((subs_count_ + 1)/ 2)) {

			widget = first_subview_;
			while(i > 0) {
				widget = widget->next_view_;
				i--;
			}

		} else {

			widget = last_subview_;
			int max = subs_count_ - 1;
			while(i < max) {
				widget = widget->previous_view_;
				i++;
			}

		}

		//assert(widget != 0);

		return widget;
	}

	AbstractView* AbstractView::GetWidgetAt(int i) const
	{
		if((i < 0) || (i >= subs_count_)) return 0;

		AbstractView* widget = 0;

		if(i < ((subs_count_ + 1)/ 2)) {

			widget = first_subview_;
			while(i > 0) {
				widget = widget->next_view_;
				i--;
			}

		} else {

			widget = last_subview_;
			int max = subs_count_ - 1;
			while(i < max) {
				widget = widget->previous_view_;
				i++;
			}

		}

		//assert(widget != 0);

		return widget;
	}

	bool AbstractView::PushFrontSubView(AbstractView* view)
	{
		if (!view)
			return false;

		if (view->superview_) {

			if (view->superview_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								view->name_.c_str(),
								view->superview_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				view->superview_->RemoveSubView(view);
			}

		}

		assert(view->previous_view_ == 0);
		assert(view->next_view_ == 0);
		assert(view->superview_ == 0);

		if(first_subview_) {
			first_subview_->previous_view_ = view;
			view->next_view_ = first_subview_;
		} else {
			assert(last_subview_ == 0);
			view->next_view_ = 0;
			last_subview_ = view;
		}
		first_subview_ = view;

		view->previous_view_ = 0;
		view->superview_ = this;
		subs_count_++;

		return true;
	}

	bool AbstractView::InsertSubView(int index, AbstractView* view)
	{
		if (!view)
			return false;

		if (view->superview_) {

			if (view->superview_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								view->name_.c_str(),
								view->superview_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				view->superview_->RemoveSubView(view);
			}

		}

		assert(view->previous_view_ == 0);
		assert(view->next_view_ == 0);
		assert(view->superview_ == 0);

		if(first_subview_ == 0) {
			assert(last_subview_ == 0);

			view->next_view_ = 0;
			last_subview_ = view;
			first_subview_ = view;
			view->previous_view_ = 0;

		} else {

			AbstractView* p = first_subview_;

			if(index > 0) {

				while(p && (index > 0)) {
					if(p->next_view_ == 0)
						break;

					p = p->next_view_;
					index--;
				}

				if(index == 0) {	// insert

					view->previous_view_ = p->previous_view_;
					view->next_view_ = p;
					p->previous_view_->next_view_ = view;
					p->previous_view_ = view;

				} else {	// same as push back

					assert(p == last_subview_);
					last_subview_->next_view_ = view;
					view->previous_view_ = last_subview_;
					last_subview_ = view;
					view->next_view_ = 0;

				}

			} else {	// same as push front

				first_subview_->previous_view_ = view;
				view->next_view_ = first_subview_;
				first_subview_ = view;
				view->previous_view_ = 0;

			}

		}

		view->superview_ = this;
		subs_count_++;

		return true;
	}

	bool AbstractView::PushBackSubView(AbstractView* view)
	{
		if (!view)
			return false;

		if (view->superview_) {

			if (view->superview_ == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
								view->name_.c_str(),
								view->superview_->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				view->superview_->RemoveSubView(view);
			}

		}

		assert(view->previous_view_ == 0);
		assert(view->next_view_ == 0);
		assert(view->superview_ == 0);

		if(last_subview_) {
			last_subview_->next_view_ = view;
			view->previous_view_ = last_subview_;
		} else {
			assert(first_subview_ == 0);
			view->previous_view_ = 0;
			first_subview_ = view;
		}
		last_subview_ = view;

		view->next_view_ = 0;
		view->superview_ = this;
		subs_count_++;

		return true;
	}

	bool AbstractView::RemoveSubView(AbstractView* view)
	{
		if (!view)
			return false;

		assert(view->superview_ == this);

		if (view->previous_view_) {
			view->previous_view_->next_view_ = view->next_view_;
		} else {
			assert(first_subview_ == view);
			first_subview_ = view->next_view_;
		}

		if (view->next_view_) {
			view->next_view_->previous_view_ = view->previous_view_;
		} else {
			assert(last_subview_ == view);
			last_subview_ = view->previous_view_;
		}

		subs_count_--;
		assert(subs_count_ >= 0);

		view->previous_view_ = 0;
		view->next_view_ = 0;
		view->superview_ = 0;

		return true;
	}

	void AbstractView::ClearSubViews()
	{
		AbstractView* widget = first_subview_;
		AbstractView* next_view = 0;

		while(widget) {

			next_view = widget->next_view_;

			widget->previous_view_ = 0;
			widget->next_view_ = 0;
			widget->superview_ = 0;

			if(widget->managed() && widget->reference_count() == 0) {
				delete widget;
			} else {
				DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", widget->name_.c_str());
			}

			widget = next_view;
		}

		subs_count_ = 0;
		first_subview_ = 0;
		last_subview_ = 0;
	}

	void AbstractView::ResizeSubView(AbstractView* sub, int width,
			int height)
	{
		if(!sub || sub->superview() != this) return;

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

	void AbstractView::ResizeSubView(AbstractView* sub, const Size& size)
	{
		if (!sub || sub->superview() != this)
			return;

		if (sub->size() == size)
			return;

		SizeUpdateRequest request(this, sub, &size);

		if(sub->SizeUpdateTest(request)) {
			sub->PerformSizeUpdate(request);
			sub->set_size(size);
		}
	}

	void AbstractView::MoveSubViewTo(AbstractView* sub, int x, int y)
	{
		if (!sub || sub->superview() != this)
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

	void AbstractView::MoveSubViewTo(AbstractView* sub,
			const Point& pos)
	{
		if(!sub || sub->superview() != this) return;

		if(sub->position() == pos) return;

		PositionUpdateRequest request (this, sub, &pos);

		if(sub->PositionUpdateTest(request)) {
			sub->PerformPositionUpdate(request);
			sub->set_position(pos);
		}
	}

	void AbstractView::SetSubViewVisibility(AbstractView* sub,
			bool visible)
	{
		if(!sub || sub->superview() != this) return;

		if(sub->visiable() == visible) return;

		VisibilityUpdateRequest request (this, sub, &visible);

		if(sub->VisibilityUpdateTest(request)) {
			sub->PerformVisibilityUpdate(request);
			sub->set_visible(visible);
		}
	}

	void AbstractView::MoveSubWidgets(int move_x, int move_y)
	{
		for (AbstractView* p = first_subview_; p; p = p->next_view_) {
			MoveSubViewTo(p, p->position().x() + move_x,
			        p->position().y() + move_y);
		}
	}

	void AbstractView::ResizeSubWidgets(const Size& size)
	{
		for (AbstractView* p = first_subview_; p; p = p->next_view_) {
			ResizeSubView(p, size);
		}
	}

	void AbstractView::ResizeSubWidgets(int w, int h)
	{
		for (AbstractView* p = first_subview_; p; p = p->next_view_) {
			ResizeSubView(p, w, h);
		}
	}

	void AbstractView::FillSingleWidget(int index, const Size& size,
			const Margin& margin)
	{
		int x = margin.left();
		int y = margin.bottom();

		int w = size.width() - margin.hsum();
		int h = size.height() - margin.vsum();

		FillSingleWidget(index, x, y, w, h);
	}

	void AbstractView::FillSingleWidget(int index, const Point& pos,
			const Size& size)
	{
		FillSingleWidget(index, pos.x(), pos.y(), size.width(), size.height());
	}

	void AbstractView::FillSingleWidget(int index, int left, int bottom,
			int width, int height)
	{
		AbstractView* widget = GetWidgetAt(index);

		if (widget) {
			ResizeSubView(widget, width, height);
			MoveSubViewTo(widget, left, bottom);

			if (widget->size().width() < width) {
				MoveSubViewTo(widget,
				        left + (width - widget->size().width()) / 2, bottom);
			}

			if (widget->size().height() < height) {
				MoveSubViewTo(widget, left,
				        bottom + (height - widget->size().height() / 2));
			}
		}
	}

	void AbstractView::FillSubWidgetsAveragely(const Point& out_pos,
			const Size& out_size, const Margin& margin, Orientation orientation,
			int alignment, int space)
	{
		if(first_subview_ == 0) return;

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

	void AbstractView::FillSubWidgetsAveragely(const Point& pos,
			const Size& size, Orientation orientation, int alignment, int space)
	{
		if(first_subview_ == 0) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(pos.x(), size.width(), space);
			AlignHorizontally(pos.y(), size.height(), alignment);
		} else {
			DistributeVertically(pos.y(), size.height(), space);
			AlignVertically(pos.x(), size.width(), alignment);
		}
	}

	void AbstractView::FillSubWidgetsAveragely(int x, int y, int width,
			int height, Orientation orientation, int alignment, int space)
	{
		if(first_subview_ == 0) return;

		if(orientation == Horizontal) {
			DistributeHorizontally(x, width, space);
			AlignHorizontally(y, height, alignment);
		} else {
			DistributeVertically(y, height, space);
			AlignVertically(x, width, alignment);
		}
	}

	float AbstractView::make_shaded_offset (short shadetop, short shadedown, float fact)
	{
		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
	}

	void AbstractView::DistributeHorizontally(int x, int width, int space)
	{
		int sum = subs_count();

		if (sum) {
			int average_width = (width - (sum - 1)* space) / sum;

			if (average_width > 0) {

				for (AbstractView* p = first_subview_; p; p = p->next_view_) {
					ResizeSubView(p, average_width, p->size().height());
					MoveSubViewTo(p, x, p->position().y());
					x += average_width + space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractView::DistributeVertically(int y, int height, int space)
	{
		int sum = subs_count();

		y = y + height;
		if (sum) {
			int average_height = (height - (sum - 1)* space) / sum;

			if (average_height > 0) {

				for (AbstractView* p = first_subview_; p; p = p->next_view_) {
					ResizeSubView(p, p->size().width(), average_height);
					y -= average_height;
					MoveSubViewTo(p, p->position().x(), y);
					y -= space;
				}

			} else {

				// TODO: set invisiable

			}
		}
	}

	void AbstractView::AlignHorizontally(int y, int height, int alignment)
	{
		for (AbstractView* p = first_subview_; p; p = p->next_view_) {
			if(p->IsExpandY()) {
				ResizeSubView(p, p->size().width(), height);
				MoveSubViewTo(p, p->position().x(), y);
			} else {

				if (alignment & AlignTop) {
					MoveSubViewTo(p, p->position().x(),
					        y + (height - p->size().height()));
				} else if (alignment & AlignBottom) {
					MoveSubViewTo(p, p->position().x(), y);
				} else if (alignment & AlignHorizontalCenter) {
					MoveSubViewTo(p, p->position().x(),
					        y + (height - p->size().height()) / 2);
				}

			}
		}
	}

	void AbstractView::AlignVertically(int x, int width, int alignment)
	{
		for (AbstractView* p = first_subview_; p; p = p->next_view_) {
			if (p->IsExpandX()) {
				ResizeSubView(p, width, p->size().height());
				MoveSubViewTo(p, x, p->position().y());
			} else {

				if (alignment & AlignLeft) {
					MoveSubViewTo(p, x, p->position().y());
				} else if (alignment & AlignRight) {
					MoveSubViewTo(p, x + (width - p->size().width()), p->position().y());
				} else if (alignment & AlignVerticalCenter) {
					MoveSubViewTo(p, x + (width - p->size().width()) / 2, p->position().y());
				}

			}
		}
	}

} /* namespace BlendInt */
