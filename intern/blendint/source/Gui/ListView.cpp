/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/ListView.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	ListView::ListView ()
	: AbstractItemView(),
	  highlight_index_(-1)
	{
		set_size(400, 300);

		InitializeListView();
	}

	ListView::~ListView ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool ListView::IsExpandX () const
	{
		return true;
	}

	bool ListView::IsExpandY () const
	{
		return true;
	}

	const RefPtr<AbstractItemModel> ListView::GetModel () const
	{
		return model_;
	}

	void ListView::SetModel (const RefPtr<AbstractItemModel>& model)
	{
		model_ = model;

		if(model_) {
			int h = font_.GetHeight();
			h = model_->GetRows() * h;	// total height
		}
	}

	Size ListView::GetPreferredSize () const
	{
		Size preferred_size(400, 300);

		return preferred_size;
	}

	ResponseType ListView::Draw (const Context* context)
	{
		Context* c = const_cast<Context*>(context);

		int y = position().y() + size().height();
		int h = font_.GetHeight();

        Context::shaders->widget_inner_program()->use();

		glUniform1i(Context::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		glUniform4fv(Context::shaders->location(Shaders::WIDGET_INNER_COLOR), 1,
				Context::theme->regular().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		c->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		c->EndPushStencil();

        RefPtr<GLSLProgram> program = Context::shaders->widget_triangle_program();

        glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);
		glVertexAttrib4f(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COLOR), 0.475f,
				0.475f, 0.475f, 0.75f);

		glBindVertexArray(vaos_[1]);

		int i = 0;
		while(y > position().y()) {
			y -= h;


			glUniform2f(Context::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION),
					(float) position().x(), (float) y);

			if(i == highlight_index_) {	// TODO: use different functions for performance
				glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), -35);
			} else {
				if(i % 2 == 0) {
					glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
				} else {
					glUniform1i(Context::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), 15);
				}
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			i++;
		}

		glBindVertexArray(0);
		program->reset();

		RefPtr<AbstractItemModel> model = GetModel();
		if(model) {

			ModelIndex index = model->GetRootIndex();
			index = index.GetChildIndex(0, 0);

			y = position().y() + size().height();
			while(index.IsValid()) {
				y -= h;
				font_.Print(position().x(), y, *index.GetData());
				index = index.GetDownIndex();
			}

		}

        Context::shaders->widget_inner_program()->use();

		c->BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);
		c->EndPopStencil();

		program->reset();

		return Finish;
	}

	ResponseType ListView::PerformMousePress (const Context* context)
	{
		if(model_) {

			ModelIndex index;

			int rows = model_->GetRows();

			if(rows > 0) {
				int h = font_.GetHeight();	// the row height
				int total = rows * h;

				int i = 0;
				if(total > size().height()) {
					i = position().y() - context->cursor_position().y();
				} else {	// no vbar
					i = position().y() + size().height() - context->cursor_position().y();
				}

				i = i / h;
				highlight_index_ = i;

				index = model_->GetRootIndex().GetChildIndex();
				while((i > 0) && index.IsValid()) {
					index = index.GetDownIndex();
					i--;
				}

				if(!index.IsValid()) {
					highlight_index_ = -1;
				}
			}

		} else {
			highlight_index_ = -1;
		}

		return Finish;
	}

	ModelIndex ListView::GetIndexAt (const Point& point) const
	{
		return ModelIndex();
	}

	void ListView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

            set_size(*request.size());

			GLfloat h = font_.GetHeight();
			GLfloat verts[] = {
					0.f, 0.f,
					(GLfloat)request.size()->width(), 0.f,
					0.f, h,
					(GLfloat)request.size()->width(), h
			};

			row_->bind();
			row_->set_data(sizeof(verts), verts);

            std::vector<GLfloat> inner_verts;
            GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			inner_->bind();
			inner_->set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

			GLArrayBuffer::reset();
		}

		ReportSizeUpdate(request);
	}

	void ListView::InitializeListView ()
	{
		GLfloat h = (GLfloat)font_.GetHeight();
		GLfloat verts[] = {
				0.f, 0.f,
				(GLfloat)size().width(), 0.f,
				0.f, h,
				(GLfloat)size().width(), h
		};

        std::vector<GLfloat> inner_verts;

        GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_INNER_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);

		row_.reset(new GLArrayBuffer);
		row_->generate();
		row_->bind();
		row_->set_data(sizeof(verts), verts);

		glEnableVertexAttribArray(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Context::shaders->location(Shaders::WIDGET_TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		font_.set_pen(font_.pen().x() + 4, std::abs(font_.GetDescender()));
	}

}
