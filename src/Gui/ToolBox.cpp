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

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ToolBox::ToolBox()
	: Layout(),
	  vao_(0),
	  space_(1)
	{
		set_size(200, 400);

		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		inner_.generate();
		inner_.bind();

		inner_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_.reset();
	}

	ToolBox::~ToolBox()
	{
		glDeleteVertexArrays(1, &vao_);
	}
	
	void ToolBox::Append (AbstractWidget* widget)
	{
		int x = margin().left();
		int y = GetLastPosition();
		int w = size().width() - margin().hsum();

		if(PushBackSubWidget(widget)) {
			Size prefer = widget->GetPreferredSize();
			y = y - prefer.height();
			SetSubWidgetPosition(widget, x, y);
			if(widget->IsExpandX()) {
				ResizeSubWidget(widget, w, prefer.height());
			} else {
				if(widget->size().width() > w) {
					ResizeSubWidget(widget, w, prefer.height());
				} else {
					ResizeSubWidget(widget, widget->size().width(), prefer.height());
				}
			}
		}
	}
	
	bool ToolBox::IsExpandY () const
	{
		return true;
	}
	
	Size ToolBox::GetPreferredSize () const
	{
		Size preferred_size;

		if(first_child() == 0) {

			preferred_size.set_width(200);
			preferred_size.set_height(400);

		} else {

			Size tmp_size;
			preferred_size.set_height(-space_);

			for(AbstractWidget* p = first_child(); p; p = p->next())
			{
				if(p->visiable()) {
					tmp_size = p->GetPreferredSize();

					preferred_size.add_height(tmp_size.height() + space_);
					preferred_size.set_width(std::max(preferred_size.width(), tmp_size.width()));
				}
			}

			preferred_size.add_width(margin().hsum());
			preferred_size.add_height(margin().vsum());
		}

		return preferred_size;
	}

	bool ToolBox::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	void ToolBox::PerformMarginUpdate (const Margin& request)
	{
		int x = request.left();
		int y = request.bottom();
		int w = size().width() - request.hsum();
		int h = size().height() - request.vsum();

		FillSubWidgets(x, y, w, h, space_);
	}

	void ToolBox::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			int x = margin().left();
			int y = margin().bottom();
			int w = request.size()->width() - margin().hsum();
			int h = request.size()->height() - margin().vsum();

			FillSubWidgets(x, y, w, h, space_);

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0, RoundNone, 0.f, &inner_verts, 0);

			inner_.bind();
			inner_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_.reset();

		} else if (request.target()->parent() == this) {
			FillSubWidgets(size(), margin(), space_);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType ToolBox::Draw (Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_INNER_POSITION), 0.f, 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_ANTI_ALIAS), 0);
		glUniform4f(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		GLSLProgram::reset();

		return Ignore;
	}

	int ToolBox::GetLastPosition () const
	{
		int y = size().height() - margin().top();

		if(last_child()) {
			y = last_child()->position().y();
			y -= space_;
		}

		return y;
	}

	void ToolBox::FillSubWidgets (const Size& out_size,
					const Margin& margin, int space)
	{
		int x = margin.left();
		int y = margin.bottom();
		int width = out_size.width() - margin.hsum();
		int height = out_size.height() - margin.vsum();

		FillSubWidgets (x, y, width, height, space);
	}

	void ToolBox::FillSubWidgets (int x, int y, int width, int height,
					int space)
	{
		y = y + height + space;

		for(AbstractWidget* p = first_child(); p; p = p->next())
		{
			y = y - p->size().height() - space;

			SetSubWidgetPosition(p, x, y);

			if(p->IsExpandX()) {
				ResizeSubWidget(p, width, p->size().height());
			} else {

				if(p->size().width() > width) {
					ResizeSubWidget(p, width, p->size().height());
				} else {
					SetSubWidgetPosition(p, x + (width - p->size().width()) / 2,
									y);
				}

			}
		}
	}

}
