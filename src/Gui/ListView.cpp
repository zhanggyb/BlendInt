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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/ListView.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ListView::ListView ()
	: AbstractItemView(),
	  vao_(0),
	  highlight_index_(-1)
	{
		set_size(400, 300);
		set_drop_shadow(true);

		InitializeListView();

		AdjustScrollBarGeometries(position().x(), position().y(),
		        size().width(), size().height());
	}

	ListView::~ListView ()
	{
		glDeleteVertexArrays(1, &vao_);
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

			if(h > size().height()) {
				vbar()->SetVisible(true);
				vbar()->SetMaximum(h);
				vbar()->SetMinimum(size().height());
				vbar()->SetSliderPercentage(size().height() * 100 / h);
			} else {
				vbar()->SetVisible(false);
			}
			hbar()->SetVisible(false);

			AdjustScrollBarGeometries(position().x(), position().y(),
					size().width(), size().height());
		}
	}

	Size ListView::GetPreferredSize () const
	{
		Size preferred_size(400, 300);

		return preferred_size;
	}

	ResponseType ListView::Draw (Profile& profile)
	{
		int y = position().y() + size().height();

		if(model_ && vbar()->visiable()) {
			y = position().y() + vbar()->value();
		}

		int h = font_.GetHeight();

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(),
				position().y(),
				size().width(),
				size().height());

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.475f,
				0.475f, 0.475f, 0.75f);

		glBindVertexArray(vao_);

		int i = 0;
		while(y > position().y()) {
			y -= h;

			glUniform3f(Shaders::instance->triangle_uniform_position(),
					(float) position().x(), (float) y, 0.f);

			if(i == highlight_index_) {	// TODO: use different functions for performance
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), -35);
			} else {
				if(i % 2 == 0) {
					glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
				} else {
					glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
				}
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			i++;
		}

		glBindVertexArray(0);
		program->Reset();

		RefPtr<AbstractItemModel> model = GetModel();
		if(model) {

			ModelIndex index = model->GetRootIndex();
			index = index.GetChildIndex(0, 0);

			y = position().y() + size().height();
			if(vbar()->visiable()) {
				y = position().y() + vbar()->value();
			}

			while(index.IsValid()) {

				y -= h;
				font_.Print(position().x(), y, *index.GetData());

				index = index.GetDownIndex();
			}

		}

        DispatchDrawEvent(hbar(), profile);
		DispatchDrawEvent(vbar(), profile);

		glDisable(GL_SCISSOR_TEST);

		return Accept;
	}

	ResponseType ListView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ListView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ListView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ListView::MousePressEvent (const MouseEvent& event)
	{
		if (hbar()->visiable() && hbar()->Contain(event.position())) {
			DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->visiable() && vbar()->Contain(event.position())) {
			DispatchMousePressEvent(vbar(), event);
		}

		if(model_) {

			ModelIndex index;

			int rows = model_->GetRows();

			if(rows > 0) {
				int h = font_.GetHeight();	// the row height
				int total = rows * h;

				int i = 0;
				if(total > size().height()) {
					i = position().y() + vbar()->value() - event.position().y();
				} else {	// no vbar
					i = position().y() + size().height() - event.position().y();
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

		return Accept;
	}

	ResponseType ListView::MouseReleaseEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			DispatchMouseReleaseEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			DispatchMouseReleaseEvent(vbar(), event);
		}

		return Accept;
	}

	ModelIndex ListView::GetIndexAt (const Point& point) const
	{
		return ModelIndex();
	}

	ResponseType ListView::MouseMoveEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			DispatchMouseMoveEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			DispatchMouseMoveEvent(vbar(), event);
		}

		return Accept;
	}

	void ListView::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(),
					request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
	}

	void ListView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			GLfloat h = font_.GetHeight();
			GLfloat verts[] = {
					0.f, 0.f,
					(GLfloat)request.size()->width(), 0.f,
					0.f, h,
					(GLfloat)request.size()->width(), h
			};

			inner_->Bind();
			inner_->SetData(sizeof(verts), verts);
			inner_->Reset();

			AdjustScrollBarGeometries(position().x(), position().y(),
					request.size()->width(), request.size()->height());
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

		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		inner_->SetData(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_->Reset();

		font_.set_pen(font_.pen().x() + 4, std::abs(font_.GetDescender()));
	}

}
