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

#include <gui/abstract-layout.hpp>

namespace BlendInt {

  class StackLayout: public AbstractLayout
  {
  DISALLOW_COPY_AND_ASSIGN(StackLayout);

  public:

    StackLayout ();

    virtual ~StackLayout ();

    virtual bool AddWidget (AbstractWidget* widget);

    virtual bool InsertWidget (int index, AbstractWidget* widget);

    virtual bool InsertWidget (int row, int column, AbstractWidget* widget);

    virtual void Adjust () const;

    void Remove (AbstractWidget* widget);

    int GetIndex () const;

    void SetIndex (int index);

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

    virtual Size GetPreferredSize () const;

    virtual AbstractView* GetFirstSubView ();

    virtual AbstractView* GetLastSubView ();

    virtual AbstractView* GetNextSubView (AbstractView* view);

    virtual AbstractView* GetPreviousSubView (AbstractView* view);

    virtual int GetSubViewCount ();

    inline AbstractWidget* active_widget () const
    {
      return active_widget_;
    }

  protected:

    virtual void PerformMarginUpdate (const Margin& request);

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    void HideSubWidget (int index);

  private:

    AbstractWidget* active_widget_;

  };

}
