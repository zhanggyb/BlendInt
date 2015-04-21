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

#include <blendint/core/margin.hpp>
#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/abstract-icon.hpp>
#include <blendint/gui/text.hpp>

#include <blendint/gui/abstract-list-model.hpp>
#include <blendint/gui/abstract-item-view.hpp>
#include <blendint/gui/abstract-round-widget.hpp>

namespace BlendInt {

class AbstractFrame;  // forward declare

class ComboListModel: public AbstractListModel
{
public:

  ComboListModel ();

  virtual ~ComboListModel ();

  virtual bool InsertColumns (int column, int count, const ModelIndex& parent =
                                  ModelIndex()) final;

  virtual bool RemoveColumns (int column, int count, const ModelIndex& parent =
                                  ModelIndex()) final;

  virtual bool InsertRows (int row, int count, const ModelIndex& parent =
                               ModelIndex());

  virtual bool RemoveRows (int row, int count, const ModelIndex& parent =
                               ModelIndex());

  virtual int GetRowCount (const ModelIndex& parent = ModelIndex()) const final;

  virtual int GetColumnCount (const ModelIndex& parent =
      ModelIndex()) const final;

  virtual int GetPreferredColumnWidth (int index, const ModelIndex& parent =
                                           ModelIndex()) const override;

  virtual int GetPreferredRowHeight (int index, const ModelIndex& parent =
                                         ModelIndex()) const override;

  void SetIcon (const ModelIndex& index, const RefPtr<AbstractIcon>& icon);

  void SetText (const ModelIndex& index, const RefPtr<Text>& text);

  const Size& max_icon_size () const
  {
    return max_icon_size_;
  }

  const Size& max_text_size () const
  {
    return max_text_size_;
  }

private:

  int rows_;

  int columns_;

  Size max_icon_size_;  // preferred icon size

  Size max_text_size_; // preferred text size

};

// ----------------

class ComboListView: public AbstractItemView
{
DISALLOW_COPY_AND_ASSIGN(ComboListView);

public:

  ComboListView ();

  virtual ~ComboListView ();

  virtual bool IsExpandX () const;

  virtual bool IsExpandY () const;

  virtual Size GetPreferredSize () const final;

  virtual const RefPtr<AbstractItemModel> GetModel () const final;

  virtual void SetModel (const RefPtr<AbstractItemModel>& model) final;

  virtual ModelIndex GetIndexAt (const Point& point) const;

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual Response Draw (AbstractWindow* context) final;

  virtual Response PerformMousePress (AbstractWindow* context) final;

private:

  // 0 for inner buffer
  // 1 for row_ background
  GLuint vao_[2];

  GLBuffer<ARRAY_BUFFER, 2> vbo_;

  RefPtr<AbstractItemModel> model_;

  int highlight_index_;

  static const int kPaddingLeft = 5;

  static const int kPaddingRight = 5;
};

// ----------------

/**
 * @brief A combined button and popup list.
 *
 * @ingroup blendint_gui_widgets
 */
class ComboBox: public AbstractRoundWidget
{
DISALLOW_COPY_AND_ASSIGN(ComboBox);

public:

  enum DisplayMode
  {
    IconMode, TextMode, IconTextMode, TextIconMode
  };

  ComboBox (DisplayMode mode = IconTextMode);

  virtual ~ComboBox ();

  virtual Size GetPreferredSize () const;

  void SetModel (const RefPtr<ComboListModel>& model);

  void SetCurrentIndex (int index);

protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual void PerformRoundTypeUpdate (int round_type);

  virtual void PerformRoundRadiusUpdate (float radius);

  virtual Response Draw (AbstractWindow* context);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

private:

  void InitializeComboBox ();

  void OnPopupListDestroyed (AbstractFrame* frame);

  GLuint vaos_[2];

  GLBuffer<ARRAY_BUFFER, 2> vbo_;

  bool status_down_;

  DisplayMode display_mode_;

  int last_round_status_;

  RefPtr<ComboListModel> model_;

  ModelIndex current_index_;

  AbstractFrame* popup_;

  static Margin kPadding;

};

}
