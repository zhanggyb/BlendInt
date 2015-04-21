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

#include <deque>

#include <blendint/gui/abstract-frame.hpp>

namespace BlendInt {

class FrameSplitter;

/**
 * @brief A special frame used in FrameSplitter only
 *
 * @ingroup blendint_gui_frames
 */
class FrameSplitterHandle: public AbstractFrame
{
public:

  virtual Size GetPreferredSize () const final;

  virtual bool Contain (const Point& point) const final;

  virtual bool IsExpandX () const final;

  virtual bool IsExpandY () const final;

  virtual ~FrameSplitterHandle ();

private:

  friend class FrameSplitter;

  FrameSplitterHandle (Orientation orientation = Horizontal);

  virtual bool PreDraw (AbstractWindow* context) final;

  virtual Response Draw (AbstractWindow* context) final;

  virtual void PostDraw (AbstractWindow* context) final;

  virtual void PerformFocusOn (AbstractWindow* context) final;

  virtual void PerformFocusOff (AbstractWindow* context) final;

  virtual void PerformHoverIn (AbstractWindow* context) final;

  virtual void PerformHoverOut (AbstractWindow* context) final;

  virtual Response PerformKeyPress (AbstractWindow* context) final;

  virtual Response PerformContextMenuPress (AbstractWindow* context) final;

  virtual Response PerformContextMenuRelease (AbstractWindow* context) final;

  virtual Response PerformMousePress (AbstractWindow* context) final;

  virtual Response PerformMouseRelease (AbstractWindow* context) final;

  virtual Response PerformMouseMove (AbstractWindow* context) final;

  virtual Response PerformMouseHover (AbstractWindow* context) final;

  Orientation orientation_;

  Point last_;
  Point cursor_;

  int prev_size_;
  int next_size_;
  int nearby_pos_;

  bool hover_;

  bool pressed_;
};

// -------------------------------

/**
 * @brief Frame splitter
 *
 * @ingroup blendint_gui_frames
 */
class FrameSplitter: public AbstractFrame
{
public:

  FrameSplitter (Orientation orientation = Horizontal);

  virtual ~FrameSplitter ();

  AbstractFrame* AddFrame (AbstractFrame* frame, SizePolicy policy =
                               DefaultSizePolicy);

  /**
   * @brief Insert a frame
   * @param[in] index The index of content frame, not splitter handle
   */
  AbstractFrame* InsertFrame (int index,
                              AbstractFrame* frame,
                              SizePolicy policy = DefaultSizePolicy);

  int GetFrameIndex (AbstractFrame* frame) const;

  int GetHandleIndex (FrameSplitterHandle* handle) const;

  AbstractFrame* GetFrame (int index) const;

  FrameSplitterHandle* GetHandle (int index) const;

  int GetFramesCount () const;

  virtual bool IsExpandX () const final;

  virtual bool IsExpandY () const final;

  virtual Size GetPreferredSize () const final;

protected:

  virtual void PerformPositionUpdate (const AbstractView* source,
                                      const AbstractView* target,
                                      int x,
                                      int y) final;

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height) final;

  virtual bool PreDraw (AbstractWindow* context) final;

  virtual Response Draw (AbstractWindow* context) final;

  virtual void PostDraw (AbstractWindow* context) final;

  virtual void PerformFocusOn (AbstractWindow* context) final;

  virtual void PerformFocusOff (AbstractWindow* context) final;

  virtual void PerformHoverIn (AbstractWindow* context) final;

  virtual void PerformHoverOut (AbstractWindow* context) final;

  virtual Response PerformKeyPress (AbstractWindow* context) final;

  virtual Response PerformMousePress (AbstractWindow* context) final;

  virtual Response PerformMouseRelease (AbstractWindow* context) final;

  virtual Response PerformMouseMove (AbstractWindow* context) final;

  virtual Response PerformMouseHover (AbstractWindow* context) final;

  virtual AbstractView* RemoveSubView (AbstractView* view) final;

  void FillSubFrames ();

private:

  friend class FrameSplitterHandle;

  void AddColumn (AbstractFrame* frame, SizePolicy policy);

  void AddRow (AbstractFrame* frame, SizePolicy policy);

  void InsertColumn (int index, AbstractFrame* frame, SizePolicy policy);

  void InsertRow (int index, AbstractFrame* frame, SizePolicy policy);

  void DistributeHorizontally ();

  void DistributeHorizontallyInProportion (std::deque<int>* widget_deque,
                                           int widget_width_sum,
                                           std::deque<int>* prefer_deque,
                                           int prefer_width_sum);

  void DistributeExpandableFramesHorizontally (int unexpandable_width_sum,
                                               std::deque<int>* widget_deque,
                                               int widget_width_sum,
                                               std::deque<int>* prefer_deque,
                                               int prefer_width_sum);

  void DistributeUnexpandableFramesHorizontally (std::deque<int>* widget_deque,
                                                 int widget_width_sum,
                                                 std::deque<int>* prefer_deque,
                                                 int prefer_width_sum);

  void DistributeVertically ();

  void DistributeVerticallyInProportion (std::deque<int>* widget_deque,
                                         int widget_height_sum,
                                         std::deque<int>* prefer_deque,
                                         int prefer_height_sum);

  void DistributeExpandableFramesVertically (int unexpandable_height_sum,
                                             std::deque<int>* widget_deque,
                                             int widget_height_sum,
                                             std::deque<int>* prefer_deque,
                                             int prefer_height_sum);

  void DistributeUnexpandableFramesVertically (std::deque<int>* widget_deque,
                                               int widget_height_sum,
                                               std::deque<int>* prefer_deque,
                                               int prefer_height_sum);

  void AlignHorizontally ();

  // this will replace AlignHorizontally later
  void AlignHorizontallyExt ();

  void AlignVertically ();

  // this will replace AlignVertically later
  void AlignVerticallyExt ();

  void AlignSubFrames (Orientation orientation, const Size& size);

  int GetAverageRoom (Orientation orientation, const Size& size);

  void SetFocusedFrame (AbstractFrame* frame, AbstractWindow* context);

  void SetHoveredFrame (AbstractWindow* context);

  Orientation orientation_;

  AbstractFrame* hover_frame_;

  AbstractFrame* focused_frame_;

  bool focused_;

  bool pressed_;

  bool hover_;

  int cursor_position_;

};
}
