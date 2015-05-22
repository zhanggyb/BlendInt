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

#include <blendint/gui/abstract-round-frame.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

  AbstractRoundFrame::AbstractRoundFrame (int frame_flag)
  : AbstractFrame(frame_flag),
    round_frame_flag_(0),
    round_radius_(5.f)
  {
  }

  AbstractRoundFrame::AbstractRoundFrame (int width, int height, int frame_flag)
  : AbstractFrame(width, height, frame_flag),
    round_frame_flag_(0),
    round_radius_(5.f)
  {
  }

  AbstractRoundFrame::~AbstractRoundFrame ()
  {
  }

  AbstractView* AbstractRoundFrame::GetFocusedView () const
  {
    return nullptr;
  }

  void AbstractRoundFrame::SetRoundType (int type)
  {
    if ((round_frame_flag_ & 0x0F) == (type & 0x0F)) return;

    PerformRoundTypeUpdate(type & 0x0F);
  }

  void AbstractRoundFrame::SetRoundRadius (float radius)
  {
    if (round_radius_ == radius) return;

    PerformRoundRadiusUpdate(radius);
  }

  bool AbstractRoundFrame::PreDraw (AbstractWindow* context)
  {
    return true;
  }

  Response AbstractRoundFrame::Draw (AbstractWindow* context)
  {
    DrawSubViewsOnce(context);

    return subview_count() ? Ignore : Finish;
  }

  void AbstractRoundFrame::PostDraw (AbstractWindow* context)
  {

  }

  void AbstractRoundFrame::PerformFocusOn (AbstractWindow* context)
  {
  }

  void AbstractRoundFrame::PerformFocusOff (AbstractWindow* context)
  {

  }

  void AbstractRoundFrame::PerformHoverIn (AbstractWindow* context)
  {
  }

  void AbstractRoundFrame::PerformHoverOut (AbstractWindow* context)
  {
  }

  Response AbstractRoundFrame::PerformContextMenuPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformContextMenuRelease (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformKeyPress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformMousePress (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformMouseRelease (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformMouseMove (AbstractWindow* context)
  {
    return Ignore;
  }

  Response AbstractRoundFrame::PerformMouseHover (AbstractWindow* context)
  {
    return Ignore;
  }

  void AbstractRoundFrame::GenerateRoundedVertices (std::vector<GLfloat>* inner,
                                                    std::vector<GLfloat>* outer)
  {
    GenerateVertices(size(),
                     default_border_width(),
                     round_type(),
                     round_radius_,
                     inner,
                     outer);
  }

  void AbstractRoundFrame::GenerateRoundedVertices (Orientation shadedir,
                                                    short shadetop,
                                                    short shadedown,
                                                    std::vector<GLfloat>* inner,
                                                    std::vector<GLfloat>* outer)
  {
    GenerateVertices(size(),
                     default_border_width(),
                     round_type(),
                     round_radius_,
                     shadedir,
                     shadetop,
                     shadedown,
                     inner,
                     outer);
  }

  void AbstractRoundFrame::PerformRoundTypeUpdate (int round_type)
  {
    set_round_type(round_type);
  }

  void AbstractRoundFrame::PerformRoundRadiusUpdate (float radius)
  {
    round_radius_ = radius;
  }

}
