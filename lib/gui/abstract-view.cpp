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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <blendint/opengl/opengl.hpp>

#include <blendint/gui/abstract-view.hpp>
#include <blendint/gui/abstract-window.hpp>

#include <blendint/gui/managed-ptr.hpp>

namespace BlendInt {

bool IsContained (AbstractView* container, AbstractView* widget)
{
  bool retval = false;

  AbstractView* p = widget->super();
  while (p) {
    if (p == container) {
      retval = true;
      break;
    }
    p = p->super();
  }

  return retval;
}

float AbstractView::kBorderWidth = 1.f;

// std::mutex AbstractView::kRefreshMutex;

const float AbstractView::cornervec[WIDGET_CURVE_RESOLU][2] = {
    { 0.0, 0.0 }, //
    { 0.195, 0.02 },  //
    { 0.383, 0.067 }, //
    { 0.55, 0.169 },  //
    { 0.707, 0.293 }, //
    { 0.831, 0.45 },  //
    { 0.924, 0.617 }, //
    { 0.98, 0.805 },  //
    { 1.0, 1.0 }      //
};

const int AbstractView::kOutlineVertexTable[16] = {
    4,  // RoundNone
    12, // RoundTopLeft
    12, // RoundTopRight
    20, // RoundTopLeft | RoundTopRight
    12, // RoundBottomRight
    20, // ...
    20, //
    28, //
    12, //
    20, //
    20, //
    28, //
    20, //
    28, //
    28, //
    36  // RoundAll
    };

const int AbstractView::kEmbossVertexTable[16] = {
    2,  //
    2,  //
    2,  //
    2,  //
    10, //
    10, //
    10, //
    10, //
    10, //
    10, //
    10, //
    10, //
    18, //
    18, //
    18, //
    18, //
};

AbstractView::AbstractView ()
: CppEvent::Trackable(),
  refresh_(false),
  destroying_(false),
  view_type_(ViewTypeUndefined),
  reference_count_(0),
  subview_count_(0),
  super_(0),
  previous_(0),
  next_(0),
  first_(0),
  last_(0)
{
}

AbstractView::AbstractView (int width, int height)
: CppEvent::Trackable(),
  refresh_(false),
  destroying_(false),
  view_type_(ViewTypeUndefined),
  reference_count_(0),
  subview_count_(0),
  super_(0),
  previous_(0),
  next_(0),
  first_(0),
  last_(0)
{
  set_size(std::abs(width), std::abs(height));
}

AbstractView::~AbstractView ()
{
  if (subview_count() > 0) {
    ClearSubViews();
  } else {
    DBG_ASSERT(subview_count_ == 0);
    DBG_ASSERT(first_ == 0);
    DBG_ASSERT(last_ == 0);
  }

  if (super_) {
    super_->RemoveSubView(this);
  } else {
    DBG_ASSERT(previous_ == 0);
    DBG_ASSERT(next_ == 0);
  }
}

Point AbstractView::GetGlobalPosition () const
{
  Point retval = position_;

  AbstractView* p = super_;
  while (p) {
    retval = retval + p->position() + p->GetOffset();
    p = p->super_;
  }

  return retval;
}

void AbstractView::Resize (int width, int height)
{
  if (size().width() == width && size().height() == height) return;

  if (super_) {
    if (super_->SizeUpdateTest(this, this, width, height)
        && SizeUpdateTest(this, this, width, height)) {
      PerformSizeUpdate(this, this, width, height);
      set_size(width, height);
    }
  } else {
    if (SizeUpdateTest(this, this, width, height)) {
      PerformSizeUpdate(this, this, width, height);
      set_size(width, height);
    }
  }
}

void AbstractView::Resize (const Size& size)
{
  if (AbstractView::size() == size) return;

  if (super_) {
    if (super_->SizeUpdateTest(this, this, size.width(), size.height())
        && SizeUpdateTest(this, this, size.width(), size.height())) {
      PerformSizeUpdate(this, this, size.width(), size.height());
      set_size(size);
    }
  } else {
    if (SizeUpdateTest(this, this, size.width(), size.height())) {
      PerformSizeUpdate(this, this, size.width(), size.height());
      set_size(size);
    }
  }
}

void AbstractView::MoveTo (int x, int y)
{
  if (position().x() == x && position().y() == y) return;

  if (super_) {
    if (super_->PositionUpdateTest(this, this, x, y)
        && PositionUpdateTest(this, this, x, y)) {
      PerformPositionUpdate(this, this, x, y);
      set_position(x, y);
    }
  } else {
    if (PositionUpdateTest(this, this, x, y)) {
      PerformPositionUpdate(this, this, x, y);
      set_position(x, y);
    }
  }
}

void AbstractView::MoveTo (const Point& pos)
{
  if (position() == pos) return;

  if (super_) {
    if (super_->PositionUpdateTest(this, this, pos.x(), pos.y())
        && PositionUpdateTest(this, this, pos.x(), pos.y())) {
      PerformPositionUpdate(this, this, pos.x(), pos.y());
      set_position(pos);
    }
  } else {
    if (PositionUpdateTest(this, this, pos.x(), pos.y())) {
      PerformPositionUpdate(this, this, pos.x(), pos.y());
      set_position(pos);
    }
  }
}

void AbstractView::RequestRedraw ()
{
  if (!refresh()) {

    AbstractView* root = this;
    AbstractView* p = super();
    //std::thread::id id = std::this_thread::get_id();

    /*
    if (id == AbstractWindow::main_thread_id()) {

      set_refresh(true);
      while (p && (!p->refresh())) {
        root = p;
        p->set_refresh(true);
        p = p->super();
      }

    }
    */
    //else {

    set_refresh(true);
    
    while (p && (!p->refresh())) {
      root = p;
      p->set_refresh(true);
      p = p->super();
    }
    
    //}

    // DBG_PRINT_MSG("%s", "----------------------------");
    
    if ((root->super() == 0) && is_window(root)) {
      // DBG_PRINT_MSG("%s", "Synchronize GL context");
      static_cast<AbstractWindow*>(root)->Synchronize();
    }

    /*
    p = super();
    while(p) {
      DBG_PRINT_MSG("superview name: %s, refresh flag: %s",
                    p->name().c_str(),
                    p->refresh() ? "True":"False");
      p = p->super();
    }
    */
    
  }
}

bool AbstractView::IsExpandX () const
{
  return false;
}

bool AbstractView::IsExpandY () const
{
  return false;
}

Size AbstractView::GetPreferredSize () const
{
  return Size(200, 200);
}

bool AbstractView::Contain (const Point& point) const
{
  if (point.x() < position_.x() || point.y() < position_.y()
      || point.x() > (position_.x() + size_.width())
      || point.y() > (position_.y() + size_.height())) {
    return false;
  }

  return true;
}

Point AbstractView::GetOffset () const
{
  return Point(0, 0);
}

AbstractView* AbstractView::GetFirstSubView () const
{
  return first_;
}

AbstractView* AbstractView::GetLastSubView () const
{
  return last_;
}

AbstractView* AbstractView::GetNextSubView (const AbstractView* subview) const
{
  DBG_ASSERT(subview && subview->super() == this);
  return subview->next_;
}

AbstractView* AbstractView::GetPreviousSubView (const AbstractView* subview) const
{
  DBG_ASSERT(subview && subview->super() == this);
  return subview->previous_;
}

int AbstractView::GetSubViewCount () const
{
  return subview_count_;
}

bool AbstractView::IsSubViewActive (const AbstractView* subview) const
{
  DBG_ASSERT(subview && subview->super() == this);
  return true;
}

bool AbstractView::SizeUpdateTest (const AbstractView* source,
                                   const AbstractView* target,
                                   int width,
                                   int height) const
{
  if (source->super() == this) {
    return false;
  } else {
    return true;
  }
}

bool AbstractView::PositionUpdateTest (const AbstractView* source,
                                       const AbstractView* target,
                                       int x,
                                       int y) const
{
  if (source->super() == this) {
    return false;
  } else {
    return true;
  }
}

bool AbstractView::IsActive (const AbstractView* view)
{
  bool active = true;

  AbstractView* parent = view->super_;
  while (parent) {

    if (!parent->IsSubViewActive(view)) {
      active = false;
      break;
    }

    view = parent;
    parent = parent->super_;
  }

  if (active && (!is_window(view))) {
    active = false;
  }

  return active;
}

void AbstractView::MoveToFirst (AbstractView* view)
{
  if (view->super_) {

    if (view->super_->first_ == view) {
      DBG_ASSERT(view->previous_ == 0);
      return;	// already at first
    }

    view->previous_->next_ = view->next_;
    if (view->next_) {
      view->next_->previous_ = view->previous_;
    } else {
      DBG_ASSERT(view->super_->last_ == view);
      view->super_->last_ = view->previous_;
    }

    view->previous_ = 0;
    view->next_ = view->super_->first_;
    view->super_->first_->previous_ = view;
    view->super_->first_ = view;

    view->super_->RequestRedraw();
  }
}

void AbstractView::MoveToLast (AbstractView* view)
{
  if (view->super_) {

    if (view->super_->last_ == view) {
      DBG_ASSERT(view->next_ == 0);
      return;	// already at last
    }

    view->next_->previous_ = view->previous_;

    if (view->previous_) {
      view->previous_->next_ = view->next_;
    } else {
      DBG_ASSERT(view->super_->first_ == view);
      view->super_->first_ = view->next_;
    }

    view->next_ = 0;
    view->previous_ = view->super_->last_;
    view->super_->last_->next_ = view;
    view->super_->last_ = view;

    view->super_->RequestRedraw();
  }
}

void AbstractView::MoveForward (AbstractView* view)
{
  if (view->super_) {

    if (view->next_) {

      AbstractView* tmp = view->next_;

      tmp->previous_ = view->previous_;
      if (view->previous_) {
        view->previous_->next_ = tmp;
      } else {
        DBG_ASSERT(view->super_->first_ == view);
        view->super_->first_ = tmp;
      }

      view->previous_ = tmp;
      view->next_ = tmp->next_;
      if (tmp->next_) {
        tmp->next_->previous_ = view;
      }
      tmp->next_ = view;

      if (view->next_ == 0) {
        DBG_ASSERT(view->super_->last_ == tmp);
        view->super_->last_ = view;
      }

      if (view->previous_) {
        DBG_ASSERT(view->previous_->next_ == view);
      }
      if (view->next_) {
        DBG_ASSERT(view->next_->previous_ == view);
      }

      view->super_->RequestRedraw();

    } else {
      DBG_ASSERT(view->super_->last_ == view);
    }

  }
}

void AbstractView::MoveBackward (AbstractView* view)
{
  if (view->super_) {

    if (view->previous_) {

      AbstractView* tmp = view->previous_;

      tmp->next_ = view->next_;
      if (view->next_) {
        view->next_->previous_ = tmp;
      } else {
        DBG_ASSERT(view->super_->last_ == view);
        view->super_->last_ = tmp;
      }

      view->next_ = tmp;
      view->previous_ = tmp->previous_;
      if (tmp->previous_) {
        tmp->previous_->next_ = view;
      }
      tmp->previous_ = view;

      if (view->previous_ == 0) {
        DBG_ASSERT(view->super_->first_ == tmp);
        view->super_->first_ = view;
      }

      DBG_PRINT_MSG("this: %s", view->name_.c_str());
      if (view->previous_) {
        DBG_PRINT_MSG("previous_view: %s", view->previous_->name_.c_str());
        DBG_ASSERT(view->previous_->next_ == view);
      }
      if (view->next_) {
        DBG_PRINT_MSG("next_view: %s", view->next_->name_.c_str());
        DBG_ASSERT(view->next_->previous_ == view);
      }

      view->super_->RequestRedraw();

    } else {
      DBG_ASSERT(view->super_->first_ == view);
    }

  }
}

void AbstractView::SetDefaultBorderWidth (float border)
{
  kBorderWidth = border;
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

int AbstractView::GetHalfOutlineVertices (int round_type)
{
  round_type = round_type & (RoundBottomLeft | RoundBottomRight);
  int count = 0;
  while (round_type != 0) {
    count += round_type & 0x1;
    round_type = round_type >> 1;
  }

  return 2 - count + count * WIDGET_CURVE_RESOLU;
}

void AbstractView::DrawSubViewsOnce (AbstractWindow* context)
{
  //bool refresh_record = false;

  for (ManagedPtr p = GetFirstSubView(); p; ++p) {

    set_refresh(false);
    
    if (p->PreDraw(context)) {

      Response response = p->Draw(context);

      p->set_refresh(refresh());

      if (response == Ignore) {
        for (ManagedPtr sub = p->GetFirstSubView(); sub; ++sub) {
          DispatchDrawEvent(sub.get(), context);
        }
      }

      p->PostDraw(context);
    }

    //if(refresh()) refresh_record = true;
  }

  //set_refresh(refresh_record);
  if (super_) {
    set_refresh(super_->refresh());
  } else {
    set_refresh(false);
  }
}

AbstractView* AbstractView::Destroy (AbstractView* view)
{
  DBG_ASSERT(view);

  view->set_destroying(true);
  if(view->reference_count_ <= 0) {
    delete view;
    return 0;
  }
  return view;
}

bool AbstractView::SwapIndex (AbstractView *view1, AbstractView *view2)
{
  if (view1 == nullptr || view2 == nullptr) return false;
  if (view1 == view2) return false;
  if (view1->super_ != view2->super_) return false;
  if (view1->super_ == nullptr) return false;

  AbstractView* tmp1 = nullptr;
  AbstractView* tmp2 = nullptr;

  if (view1->next_ == view2) {	// view1 is just the previous sibling of view2

    DBG_ASSERT(view2->previous_ == view1);

    tmp1 = view1->previous_;
    tmp2 = view2->next_;

    view2->previous_ = tmp1;
    view1->previous_ = view2;

    view2->next_ = view1;
    view1->next_ = tmp2;

    if (tmp1 != nullptr) {
      tmp1->next_ = view2;
    } else {
      view1->super_->first_ = view2;
    }

    if (tmp2 != nullptr) {
      tmp2->previous_ = view1;
    } else {
      view2->super_->last_ = view2;
    }

  } else if (view1->previous_ == view2) {

    DBG_ASSERT(view2->next_ == view1);

    tmp1 = view2->previous_;
    tmp2 = view1->next_;

    view1->previous_ = tmp1;
    view2->previous_ = view1;

    view1->next_ = view2;
    view2->next_ = tmp2;

    if (tmp1 != nullptr) {
      tmp1->next_ = view1;
    } else {
      view2->super_->first_ = view1;
    }

    if (tmp2 != nullptr) {
      tmp2->previous_ = view2;
    } else {
      view1->super_->last_ = view2;
    }

  } else {

    tmp1 = view1->previous_;
    tmp2 = view2->previous_;

    view1->previous_ = tmp2;
    view2->previous_ = tmp1;

    if (tmp1 != nullptr) {
      tmp1->next_ = view2;
    } else {
      view1->super_->first_ = view2;
    }

    if (tmp2 != nullptr) {
      tmp2->next_ = view1;
    } else {
      view2->super_->first_ = view1;
    }

    tmp1 = view1->next_;
    tmp2 = view2->next_;

    view1->next_ = tmp2;
    view2->next_ = tmp1;

    if (tmp1 != nullptr) {
      tmp1->previous_ = view2;
    } else {
      view1->super_->last_ = view2;
    }

    if (tmp2 != nullptr) {
      tmp2->previous_ = view1;
    } else {
      view2->super_->last_ = view1;
    }

  }

  return true;
}

bool AbstractView::InsertSiblingBefore (AbstractView *src, AbstractView *dst)
{
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->super_ != nullptr) {

    if (dst->super_ == src->super_) {

      if (src->previous_ == dst) {	// already is the previous one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        DBG_ASSERT(dst->super_->first_ == dst);
        dst->super_->first_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        DBG_ASSERT(dst->super_->last_ == dst);
        dst->super_->last_ = dst->previous_;
      }

      AbstractView* tmp = src->previous_;

      src->previous_ = dst;
      dst->next_ = src;
      dst->previous_ = tmp;

      if (tmp) {
        tmp->next_ = dst;
      } else {
        DBG_ASSERT(src->super_->first_ == src);
        dst->super_->first_ = dst;
      }

      return true;

    } else {
      dst->super_->RemoveSubView(dst);
    }

  }

  DBG_ASSERT(dst->super_ == nullptr);
  DBG_ASSERT(dst->next_ == nullptr);
  DBG_ASSERT(dst->previous_ == nullptr);

  AbstractView* tmp = src->previous_;

  src->previous_ = dst;
  dst->next_ = src;
  dst->previous_ = tmp;
  if (tmp) {
    tmp->next_ = dst;
  } else {
    DBG_ASSERT(src->super_->first_ == src);
    src->super_->first_ = dst;
  }

  dst->super_ = src->super_;
  src->super_->subview_count_++;

  return true;
}

bool AbstractView::InsertSiblingAfter (AbstractView *src, AbstractView *dst)
{
  if (src == nullptr || dst == nullptr) return false;
  if (src == dst) return false;

  if (dst->super_ != nullptr) {

    if (dst->previous_ == src->super_) {

      if (src->next_ == dst) {	// alrady is the next one of src
        return true;
      }

      if (dst->previous_) {
        dst->previous_->next_ = dst->next_;
      } else {
        DBG_ASSERT(dst->super_->first_ == dst);
        dst->super_->first_ = dst->next_;
      }

      if (dst->next_) {
        dst->next_->previous_ = dst->previous_;
      } else {
        DBG_ASSERT(dst->super_->last_ == dst);
        dst->super_->last_ = dst->previous_;
      }

      AbstractView* tmp = src->next_;

      src->next_ = dst;
      dst->previous_ = src;
      dst->next_ = tmp;

      if (tmp) {
        tmp->previous_ = dst;
      } else {
        DBG_ASSERT(src->super_->last_ == src);
        dst->super_->last_ = dst;
      }

      return true;

    } else {
      dst->super_->RemoveSubView(dst);
    }

  }

  DBG_ASSERT(dst->super_ == nullptr);
  DBG_ASSERT(dst->next_ == nullptr);
  DBG_ASSERT(dst->previous_ == nullptr);

  AbstractView* tmp = src->next_;

  src->next_ = dst;
  dst->previous_ = src;
  dst->next_ = tmp;
  if (tmp) {
    tmp->previous_ = dst;
  } else {
    DBG_ASSERT(src->super_->last_ == src);
    src->super_->last_ = dst;
  }

  dst->super_ = src->super_;
  src->super_->subview_count_++;

  return true;
}

void AbstractView::DispatchDrawEvent (AbstractView* view,
                                      AbstractWindow* context)
{
  DBG_ASSERT(view != 0);

  if (view->PreDraw(context)) {

    Response response = view->Draw(context);

    view->set_refresh(view->super_->refresh());

    if (response == Ignore) {
      for (ManagedPtr sub = view->GetFirstSubView(); sub; ++sub) {
        DispatchDrawEvent(sub.get(), context);
      }
    }

    view->PostDraw(context);
  }
}

void AbstractView::PerformSizeUpdate (const AbstractView* source,
                                      const AbstractView* target,
                                      int width,
                                      int height)
{
  if (target == this) {
    set_size(width, height);
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void AbstractView::PerformPositionUpdate (const AbstractView* source,
                                          const AbstractView* target,
                                          int x,
                                          int y)
{
  if (target == this) {
    set_position(x, y);
  }

  if (source == this) {
    report_position_update(source, target, x, y);
  }
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

 float border = default_border_width * theme()->pixel();

 float rad = round_radius_ * theme()->pixel();
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

 float border = default_border_width * theme()->pixel();

 float rad = round_radius_ * theme()->pixel();
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
 radi = 0.5f * minsize - border * theme()->pixel();	// U.pixelsize;

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

void AbstractView::GenerateTriangleStripVertices (const std::vector<GLfloat>* inner,
                                                  const std::vector<GLfloat>* edge,
                                                  unsigned int num,
                                                  std::vector<GLfloat>* strip)
{
  if (num > edge->size() / 2) {
    DBG_PRINT_MSG("Attempt to process %u vertices, but maximum is %ld", num,
                  edge->size() / 2);
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

void AbstractView::GenerateRawVertices (float minx,
                                        float miny,
                                        float maxx,
                                        float maxy,
                                        float border,
                                        int round_type,
                                        float radius,
                                        std::vector<GLfloat>* inner,
                                        std::vector<GLfloat>* outer)
{
  if (inner == nullptr && outer == nullptr) return;

  std::vector<GLfloat>* inner_ptr = nullptr;

  if (inner == nullptr) {
    inner_ptr = new std::vector<GLfloat>;
  } else {
    inner_ptr = inner;
  }

  float rad = radius;
  float radi = rad - border;

  float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

  float minxi = minx + border;    // U.pixelsize; // boundbox inner
  float maxxi = maxx - border;  // U.pixelsize;
  float minyi = miny + border;    // U.pixelsize;
  float maxyi = maxy - border;    // U.pixelsize;

  int minsize = 0;
  int corner = round_type;
  const int hnum =
      ((corner & (RoundTopLeft | RoundTopRight))
          == (RoundTopLeft | RoundTopRight)
          || (corner & (RoundBottomRight | RoundBottomLeft))
              == (RoundBottomRight | RoundBottomLeft)) ? 1 : 2;
  const int vnum =
      ((corner & (RoundTopLeft | RoundBottomLeft))
          == (RoundTopLeft | RoundBottomLeft)
          || (corner & (RoundTopRight | RoundBottomRight))
              == (RoundTopRight | RoundBottomRight)) ? 1 : 2;

  int count = 0;
  while (corner != 0) {
    count += corner & 0x1;
    corner = corner >> 1;
  }
  unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
  corner = round_type;

  minsize = std::min((maxx - minx) * hnum, (maxy - miny) * vnum);

  if (2.0f * rad > minsize) rad = 0.5f * minsize;

  if (2.0f * (radi + border) > minsize) radi = 0.5f * minsize - border; // U.pixelsize;

  // mult
  for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
    veci[i][0] = radi * cornervec[i][0];
    veci[i][1] = radi * cornervec[i][1];
    vec[i][0] = rad * cornervec[i][0];
    vec[i][1] = rad * cornervec[i][1];
  }

  { // generate inner vertices
    if (inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
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

  if (outer != nullptr) {

    if (border > 0.f) {

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

  if (inner == nullptr) {
    delete inner_ptr;
  }
}

void AbstractView::GenerateVertices (const Size& size,
                                     float border,
                                     int round_type,
                                     float radius,
                                     std::vector<GLfloat>* inner,
                                     std::vector<GLfloat>* outer)
{
  return GenerateRawVertices(0.f,
                             0.f,
                             pixel_size(size.width()),
                             pixel_size(size.height()),
                             pixel_size(border),
                             round_type,
                             pixel_size(radius),
                             inner,
                             outer);
}

void AbstractView::GenerateRawVertices (float minx,
                                        float miny,
                                        float maxx,
                                        float maxy,
                                        float border,
                                        int round_type,
                                        float radius,
                                        Orientation shadedir,
                                        short shadetop,
                                        short shadedown,
                                        std::vector<GLfloat>* inner,
                                        std::vector<GLfloat>* outer)
{

  if (inner == nullptr && outer == nullptr) return;

  std::vector<GLfloat>* inner_ptr = nullptr;

  if (inner == nullptr) {
    inner_ptr = new std::vector<GLfloat>;
  } else {
    inner_ptr = inner;
  }

  float rad = radius;
  float radi = rad - border;

  float vec[WIDGET_CURVE_RESOLU][2], veci[WIDGET_CURVE_RESOLU][2];

  float minxi = minx + border;
  float maxxi = maxx - border;
  float minyi = miny + border;
  float maxyi = maxy - border;

  float facxi = (maxxi != minxi) ? 1.0f / (maxxi - minxi) : 0.0f;
  float facyi = (maxyi != minyi) ? 1.0f / (maxyi - minyi) : 0.0f;

  int corner = round_type;
  int minsize = 0;
  const int hnum =
      ((corner & (RoundTopLeft | RoundTopRight))
          == (RoundTopLeft | RoundTopRight)
          || (corner & (RoundBottomRight | RoundBottomLeft))
              == (RoundBottomRight | RoundBottomLeft)) ? 1 : 2;
  const int vnum =
      ((corner & (RoundTopLeft | RoundBottomLeft))
          == (RoundTopLeft | RoundBottomLeft)
          || (corner & (RoundTopRight | RoundBottomRight))
              == (RoundTopRight | RoundBottomRight)) ? 1 : 2;

  float offset = 0.f;

  int count = 0;
  while (corner != 0) {
    count += corner & 0x1;
    corner = corner >> 1;
  }
  unsigned int outline_vertex_number = 4 - count + count * WIDGET_CURVE_RESOLU;
  corner = round_type;

  minsize = std::min((maxx - minx) * hnum, (maxy - miny) * vnum);

  if (2.0f * rad > minsize) rad = 0.5f * minsize;

  if (2.0f * (radi + border) > minsize)
    radi = 0.5f * minsize; // U.pixelsize;

        // mult
  for (int i = 0; i < WIDGET_CURVE_RESOLU; i++) {
    veci[i][0] = radi * cornervec[i][0];
    veci[i][1] = radi * cornervec[i][1];
    vec[i][0] = rad * cornervec[i][0];
    vec[i][1] = rad * cornervec[i][1];
  }

  { // generate inner vertices

    if (inner_ptr->size() != ((outline_vertex_number + 2) * 3)) {
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
          offset = make_shaded_offset(
              shadetop, shadedown,
              facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
        } else {
          offset = make_shaded_offset(
              shadetop, shadedown,
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
          offset = make_shaded_offset(
              shadetop, shadedown,
              facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
        } else {
          offset = make_shaded_offset(
              shadetop, shadedown,
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
          offset = make_shaded_offset(
              shadetop, shadedown,
              facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
        } else {
          offset = make_shaded_offset(
              shadetop, shadedown,
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
          offset = make_shaded_offset(
              shadetop, shadedown,
              facyi * ((*inner_ptr)[count * 3 + 1] - minyi));
        } else {
          offset = make_shaded_offset(
              shadetop, shadedown,
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

  if (outer != nullptr) {

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

  if (inner == nullptr) {
    delete inner_ptr;
  }
}

void AbstractView::GenerateVertices (const Size& size,
                                     float border,
                                     int round_type,
                                     float radius,
                                     Orientation shadedir,
                                     short shadetop,
                                     short shadedown,
                                     std::vector<GLfloat>* inner,
                                     std::vector<GLfloat>* outer)
{
  GenerateRawVertices(0.f,
                      0.f,
                      pixel_size(size.width()),
                      pixel_size(size.height()),
                      pixel_size(border),
                      round_type,
                      pixel_size(radius),
                      shadedir,
                      shadetop,
                      shadedown,
                      inner,
                      outer);
}

void AbstractView::GenerateVertices (const Size& size,
                                     float border,
                                     int round_type,
                                     float radius,
                                     const ColorScheme& color_scheme,
                                     std::vector<GLfloat>* inner,
                                     std::vector<GLfloat>* outer)
{
  GenerateRawVertices(0.f,
                      0.f,
                      pixel_size(size.width()),
                      pixel_size(size.height()),
                      pixel_size(border),
                      round_type,
                      pixel_size(radius),
                      Vertical,
                      color_scheme.shadetop,
                      color_scheme.shadedown,
                      inner,
                      outer);
}

AbstractView* AbstractView::GetSubViewAt (int i) const
{
  if ((i < 0) || (i >= subview_count_)) return 0;

  AbstractView* widget = 0;

  if (i < ((subview_count_ + 1) / 2)) {

    widget = first_;
    while (i > 0) {
      widget = widget->next_;
      i--;
    }

  } else {

    widget = last_;
    int max = subview_count_ - 1;
    while (i < max) {
      widget = widget->previous_;
      i++;
    }

  }

  return widget;
}

AbstractView* AbstractView::PushFrontSubView (AbstractView* view)
{
  if (!view) return 0;

  if (view->super_) {

    if (view->super_ == this) {
      DBG_PRINT_MSG("AbstractRoundWidget %s is already in container %s",
                    view->name_.c_str(), view->super_->name().c_str());
      return view;
    } else {
      // Set widget's container to 0
      view->super_->RemoveSubView(view);
    }

  }

  DBG_ASSERT(view->previous_ == 0);
  DBG_ASSERT(view->next_ == 0);
  DBG_ASSERT(view->super_ == 0);

  if (first_) {
    first_->previous_ = view;
    view->next_ = first_;
  } else {
    DBG_ASSERT(last_ == 0);
    view->next_ = 0;
    last_ = view;
  }
  first_ = view;

  view->previous_ = 0;
  view->super_ = this;
  subview_count_++;

  view->PerformAfterAdded();

  return view;
}

AbstractView* AbstractView::InsertSubView (int index, AbstractView* view)
{
  if (!view) return 0;

  if (view->super_) {

    if (view->super_ == this) {
      DBG_PRINT_MSG("AbstractRoundWidget %s is already in container %s",
                    view->name_.c_str(), view->super_->name().c_str());
      return view;
    } else {
      // Set widget's container to 0
      view->super_->RemoveSubView(view);
    }

  }

  DBG_ASSERT(view->previous_ == 0);
  DBG_ASSERT(view->next_ == 0);
  DBG_ASSERT(view->super_ == 0);

  if (first_ == 0) {
    DBG_ASSERT(last_ == 0);

    view->next_ = 0;
    last_ = view;
    first_ = view;
    view->previous_ = 0;

  } else {

    AbstractView* p = first_;

    if (index > 0) {

      while (p && (index > 0)) {
        if (p->next_ == 0) break;

        p = p->next_;
        index--;
      }

      if (index == 0) {	// insert

        view->previous_ = p->previous_;
        view->next_ = p;
        p->previous_->next_ = view;
        p->previous_ = view;

      } else {	// same as push back

        DBG_ASSERT(p == last_);
        last_->next_ = view;
        view->previous_ = last_;
        last_ = view;
        view->next_ = 0;

      }

    } else {	// same as push front

      first_->previous_ = view;
      view->next_ = first_;
      first_ = view;
      view->previous_ = 0;

    }

  }

  view->super_ = this;
  subview_count_++;

  view->PerformAfterAdded();

  return view;
}

AbstractView* AbstractView::PushBackSubView (AbstractView* view)
{
  if (!view) return 0;

  if (view->super_) {

    if (view->super_ == this) {
      DBG_PRINT_MSG("AbstractRoundWidget %s is already in container %s",
                    view->name_.c_str(), view->super_->name().c_str());
      return view;
    } else {
      // Set widget's container to 0
      view->super_->RemoveSubView(view);
    }

  }

  DBG_ASSERT(view->previous_ == 0);
  DBG_ASSERT(view->next_ == 0);
  DBG_ASSERT(view->super_ == 0);

  if (last_) {
    last_->next_ = view;
    view->previous_ = last_;
  } else {
    DBG_ASSERT(first_ == 0);
    view->previous_ = 0;
    first_ = view;
  }
  last_ = view;

  view->next_ = 0;
  view->super_ = this;
  subview_count_++;

  view->PerformAfterAdded();
  DBG_ASSERT(view->super_ == this);

  return view;
}

AbstractView* AbstractView::RemoveSubView (AbstractView* view)
{
  if (!view) return 0;

  DBG_ASSERT(view->super_ == this);
  view->PerformBeforeRemoved();
  DBG_ASSERT(view->super_ == this);

  if (view->previous_) {
    view->previous_->next_ = view->next_;
  } else {
    DBG_ASSERT(first_ == view);
    first_ = view->next_;
  }

  if (view->next_) {
    view->next_->previous_ = view->previous_;
  } else {
    DBG_ASSERT(last_ == view);
    last_ = view->previous_;
  }

  subview_count_--;
  DBG_ASSERT(subview_count_ >= 0);

  view->previous_ = 0;
  view->next_ = 0;
  view->super_ = 0;

  return view;
}

void AbstractView::PerformAfterAdded ()
{

}

void AbstractView::PerformBeforeRemoved ()
{

}

void AbstractView::ClearSubViews ()
{
  AbstractView* ptr = first_;
  AbstractView* next_ptr = 0;

  while (ptr) {

    next_ptr = ptr->next_;

    ptr->previous_ = 0;
    ptr->next_ = 0;
    ptr->super_ = 0;

    // delete ptr;
    Destroy(ptr);

    ptr = next_ptr;
  }

  subview_count_ = 0;
  first_ = 0;
  last_ = 0;
}

void AbstractView::ResizeSubView (AbstractView* sub, int width, int height)
{
  if (!sub || sub->super() != this) return;

  if (sub->size().width() == width && sub->size().height() == height) return;

  if (sub->SizeUpdateTest(this, sub, width, height)) {
    sub->PerformSizeUpdate(this, sub, width, height);
    sub->set_size(width, height);
  }
}

void AbstractView::ResizeSubView (AbstractView* sub, const Size& size)
{
  if (!sub || sub->super() != this) return;

  if (sub->size() == size) return;

  if (sub->SizeUpdateTest(this, sub, size.width(), size.height())) {
    sub->PerformSizeUpdate(this, sub, size.width(), size.height());
    sub->set_size(size);
  }
}

void AbstractView::MoveSubViewTo (AbstractView* sub, int x, int y)
{
  if (!sub || sub->super() != this) return;

  if (sub->position().x() == x && sub->position().y() == y) return;

  if (sub->PositionUpdateTest(this, sub, x, y)) {
    sub->PerformPositionUpdate(this, sub, x, y);
    sub->set_position(x, y);
  }
}

void AbstractView::MoveSubViewTo (AbstractView* sub, const Point& pos)
{
  if (!sub || sub->super() != this) return;

  if (sub->position() == pos) return;

  if (sub->PositionUpdateTest(this, sub, pos.x(), pos.y())) {
    sub->PerformPositionUpdate(this, sub, pos.x(), pos.y());
    sub->set_position(pos);
  }
}

Response AbstractView::RecursiveDispatchKeyEvent (AbstractView* subview,
                                                  AbstractWindow* context)
{
  if (subview == this) {
    return Ignore;
  } else {

    Response response = Ignore;

    if (subview->super()) {
      response = RecursiveDispatchKeyEvent(subview->super(), context);
      if (response == Finish) {
        return response;
      } else {
        return subview->PerformKeyPress(context);
      }
    } else {
      return subview->PerformKeyPress(context);
    }

  }
}

AbstractView* AbstractView::RecursiveDispatchMousePress (AbstractView* subview,
                                                         AbstractWindow* context)
{
  if (subview == this) {
    return 0;
  } else {

    Response response = Ignore;
    AbstractView* ret_val = 0;

    if (subview->super()) {

      ret_val = RecursiveDispatchMousePress(subview->super(), context);

      if (ret_val == 0) {

        response = subview->PerformMousePress(context);

        return response == Finish ? subview : 0;

      } else {
        return ret_val;
      }

    } else {
      response = subview->PerformMousePress(context);
      return response == Finish ? subview : 0;
    }

  }
}

Response AbstractView::RecursiveDispatchMouseMoveEvent (AbstractView* subview,
                                                        AbstractWindow* context)
{
  if (subview == this) {
    return Ignore;
  } else {

    if (subview->super()) {
      if (RecursiveDispatchMouseMoveEvent(subview->super(), context)
          == Ignore) {
        return subview->PerformMouseMove(context);
      } else {
        return Finish;
      }

    } else {
      return subview->PerformMouseMove(context);
    }

  }
}

Response AbstractView::RecursiveDispatchMouseReleaseEvent (AbstractView* subview,
                                                           AbstractWindow* context)
{
  if (subview == this) {
    return Ignore;
  } else {

    if (subview->super()) {
      if (RecursiveDispatchMouseReleaseEvent(subview->super(), context)
          == Ignore) {
        return subview->PerformMouseRelease(context);
      } else {
        return Finish;
      }

    } else {
      DBG_PRINT_MSG("mouse press in %s", subview->name().c_str());
      return subview->PerformMouseRelease(context);
    }

  }
}

float AbstractView::make_shaded_offset (short shadetop,
                                        short shadedown,
                                        float fact)
{
  float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
  float facm = 1.f - fact;

  return faci * (shadetop / 255.f) + facm * (shadedown / 255.f);
}

} /* namespace BlendInt */
