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


#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/FileBrowser.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FileBrowser::FileBrowser ()
	: AbstractItemView(),
	  vao_(0),
	  highlight_index_(-1)
	{
		set_drop_shadow(true);
		set_size(400, 300);

		InitializeFileBrowserOnce();
	}

	FileBrowser::~FileBrowser ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	bool FileBrowser::Load (const std::string& pathname)
	{
		assert(model_);

		bool retval = false;

		retval = model_->Load(pathname);

		if(retval) {
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

		return retval;
	}

	bool FileBrowser::IsExpandX() const
	{
		return true;
	}

	bool FileBrowser::IsExpandY() const
	{
		return true;
	}

	const RefPtr<AbstractItemModel> FileBrowser::GetModel () const
	{
		return model_;
	}

	void FileBrowser::SetModel (const RefPtr<AbstractItemModel>& model)
	{
		RefPtr<FileSystemModel> fs_model = RefPtr<FileSystemModel>::cast_dynamic(model);

		if(fs_model) {
			model_ = fs_model;
		} else {
			DBG_PRINT_MSG("Error: %s", "FileBrowser only accept FileSystemModel");
		}
	}

	ModelIndex FileBrowser::GetIndexAt (const Point& point) const
	{
		ModelIndex index;

		int rows = model_->GetRows();

		if(rows > 0) {
			int h = font_.GetHeight();	// the row height
			int total = rows * h;

			int i = 0;
			if(total > size().height()) {
				i = position().y() + vbar()->value() - point.y();
			} else {	// no vbar
				i = position().y() + size().height() - point.y();
			}

			i = i / h;

			index = model_->GetRootIndex();

			index = index.GetChildIndex(0, 0);
			while((i > 0) && index.IsValid()) {
				index = index.GetDownIndex();
				i--;
			}
		}

		return index;
	}

	ResponseType FileBrowser::Draw (const RedrawEvent& event)
	{
		int y = position().y() + size().height();

		if(vbar()->visiable()) {
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

			if(i == highlight_index_) {
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

		if(GetModel()) {

			ModelIndex index = GetModel()->GetRootIndex();
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

        DispatchDrawEvent(hbar(), event);
		DispatchDrawEvent(vbar(), event);

		glDisable(GL_SCISSOR_TEST);

		return Accept;
	}

	void FileBrowser::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(), request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
	}

	void FileBrowser::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		namespace fs = boost::filesystem;

		if(request.target() == this) {

			int h = font_.GetHeight();
			GLfloat verts[] = {
					0.f, 0.f,
					(GLfloat)request.size()->width(), 0.f,
					0.f, (GLfloat)h,
					(GLfloat)request.size()->width(), (GLfloat)h
			};

			inner_->Bind();
			inner_->SetData(sizeof(verts), verts);
			inner_->Reset();

			h = model_->GetRows() * h;	// total height

			if(h > request.size()->height()) {
				vbar()->SetVisible(true);
				vbar()->SetMaximum(h);
				vbar()->SetMinimum(request.size()->height());
				vbar()->SetSliderPercentage(request.size()->height() * 100 / h);
			} else {
				vbar()->SetVisible(false);
			}
			hbar()->SetVisible(false);

			AdjustScrollBarGeometries(position().x(), position().y(),
					request.size()->width(), request.size()->height());
		}

		ReportSizeUpdate(request);
	}

	ResponseType FileBrowser::MousePressEvent (const MouseEvent& event)
	{
		if (hbar()->visiable() && hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->visiable() && vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

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

		//DBG_PRINT_MSG("highlight index: %d", highlight_index_);

		if(index.IsValid()) {
			file_selected_ = *index.GetData();
			//DBG_PRINT_MSG("index item: %s", ConvertFromString(file_selected_).c_str());
		} else {
			file_selected_.clear();
		}

		clicked_.fire();
		return Accept;
	}

	ResponseType FileBrowser::MouseReleaseEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseReleaseEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseReleaseEvent(vbar(), event);
		}

		return Accept;
	}

	ResponseType FileBrowser::MouseMoveEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {

			return DispatchMouseMoveEvent(hbar(), event);

		} else if (vbar()->pressed()) {

			return DispatchMouseMoveEvent(vbar(), event);

		}

		return Accept;
	}

	void FileBrowser::InitializeFileBrowserOnce ()
	{
		GLfloat row_height = (GLfloat)font_.GetHeight();
		GLfloat verts[] = {
						0.f, 0.f,
						(GLfloat)size().width(), 0.f,
						0.f, row_height,
						(GLfloat)size().width(), row_height
		};

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		inner_->SetData(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		inner_->Reset();

		font_.set_color(Color(0xF0F0F0FF));
		font_.set_pen(font_.pen().x() + 4, std::abs(font_.GetDescender()));

		model_.reset(new FileSystemModel);

		Load(getenv("PWD"));

		events()->connect(hbar_moved(), this, &FileBrowser::OnHBarSlide);
		events()->connect(vbar_moved(), this, &FileBrowser::OnVBarSlide);
	}

	void FileBrowser::OnHBarSlide (int val)
	{
		//DBG_PRINT_MSG("val: %d", val);
	}

	void FileBrowser::OnVBarSlide (int val)
	{
		//DBG_PRINT_MSG("val: %d", val);
	}

}

