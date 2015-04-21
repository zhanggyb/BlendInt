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

#include <blendint/opengl/gl-vertexarrays.hpp>
#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/text.hpp>
#include <blendint/gui/abstract-slider.hpp>

namespace BlendInt {

class NumericalSlider: public AbstractSlider<double>
{
  DISALLOW_COPY_AND_ASSIGN (NumericalSlider);

public:

  NumericalSlider (Orientation orientation = Horizontal);

  NumericalSlider (const String& title, Orientation orientation = Horizontal);

  virtual ~NumericalSlider ();

  void SetTitle (const String& title);

  virtual bool IsExpandX () const;

  virtual Size GetPreferredSize () const;

protected:

  virtual void PerformOrientationUpdate (Orientation orientation);

  virtual void PerformMinimumUpdate (double minimum);

  virtual void PerformMaximumUpdate (double maximum);

  virtual void PerformValueUpdate (double value);

  virtual void PerformStepUpdate (double step);

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual void PerformRoundTypeUpdate (int round_type);

  virtual void PerformRoundRadiusUpdate (float radius);

  virtual Response Draw (AbstractWindow* context);

  virtual void PerformFocusOn (AbstractWindow* context);

  virtual void PerformFocusOff (AbstractWindow* context);

  virtual void PerformHoverIn (AbstractWindow* context);

  virtual void PerformHoverOut (AbstractWindow* context);

  virtual Response PerformKeyPress (AbstractWindow* context);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseMove (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

private:

  void InitializeNumericalSlider ();

  Response KeyPressInEditMode (AbstractWindow* context);

  void DisposeBackspacePress ();

  void DisposeDeletePress ();

  void DisposeLeftPress ();

  void DisposeRightPress ();

  float GetSlidePosition (float border, double value);

  bool GetNewValue (const Point& cursor, double* out_v);

  void DrawSlideMode (AbstractWindow* context);

  void DrawEditMode (AbstractWindow* context);

  /**
   * @brief VertexArray objects used in this widget
   *
   * [0] - inner buffer
   * [1] - outer buffer
   */
  GLVertexArrays<3> vao_;

  GLBuffer<ARRAY_BUFFER, 3> vbo_;

  RefPtr<Text> title_text_;

  RefPtr<Text> value_text_;

  Point last_cursor_position_;

  Font font_;

  size_t text_start_; // where start print the text

  /**
   * @brief Where display the cursor and insert new text
   */
  size_t cursor_index_;

  double last_value_;

  bool hover_;

  bool pressed_;

  bool moved_;

  bool edit_mode_;

  static const int vertical_space = 2;

  static Margin kPadding;
};

}
