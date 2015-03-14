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

#include <core/string.hpp>
#include <opengl/gl-buffer.hpp>
#include <gui/abstract-button.hpp>

namespace BlendInt {

  /**
   * @brief Tab button
   *
   * @ingroup blendint_gui_widgets_buttons
   */
  class TabButton: public AbstractButton
  {
  DISALLOW_COPY_AND_ASSIGN(TabButton);

  public:

    TabButton ();

    TabButton (const String& text);

    virtual ~TabButton ();

    virtual Size GetPreferredSize () const;

#ifdef DEBUG

    void GenerateTabButtonVertices (const Size& size,
                                    float border,
                                    std::vector<GLfloat>& inner,
                                    std::vector<GLfloat>& outer);

#endif

    void SetText (const String& text);

  protected:

    virtual void PerformSizeUpdate (const SizeUpdateRequest& request) final;

    virtual Response Draw (AbstractWindow* context) override;

#ifndef DEBUG

    void GenerateTabButtonVertices (const Size& size, float border,
        std::vector<GLfloat>& inner,
        std::vector<GLfloat>& outer);

#endif

    inline double sin_curve (double x,
                             double amplitude,
                             double shift_x,
                             double shift_y);

  private:

    void InitializeTabButton ();

    /**
     * @brief Vertex Arrays for widget
     *
     * [0] - for inner buffer
     * [1] - for outline buffer
     */
    GLuint vao_[2];

    GLBuffer<ARRAY_BUFFER, 2> vbo_;
  };

}
