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

#include <blendint/opengl/opengl.hpp>
#include <blendint/stock/theme.hpp>
#include <blendint/stock/shaders.hpp>
#include <blendint/gui/view-buffer.hpp>

namespace BlendInt {

// FIXME: the default constructor has no effect
/*
ViewBuffer::ViewBuffer ()
    : AbstractForm(),
      vao_(0)
{
  glGenVertexArrays(1, &vao_);
  vbo_.generate();

  GLfloat vertices[] = {
    // coord						uv
    0.f, 0.f,						0.f, 0.f,
    (float)size().width(), 0.f,				1.f, 0.f,
    0.f, (float)size().height(),			0.f, 1.f,
    (float)size().width(), (float)size().height(),	1.f, 1.f
  };

  glBindVertexArray(vao_);

  vbo_.bind();
  vbo_.set_data(sizeof(vertices), vertices);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE,
		        sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(AttributeUV);
  glVertexAttribPointer(AttributeUV, 2,
                        GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

  glBindVertexArray(0);
  vbo_.reset();
}
*/

ViewBuffer::ViewBuffer (int width, int height)
    : AbstractForm(width, height),
      vao_(0)
{
  glGenVertexArrays(1, &vao_);
  vbo_.generate();

  GLfloat vertices[] = {
    // coord						uv
    0.f, 0.f,						0.f, 0.f,
    pixel_size(size().width()), 0.f,			1.f, 0.f,
    0.f, pixel_size(size().height()),			0.f, 1.f,
    pixel_size(size().width()), pixel_size(size().height()),	1.f, 1.f
  };

  glBindVertexArray(vao_);

  vbo_.bind();
  vbo_.set_data(sizeof(vertices), vertices);

  glEnableVertexAttribArray(AttributeCoord);
  glEnableVertexAttribArray(AttributeUV);
  glVertexAttribPointer(AttributeCoord,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat) * 4,
                        BUFFER_OFFSET(0));
  glVertexAttribPointer(AttributeUV,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(GLfloat) * 4,
                        BUFFER_OFFSET(2 * sizeof(GLfloat)));

  glBindVertexArray(0);
  vbo_.reset();
}

ViewBuffer::~ViewBuffer ()
{
  glDeleteVertexArrays(1, &vao_);
}

static size_t count = 0;

void ViewBuffer::Draw (int x, int y, const float* color_ptr, short gamma,
                       float rotate, float scale_x, float scale_y) const
{
  texture_.bind();

#ifdef DEBUG
  if (!glIsTexture(texture_.id())) {
    DBG_PRINT_MSG("Error: %s", "texutre not valid");
  }
#endif

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ViewBuffer::DrawInRect (const Rect& rect, int align, const float* color_ptr,
                             short gamma, float rotate, bool scale) const
{
}

#ifdef DEBUG

void ViewBuffer::SaveToFile(const char* name)
{
  texture_.bind();
  if (!glIsTexture(texture_.id())) {
    DBG_PRINT_MSG("ERROR: %s", "texture not valid");
  }
  texture_.WriteToFile(name);
}

#endif

void ViewBuffer::PerformSizeUpdate (int width, int height)
{
  set_size(width, height);

  GLfloat vertices[] = {
    // coord                      	uv
    0.f, 0.f,                     	0.f, 0.f,
    pixel_size(width), 0.f,            	1.f, 0.f,
    0.f, pixel_size(height),           	0.f, 1.f,
    pixel_size(width), pixel_size(height),	1.f, 1.f
  };

  vbo_.bind(0);
  vbo_.set_data(sizeof(vertices), vertices);
  vbo_.reset();

  // FIXME: why the following code cause crash in OS X sometimes:
  
  /*
    vbo_.bind(0);
    GLfloat* buf_p = (GLfloat*) vbo_.map(GL_READ_WRITE);
    *(buf_p + 4) = (float) size.width();
    *(buf_p + 9) = (float) size.height();
    *(buf_p + 12) = (float) size.width();
    *(buf_p + 13) = (float) size.height();
    vbo_.unmap();
    vbo_.reset();
  */
}

}  // namespace BlendInt
