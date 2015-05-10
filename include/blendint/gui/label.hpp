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

#include <string>

#include <blendint/core/types.hpp>
#include <blendint/core/margin.hpp>

#include <blendint/opengl/gl-buffer.hpp>

#include <blendint/gui/text.hpp>
#include <blendint/gui/abstract-widget.hpp>

namespace BlendInt {

/**
 * @brief A widget that displays text
 *
 * @ingroup blendint_gui_widgets
 */
class Label: public AbstractWidget
{
  DISALLOW_COPY_AND_ASSIGN(Label);

 public:

  Label (const String& label, Alignment alignment = AlignLeft);

  virtual ~Label ();

  /**
   * @brief set the label text
   * @param label
   *
   * call this function will reset the size and preferred size of this object
   */
  void SetText (const String& text);

  /**
   * @brief set the text font
   * @param font
   *
   * call this function will reset the preferred size of this object
   */
  void SetFont (const Font& font);

  void SetForeground (const Color& color);

  void SetBackground (const Color& color);

  virtual Size GetPreferredSize () const;

  virtual bool IsExpandX () const;

 protected:

  virtual void PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height);

  virtual Response PerformMousePress (AbstractWindow* context);

  virtual Response PerformMouseRelease (AbstractWindow* context);

  virtual Response PerformMouseMove (AbstractWindow* context);

  virtual Response Draw (AbstractWindow* context);

 private:

  RefPtr<Text> text_;

  Alignment alignment_;

  GLuint vao_;

  GLBuffer<> vbo_;

  Color foreground_;

  Color background_;

  static Margin kPadding;

};

} /* namespace BlendInt */
