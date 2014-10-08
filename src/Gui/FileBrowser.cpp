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
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	FileBrowser::FileBrowser ()
	: AbstractItemView(),
	  highlight_index_(-1)
	{
		set_size(400, 300);

		InitializeFileBrowserOnce();
	}

	FileBrowser::~FileBrowser ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool FileBrowser::Load (const std::string& pathname)
	{
		assert(model_);

		bool retval = false;

		retval = model_->Load(pathname);

		if(retval) {
			int h = font_.GetHeight();
			h = model_->GetRows() * h;	// total height

			ScrollBar* hbar = GetHScrollBar();
			ScrollBar* vbar = GetVScrollBar();

			if(h > size().height()) {
				vbar->SetVisible(true);
				vbar->SetMaximum(h);
				vbar->SetMinimum(size().height());
				vbar->SetSliderPercentage(size().height() * 100 / h);
			} else {
				vbar->SetVisible(false);
			}
			hbar->SetVisible(false);

			AdjustScrollBarGeometries(hbar, vbar);
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
				i = position().y() + GetVScrollBar()->value() - point.y();
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

	ResponseType FileBrowser::Draw (Profile& profile)
	{
		int y = position().y() + size().height();

		ScrollBar* vbar = GetVScrollBar();
		if(vbar->visiable()) {
			y = position().y() + vbar->value();
		}

		int h = font_.GetHeight();

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR),
				Theme::instance->regular().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		profile.BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		profile.EndPushStencil();


		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);
		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR), 0.475f,
				0.475f, 0.475f, 0.75f);

		glBindVertexArray(vaos_[1]);

		int i = 0;
		while(y > position().y()) {
			y -= h;

			glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION),
					(float) position().x(), (float) y, 0.f);

			if(i == highlight_index_) {
				glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), -35);
			} else {
				if(i % 2 == 0) {
					glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
				} else {
					glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 15);
				}
			}

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			i++;
		}

		glBindVertexArray(0);
		program->reset();

		if(GetModel()) {

			ModelIndex index = GetModel()->GetRootIndex();
			index = index.GetChildIndex(0, 0);

			y = position().y() + size().height();
			if(vbar->visiable()) {
				y = position().y() + vbar->value();
			}

			while(index.IsValid()) {

				y -= h;
				font_.Print(position().x(), y, *index.GetData());

				index = index.GetDownIndex();
			}

		}

		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		profile.BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);
		profile.EndPopStencil();

		program->reset();

		return Accept;
	}

	void FileBrowser::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(), request.position()->y(), size().width(), size().height());
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
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

			row_->bind();
			row_->set_data(sizeof(verts), verts);

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0.f);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());

			GLArrayBuffer::reset();

			h = model_->GetRows() * h;	// total height

			ScrollBar* hbar = GetHScrollBar();
			ScrollBar* vbar = GetVScrollBar();
			if(h > request.size()->height()) {
				vbar->SetVisible(true);
				vbar->SetMaximum(h);
				vbar->SetMinimum(request.size()->height());
				vbar->SetSliderPercentage(request.size()->height() * 100 / h);
			} else {
				vbar->SetVisible(false);
			}

			hbar->SetVisible(false);

			AdjustScrollBarGeometries(hbar, vbar);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType FileBrowser::MousePressEvent (const MouseEvent& event)
	{
		ModelIndex index;

		int rows = model_->GetRows();

		if(rows > 0) {
			int h = font_.GetHeight();	// the row height
			int total = rows * h;

			int i = 0;
			if(total > size().height()) {
				i = position().y() + GetVScrollBar()->value() - event.local_position().y();
			} else {	// no vbar
				i = position().y() + size().height() - event.local_position().y();
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
			Refresh();
		} else {

			if(!file_selected_.empty()) {
				file_selected_.clear();
				Refresh();
			}
		}

		clicked_.fire();
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

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0);

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		row_.reset(new GLArrayBuffer);
		row_->generate();
		row_->bind();
		row_->set_data(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		row_->reset();

		font_.set_color(Color(0xF0F0F0FF));
		font_.set_pen(font_.pen().x() + 4, std::abs(font_.GetDescender()));

		model_.reset(new FileSystemModel);

		ScrollBar* hbar = Manage(new ScrollBar(Horizontal));
		ScrollBar* vbar = Manage(new ScrollBar(Vertical));
		SetScrollBar(hbar, vbar);

		Load(getenv("PWD"));

		//events()->connect(hbar_moved(), this, &FileBrowser::OnHBarSlide);
		//events()->connect(vbar_moved(), this, &FileBrowser::OnVBarSlide);
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

