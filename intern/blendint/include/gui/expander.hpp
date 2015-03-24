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

#include <gui/abstract-button.hpp>
#include <gui/abstract-layout.hpp>

namespace BlendInt {

  class ToggleButton;

  class ExpandButton: public AbstractButton
  {
  DISALLOW_COPY_AND_ASSIGN(ExpandButton);

  public:

    ExpandButton ();

    ExpandButton (const String& text);

    virtual ~ExpandButton ();

    virtual bool IsExpandX () const;

    void SetText (const String& text);

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual void PerformRoundTypeUpdate (int round_type);

    virtual void PerformRoundRadiusUpdate (float radius);

    virtual Response Draw (AbstractWindow* context);

  };

  // --------------------------

  /**
   * @brief Expander
   */
  class Expander: public AbstractWidget
  {
  DISALLOW_COPY_AND_ASSIGN(Expander);

  public:

    Expander (const String& title,
              AbstractLayout* layout = 0,
              Orientation orient = Vertical,
              int align = AlignCenter,
              int space = 0);

    virtual ~Expander ();

    bool AddWidget (AbstractWidget* widget);

    bool InsertWidget (int index, AbstractWidget* widget);

    bool InsertWidget (int row, int column, AbstractWidget* widget);

    void SetTitle (const String& text);

    const String& GetTitle () const;

    virtual AbstractView* GetFirstSubView () const final;

    virtual AbstractView* GetLastSubView () const final;

    virtual AbstractView* GetNextSubView (const AbstractView* view) const final;

    virtual AbstractView* GetPreviousSubView (const AbstractView* view) const final;

    virtual int GetSubViewCount () const final;

    virtual bool IsSubViewActive (const AbstractView* subview) const final;

    virtual bool IsExpandX () const;

    virtual bool IsExpandY () const;

    virtual Size GetPreferredSize () const;

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

    virtual Response Draw (AbstractWindow* context);

  protected:

    void FillWidgets ();

    void FillWidgets (const Size& out_size);

    void FillWidgets (int x, int y, int width, int height);

    void OnToggled (bool toggle);

    ExpandButton* title_;

    AbstractLayout* layout_;

    int last_size_;

    Orientation orientation_;

    int alignment_;

    int space_;

    bool expand_; // expand status

  };

}
