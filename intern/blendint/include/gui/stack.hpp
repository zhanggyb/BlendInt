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

#include <gui/abstract-widget.hpp>

namespace BlendInt {

  class Stack: public AbstractWidget
  {
  DISALLOW_COPY_AND_ASSIGN(Stack);

  public:

    Stack ();

    virtual ~Stack ();

    void AddWidget (AbstractWidget* widget);

    void InsertWidget (int index, AbstractWidget* widget);

    void Remove (AbstractWidget* widget);

    int GetIndex () const;

    void SetIndex (int index);

    virtual bool IsExpandX () const override;

    virtual bool IsExpandY () const override;

    virtual Size GetPreferredSize () const override;

    virtual AbstractView* GetFirstSubView () const final;

    virtual AbstractView* GetLastSubView () const final;

    virtual AbstractView* GetNextSubView (const AbstractView* view) const final;

    virtual AbstractView* GetPreviousSubView (const AbstractView* view) const final;

    virtual int GetSubViewCount () const final;

    virtual bool IsSubViewActive (const AbstractView* subview) const final;

    inline AbstractWidget* active_widget () const
    {
      return active_widget_;
    }

  protected:

    virtual void PerformSizeUpdate (const AbstractView* source, const AbstractView* target, int width, int height) final;

    virtual Response Draw (AbstractWindow* context) override;

  private:

    AbstractWidget* active_widget_;

  };

}
