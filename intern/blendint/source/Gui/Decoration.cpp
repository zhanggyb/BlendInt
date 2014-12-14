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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Decoration::Decoration()
	: Widget(),
	  space_(4)
	{
		set_size(200, 20);
		//set_round_type(RoundNone);
		//set_round_radius(5.f);

		InitializeDecoration();
	}

	Decoration::~Decoration ()
	{
		glDeleteVertexArrays(1, vao_);
	}

	void Decoration::Prepend (AbstractInteractiveForm* widget)
	{
	}

	void Decoration::Append (AbstractInteractiveForm* widget)
	{
	}

	bool Decoration::IsExpandX () const
	{
		return true;
	}

	bool Decoration::IsExpandY () const
	{
		return true;
	}

	Size Decoration::GetPreferredSize () const
	{
		return Size(200, 20);
	}

	void Decoration::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());
			MoveSubWidgets(x, y);

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Decoration::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			std::vector<GLfloat> inner_verts;
			GenerateRoundedVertices(&inner_verts, 0);

			inner_.bind();
			inner_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_.reset();

			int x = position().x();
			if (first_child()) {
				x = first_child()->position().x();
			}

			int y = position().y();
			int w = request.size()->width();
			int h = request.size()->height();

			FillSubWidgets(x, y, w, h, space_);

			set_size(*request.size());

		} else if (request.target()->parent() == this) {

			FillSubWidgets(position(), size(), space_);

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Decoration::PerformRoundTypeUpdate (int round_type)
	{
		std::vector<GLfloat> inner_verts;
		GenerateRoundedVertices(&inner_verts, 0);

		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	void Decoration::PerformRoundRadiusUpdate (float radius)
	{
		std::vector<GLfloat> inner_verts;
		GenerateRoundedVertices(&inner_verts, 0);
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		GLArrayBuffer::reset();

		RequestRedraw();
	}

	ResponseType Decoration::Draw (Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1, Color(Color::DarkGray).data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);

		GLSLProgram::reset();
		return Accept;
	}

	void Decoration::InitializeDecoration()
	{
		std::vector<GLfloat> inner_verts;
		GenerateRoundedVertices(&inner_verts, 0);

		glGenVertexArrays(1, vao_);

		glBindVertexArray(vao_[0]);

		inner_.generate();
		inner_.bind();
		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(0);
		inner_.reset();
	}

	void Decoration::FillSubWidgets (const Point& out_pos, const Size& out_size, int space)
	{
	}

	void Decoration::FillSubWidgets (int x, int y, int width, int height,
	        int space)
	{
	}

	void Decoration::RealignSubWidgets (const Size& size, const Margin& margin,
	        int space)
	{
	}

	int Decoration::GetLastPosition () const
	{
		return 0;
	}

}
