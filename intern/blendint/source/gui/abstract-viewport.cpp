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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <opengl/opengl.hpp>

#include <gui/abstract-viewport.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	AbstractViewport::AbstractViewport(int width, int height)
	: AbstractFrame(width, height),
	  vao_(0)
	{
    std::vector<GLfloat> outer_verts;
    GenerateVertices(size(), pixel_size(1), RoundNone, 0.f, 0,
                     &outer_verts);

    vbo_.generate();
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);
    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();
	}

	AbstractViewport::~AbstractViewport()
	{
    glDeleteVertexArrays(1, &vao_);
	}

	bool AbstractViewport::IsExpandX() const
	{
		return true;
	}

	bool AbstractViewport::IsExpandY() const
	{
		return true;
	}

  void AbstractViewport::PerformPositionUpdate (const PositionUpdateRequest& request)
  {
    if (request.target() == this) {
      set_position(*request.position());
      PostPositionUpdate();
    }

    if (request.source() == this) {
      ReportPositionUpdate(request);
    }
  }

  void AbstractViewport::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {
      set_size(*request.size());

      std::vector<GLfloat> outer_verts;
      GenerateVertices(size(), pixel_size(1), RoundNone, 0.f, 0, &outer_verts);

      vbo_.bind(0);
      vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(),
                        &outer_verts[0]);
      vbo_.reset();

      PostSizeUpdate();
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

	Response AbstractViewport::PerformMouseHover(AbstractWindow* context)
	{
		if(Contain(context->GetGlobalCursorPosition())) {
			return Finish;
		} else {
			return Ignore;
		}
	}

	Response AbstractViewport::PerformKeyPress(AbstractWindow* context)
	{
		return Ignore;
	}

	void AbstractViewport::PerformFocusOn (AbstractWindow* context)
	{
	}

	void AbstractViewport::PerformFocusOff (AbstractWindow* context)
	{
	}

	void AbstractViewport::PerformHoverIn(AbstractWindow* context)
	{
	}

	void AbstractViewport::PerformHoverOut(AbstractWindow* context)
	{
	}

	Response AbstractViewport::PerformMousePress(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response AbstractViewport::PerformMouseRelease(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response AbstractViewport::PerformMouseMove(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

  bool AbstractViewport::PreDraw (AbstractWindow* context)
  {
    if (!visiable()) return false;

    DeclareActiveFrame(context, this);

    // draw outline

    AbstractWindow::shaders()->frame_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_POSITION),
        position().x(), position().y());
    glBindVertexArray(vao_);

    glUniform4f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
                0.576f, 0.576f, 0.576f, 1.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 6);

    glUniform4f(AbstractWindow::shaders()->location(Shaders::FRAME_OUTER_COLOR),
                0.4f, 0.4f, 0.4f, 1.f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    // now set viewport for 3D scene
    glViewport(position().x(), position().y(), size().width(), size().height());

    glEnable(GL_SCISSOR_TEST);
    glScissor(position().x(), position().y(), size().width(), size().height());

    return true;
  }

  Response AbstractViewport::Draw (AbstractWindow* context)
  {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    RenderScene();

    glDisable(GL_DEPTH_TEST);

    // TODO: draw widgets

    return Finish;
  }

  void AbstractViewport::PostDraw (AbstractWindow* context)
  {
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, context->size().width(), context->size().height());
  }

}

