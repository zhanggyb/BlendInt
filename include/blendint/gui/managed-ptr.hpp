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

#pragma once

#include <blendint/gui/abstract-view.hpp>

namespace BlendInt {

// class AbstractView;

/**
 * @brief A smart pointer to hold AbstractView when iterating
 *
 * ManagedPtr is a special smart pointer works with
 * AbstractView::Destroy() only.
 *
 */
class ManagedPtr
{

 public:

  inline ManagedPtr ()
      : view_(0)
  {}
  
  inline ManagedPtr (AbstractView* view)
      : view_(view)
  {
    if(view_) ++view_->reference_count_;
  }

  inline ManagedPtr (const ManagedPtr& orig)
      : view_(orig.view_)
  {
    if(view_) ++view_->reference_count_;
  }

  ~ManagedPtr ();

  ManagedPtr& operator = (const ManagedPtr& orig);

  ManagedPtr& operator = (AbstractView* view);

  inline AbstractView* operator-> () const
  {
    return view_;
  }

  inline AbstractView& operator* () const
  {
    return *view_;
  }

  inline AbstractView* get () const
  {
    return view_;
  }

  ManagedPtr& operator++ ();

  ManagedPtr& operator-- ();
  
  inline bool operator == (const ManagedPtr& other) const
  {
    return view_ == other.view_;
  }

  inline bool operator != (const ManagedPtr& other) const
  {
    return view_ != other.view_;
  }

  inline operator bool () const
  {
    return view_ != 0;
  }

  inline bool operator< (const ManagedPtr& other) const
  {
    return view_ < other.view_;
  }

  inline bool operator<= (const ManagedPtr& other) const
  {
    return view_ <= other.view_;
  }

  inline bool operator> (const ManagedPtr& other) const
  {
    return view_ > other.view_;
  }

  inline bool operator>= (const ManagedPtr& other) const
  {
    return view_ >= other.view_;
  }
  
 private:

  AbstractView* view_;
  
};

}
