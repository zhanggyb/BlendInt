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
	: AbstractScrollable(), m_vao(0)
	{
		set_drop_shadow(true);
		set_size(400, 300);

		InitializeFileBrowserOnce();

		hbar()->SetVisible(false);
		vbar()->SetVisible(false);

		events()->connect(hbar_moved(), this, &FileBrowser::OnHBarSlide);
		events()->connect(vbar_moved(), this, &FileBrowser::OnVBarSlide);
	}

	FileBrowser::~FileBrowser ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool FileBrowser::Open (const std::string& pathname)
	{
		namespace fs = boost::filesystem;
		bool is_path = false;
		int height = 0;
		int row_height = m_font.GetHeight();

		m_path = fs::path(pathname);

		try {
			if (fs::exists(m_path)) {

				if (fs::is_directory(m_path)) {

					int count = 0;
					fs::directory_iterator it(m_path);
					fs::directory_iterator end;
					while (it != end) {
						count++;
						it++;
					}

					height = (count + 2) * row_height;	// count "." and ".."

					is_path = true;
				}
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		if(height > size().height()) {
			vbar()->SetVisible(true);
			vbar()->SetSliderPercentage(size().height() * 100 / height);
			vbar()->SetMinimum(size().height());
			vbar()->SetMaximum(height);

			DBG_PRINT_MSG("widget size: %d, minimum: %d", size().height(), vbar()->minimum());
			DBG_PRINT_MSG("list size: %d, maximum: %d", height, vbar()->maximum());

		} else {
			vbar()->SetVisible(false);
		}

		return is_path;
	}

	bool FileBrowser::IsExpandX() const
	{
		return true;
	}

	bool FileBrowser::IsExpandY() const
	{
		return true;
	}

	ResponseType FileBrowser::Draw (const RedrawEvent& event)
	{
		namespace fs = boost::filesystem;

        glEnable(GL_SCISSOR_TEST);
        glScissor(position().x(),
        		position().y(),
        		size().width(),
                size().height());

		glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);

		int row_height = m_font.GetHeight();
		pos.y += size().height();

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.475f, 0.475f, 0.475f, 0.75f);

		// draw background
		unsigned int i = 0;
		while (pos.y > position().y()) {

			pos.y -= row_height;

			glUniform3fv(Shaders::instance->triangle_uniform_position(), 1, glm::value_ptr(pos));

			if (i == m_index) {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 25);
			} else {
				if (i % 2 == 0) {
					glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
				} else {
					glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
				}
			}

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			i++;
		}

		program->Reset();

		if(m_path.empty()) return Accept;

		pos.x += 4;	// move 4 right
		pos.y = position().y() + size().height();

		pos.y -= row_height;
		m_font.Print(pos.x, pos.y - m_font.GetDescender(), String("."));

		pos.y -= row_height;
		m_font.Print(pos.x, pos.y - m_font.GetDescender(), String(".."));

		pos.y -= row_height;
		try {
			if (fs::exists(m_path)) {

				if (fs::is_regular_file(m_path)) {

					m_font.Print(pos.x, pos.y - m_font.GetDescender(),
							m_path.native());

				} else if (fs::is_directory(m_path)) {

					fs::directory_iterator end_it;
					fs::directory_iterator it(m_path);

					while (it != end_it) {

						m_font.Print(pos.x, pos.y - m_font.GetDescender(),
										it->path().filename().native());

						pos.y -= row_height;
						it++;
					}
				}
			}
		} catch (const fs::filesystem_error& ex) {
			DBG_PRINT_MSG("Error: %s", ex.what());
		}

        glDisable(GL_SCISSOR_TEST);

        DispatchDrawEvent(hbar(), event);
        DispatchDrawEvent(vbar(), event);

		return Accept;
	}

	ResponseType FileBrowser::MousePressEvent (const MouseEvent& event)
	{
		if (hbar()->visiable() && hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->visiable() && vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

		namespace fs = boost::filesystem;

		unsigned int index;
		bool valid = GetHighlightIndex(event.position().y(), &index);

		if(valid) {
			m_index = index;

			if(m_index == 1){	// ".." pressed
				fs::path parent = m_path.parent_path();

				if(fs::is_directory(parent)) {
					DBG_PRINT_MSG("%s", "parent path");
					//m_path = parent;
				}

				m_file_selected.clear();
			} else if (m_index >= 2) {

				unsigned int i = 2;
				try {
					if (fs::exists(m_path)) {

						if (fs::is_directory(m_path)) {

							fs::directory_iterator it(m_path);
							fs::directory_iterator it_end;
							while (it != it_end) {
								if (i == m_index)
									break;
								i++;
								it++;
							}

							if(fs::is_regular(it->path())) {
								DBG_PRINT_MSG("file %s selected", it->path().native().c_str());
								m_file_selected = it->path().string();
							} else {
								m_file_selected.clear();

								if (fs::is_directory(it->path())) {
									DBG_PRINT_MSG("path %s selected", it->path().native().c_str());
									//m_path = it->path();
								}

							}

						}
					}
				} catch (const fs::filesystem_error& ex) {
					std::cerr << ex.what() << std::endl;
				}
			}

			Refresh();
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
		if(request.target() == this) {

			GLfloat row_height = (GLfloat)m_font.GetHeight();
			GLfloat verts[] = {
							0.f, 0.f,
							(GLfloat)request.size()->width(), 0.f,
							0.f, row_height,
							(GLfloat)request.size()->width(), row_height
			};

			m_row->Bind();
			m_row->SetData(sizeof(verts), verts);
			m_row->Reset();

		}

		ReportSizeUpdate(request);
	}

	void FileBrowser::InitializeFileBrowserOnce ()
	{
		GLfloat row_height = (GLfloat)m_font.GetHeight();
		GLfloat verts[] = {
						0.f, 0.f,
						(GLfloat)size().width(), 0.f,
						0.f, row_height,
						(GLfloat)size().width(), row_height
		};

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_row.reset(new GLArrayBuffer);
		m_row->Generate();
		m_row->Bind();
		m_row->SetData(sizeof(verts), verts);

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		m_row->Reset();

		m_font.set_color(Color(0xF0F0F0FF));
	}

	bool FileBrowser::GetHighlightIndex(int y, unsigned int* index)
	{
		namespace fs = boost::filesystem;
		bool ret = false;

		if(m_path.empty()) return ret;

		int h = position().y() + size().height() - y;

		unsigned int count = 0;
		try {
			if (fs::exists(m_path)) {

				if (fs::is_directory(m_path)) {

					fs::directory_iterator it(m_path);
					fs::directory_iterator it_end;
					while (it != it_end) {
						count++;
						it++;
					}

				} else {
					count = 1;
				}

			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		count += 2;	// for "." and ".."
		unsigned int out = 0;
		unsigned cell_height = m_font.GetHeight();
		out = h / cell_height;

		if (out >= count) {
			out = 0;
		} else {
			ret = true;
		}

		*index = out;
		return ret;
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

