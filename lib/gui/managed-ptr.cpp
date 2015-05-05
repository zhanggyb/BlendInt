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

#include <blendint/core/types.hpp>
#include <blendint/gui/managed-ptr.hpp>

namespace BlendInt {

ManagedPtr::~ManagedPtr()
{
  if(view_ && (--view_->reference_count_ <= 0)) {

    if(view_->destroying())
      delete view_;

    view_ = 0;
    
  }
}

ManagedPtr& ManagedPtr::operator = (const ManagedPtr& orig)
{
  AbstractView* const old = view_;
  view_ = orig.view_;

  if(view_) ++view_->reference_count_;

  if(old && (--old->reference_count_ <= 0)) {
    if(old->destroying())
      delete old;
  }

  return *this;
}

ManagedPtr& ManagedPtr::operator = (AbstractView* view)
{
  AbstractView* const old = view_;
  view_ = view;

  if(view_) ++view_->reference_count_;

  if(old && (--old->reference_count_ <= 0)) {
    if (old->destroying())
      delete old;
  }

  return *this;
}

ManagedPtr& ManagedPtr::operator ++ ()
{
  if (view_) {

    AbstractView* const old = view_;
    
    if (view_->super()) {
      view_ = view_->super()->GetNextSubView(view_);
      if(view_) ++view_->reference_count_;
      if(old && (--old->reference_count_ <= 0)) {
        if(old->destroying())
          delete old;
      }
      
    } else {

      DBG_ASSERT(view_->next_ == 0);
      view_ = 0;
      if(old && (--old->reference_count_ <= 0)) {
        if(old->destroying())
          delete old;
      }
    }
    
  }

  return *this;
}

ManagedPtr& ManagedPtr::operator -- ()
{
  if(view_) {

    AbstractView* const old = view_;
    
    if(view_->super()) {
      view_ = view_->super()->GetPreviousSubView(view_);
      if(view_) ++view_->reference_count_;
      if(old && (--old->reference_count_ <= 0)) {
        if(old->destroying())
          delete old;
      }

    } else {

      DBG_ASSERT(view_->previous_ == 0);
      view_ = 0;
      if(old && (--old->reference_count_ <= 0)) {
        if(old->destroying())
          delete old;
      }
      
    }

  }

  return *this;
}

}
