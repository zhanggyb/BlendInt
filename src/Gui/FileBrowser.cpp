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
	: AbstractItemView(), vao_(0)
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
		AdjustScrollBarGeometries(position().x(), position().y(), size().width(), size().height());
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
		return ModelIndex();
	}

	ResponseType FileBrowser::Draw (const RedrawEvent& event)
	{
		int y = position().y() + size().height();
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

			if(i % 2 == 0) {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
			} else {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
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

	ResponseType FileBrowser::MousePressEvent (const MouseEvent& event)
	{
		if (hbar()->visiable() && hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->visiable() && vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

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

	void FileBrowser::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(), request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
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

	void FileBrowser::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		namespace fs = boost::filesystem;

		if(request.target() == this) {

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

		model_.reset(new FileSystemModel);
		model_->Load(getenv("PWD"));

		int h = font_.GetHeight();
		font_.set_color(Color(0xF0F0F0FF));
		font_.set_pen(font_.pen().x() + 4, std::abs(font_.GetDescender()));

		// total height
		h = model_->GetRows() * h;

		if(h > size().height()) {
			vbar()->SetVisible(true);
			vbar()->SetSliderPercentage(size().height() * 100 / h);
		} else {
			vbar()->SetVisible(false);
		}
		hbar()->SetVisible(false);

		AdjustScrollBarGeometries(position().x(), position().y(), size().width(), size().height());

		events()->connect(hbar_moved(), this, &FileBrowser::OnHBarSlide);
		events()->connect(vbar_moved(), this, &FileBrowser::OnVBarSlide);
	}

	void FileBrowser::OnHBarSlide (int val)
	{
		DBG_PRINT_MSG("val: %d", val);
	}

	void FileBrowser::OnVBarSlide (int val)
	{
		DBG_PRINT_MSG("val: %d", val);
	}

}

