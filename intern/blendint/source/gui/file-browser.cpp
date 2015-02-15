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

#include <boost/filesystem.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <gui/file-browser.hpp>

#include <gui/abstract-window.hpp>
#include <gui/text.hpp>

namespace BlendInt {

	namespace fs = boost::filesystem;

	FileBrowser::FileBrowser ()
	: AbstractItemView(),
	  history_index_(0),
	  highlight_index_(-1)
	{
		set_size(400, 300);

		InitializeFileBrowserOnce();
	}

	FileBrowser::~FileBrowser ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool FileBrowser::Open (const std::string& pathname)
	{
		assert(model_);

		bool retval = model_->Load(pathname);

		if(retval) {

			if(!history_.empty()) {
				while(history_index_ < (history_.size() - 1)) {
					history_.pop_back();
				}
			}

			pathname_ = pathname;

			history_.push_back(pathname_);
			history_index_ = history_.size() - 1;

			highlight_index_ = -1;

			RequestRedraw();
		}

		return retval;
	}

	bool FileBrowser::OpenParent ()
	{
		fs::path p;

		if(!history_.empty()) {

			if(history_index_ == 0) {

				p = history_[history_index_];
				p = p.parent_path();

				if(p.native().empty()) return false;

				if(fs::exists(p))
					history_.push_front(p.native());

			} else {

				history_index_--;
				p = history_[history_index_];

				if(!fs::exists(p))
					return false;

			}

		} else {
			p = pathname_;
			p = p.parent_path();
			if(p.native().empty() || (!fs::exists(p))) {
				return false;
			}
		}

		pathname_ = p.native();

		model_->Load(pathname_);
		RequestRedraw();
		return true;
	}

	bool FileBrowser::GoBackward ()
	{
		if((!history_.empty()) && history_index_ > 0) {

			history_index_--;
			bool retval = model_->Load(history_[history_index_]);

			if(retval) {
				pathname_ = history_[history_index_];
				highlight_index_ = -1;
				RequestRedraw();
				return true;
			}

		}

		return false;
	}

	bool FileBrowser::GoForward ()
	{
		if((!history_.empty()) && (history_index_ < (history_.size() - 1))) {

			history_index_++;
			bool retval = model_->Load(history_[history_index_]);

			if(retval) {
				pathname_ = history_[history_index_];
				highlight_index_ = -1;
				RequestRedraw();
				return true;
			}

		}

		return false;
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

		int rows = model_->GetRowCount();

		if(rows > 0) {
			int h = font_.height();	// the row height
			int total = rows * h;

			int i = 0;
			if(total > size().height()) {
				i = position().y() - point.y();
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

	Response FileBrowser::Draw (AbstractWindow* context)
	{
		AbstractWindow::shaders->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR),
				1, AbstractWindow::theme->box().inner.data());

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		context->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		context->EndPushStencil();

		AbstractWindow::shaders->widget_simple_triangle_program()->use();

		glUniform4fv(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR), 1,
				AbstractWindow::theme->box().inner_sel.data());

		glBindVertexArray(vaos_[1]);

		int y = size().height();
		int h = font_.height();
		int i = 0;
		while(y > 0) {
			y -= h;

			glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION), 0.f, (float)y);

			if(i == highlight_index_) {
				glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA), -35);
			} else {
				if(i % 2 == 0) {
					glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA), 0);
				} else {
					glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA), 15);
				}
			}

			glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
			i++;
		}

		if(GetModel()) {

			ModelIndex index = GetModel()->GetRootIndex();
			index = index.GetChildIndex(0, 0);

			y = size().height();

			int calib = 0;
			float ty = 0.f;

			// A workaround for Adobe Source Han Sans
			calib = font_.ascender() - font_.descender();
			if(calib < font_.height()) {
				calib = (font_.height() - calib - 1) / 2;
			} else {
				calib = 0;
			}

			while(index.IsValid()) {
				y -= h;
				ty = y - font_.descender() + calib;
				index.GetRawData()->Draw(0.f, ty);
				index = index.GetDownIndex();
			}

		}

		AbstractWindow::shaders->widget_inner_program()->use();

		context->BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		context->EndPopStencil();

		return Finish;
	}

	void FileBrowser::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			GLfloat row_height = (GLfloat)font_.height();

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> row_verts;

			if (AbstractWindow::theme->box().shaded) {
				GenerateVertices(size(),
						0.f,
						round_type(),
						round_radius(),
						Vertical,
						AbstractWindow::theme->box().shadetop,
						AbstractWindow::theme->box().shadedown,
						&inner_verts,
						0);
			} else {
				GenerateVertices(
						size(),
						0.f,
						round_type(),
						round_radius(),
						&inner_verts,
						0);
			}

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

			GenerateVertices(Size(size().width(), row_height),
					0.f,
					RoundNone,
					0.f,
					&row_verts,
					0
					);

			glBindVertexArray(vaos_[1]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * row_verts.size(), &row_verts[0]);

			buffer_.reset();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	Response FileBrowser::PerformMousePress (AbstractWindow* context)
	{
		ModelIndex index;

		int rows = model_->GetRowCount();

		if(rows > 0) {
			int h = font_.height();	// the row height

			int i = 0;
			Point local_position = context->GetCursorPosition() - context->active_frame()->GetAbsolutePosition(this);
			// TODO: count offset

			i = size().height() - local_position.y();

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
			Text* t = dynamic_cast<Text*>(index.GetData().get());
			file_selected_ = t->text();
			//DBG_PRINT_MSG("index item: %s", ConvertFromString(file_selected_).c_str());
			RequestRedraw();
		} else {

			if(!file_selected_.empty()) {
				file_selected_.clear();
				RequestRedraw();
			}
		}

		selected_.fire();
		return Finish;
	}

	void FileBrowser::InitializeFileBrowserOnce ()
	{
		GLfloat row_height = (GLfloat)font_.height();

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> row_verts;

		if (AbstractWindow::theme->box().shaded) {
			GenerateVertices(size(),
					0.f,
					round_type(),
					round_radius(),
					Vertical,
					AbstractWindow::theme->box().shadetop,
					AbstractWindow::theme->box().shadedown,
					&inner_verts,
					0);
		} else {
			GenerateVertices(
					size(),
					0.f,
					round_type(),
					round_radius(),
					&inner_verts,
					0);
		}

		buffer_.generate();
		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		GenerateVertices(Size(size().width(), row_height),
				0.f,
				RoundNone,
				0.f,
				&row_verts,
				0
				);

		glBindVertexArray(vaos_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * row_verts.size(), &row_verts[0]);

		glEnableVertexAttribArray(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COORD));
		glVertexAttribPointer(AbstractWindow::shaders->location(Shaders::WIDGET_SIMPLE_TRIANGLE_COORD), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();

		model_.reset(new FileSystemModel);

		// Load(getenv("PWD"));
	}

}

