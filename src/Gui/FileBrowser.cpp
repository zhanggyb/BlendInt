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

	FileBrowser::FileBrowser ()
	: m_vao(0)
	{
		set_drop_shadow(true);

		InitializeFileListOnce();
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

		m_path = fs::path(pathname);

		try {
			if (fs::exists(m_path)) {

				if (fs::is_directory(m_path)) {

					int count = 0;
					fs::directory_iterator it(m_path);
					fs::directory_iterator it_end;
					while (it != it_end) {
						count++;
						it++;
					}

					int h = m_font.GetHeight();

					height = (count + 2) * h;	// count "." and ".."
					is_path = true;
				}
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

		if(is_path)
			Resize (size().width(), height);

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
		using Stock::Shaders;
		namespace fs = boost::filesystem;

		glm::vec3 pos((float) position().x(), (float) position().y(), 0.f);
		unsigned int i = 0;
		int h = size().height();
		h -= m_font.GetHeight();
		pos.y = position().y() + h;

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();
		program->SetUniform3fv("u_position", 1, glm::value_ptr(pos));
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4f("a_color", 0.475f, 0.475f, 0.475f, 0.75f);

		if (i == m_index) program->SetUniform1i("u_gamma", 25);
		else program->SetUniform1i("u_gamma", 15);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		program->Reset();

		m_font.Print(pos.x, pos.y - m_font.GetDescender(), String("."));
		i++;

		h -= m_font.GetHeight();
		pos.y = position().y() + h;

		program->Use();
		program->SetUniform3fv("u_position", 1, glm::value_ptr(pos));
		if(i == m_index) {
			program->SetUniform1i("u_gamma", 25);
		} else {
			program->SetUniform1i("u_gamma", 0);
		}

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		program->Reset();

		m_font.Print(pos.x, pos.y - m_font.GetDescender(), String(".."));
		i++;

		fs::path p(m_path);

		try {
			if (fs::exists(p)) {

				if (fs::is_regular_file(p)) {

					m_font.Print(pos.x, pos.y - m_font.GetDescender(),
									p.native());

				} else if (fs::is_directory(p)) {

					bool dark = false;

					fs::directory_iterator end_it;
					fs::directory_iterator it(p);

					while (it != end_it) {

						h -= m_font.GetHeight();

						pos.y = position().y() + h;

						program->Use();
						program->SetUniform3fv("u_position", 1, glm::value_ptr(pos));

						if(i == m_index) {
							program->SetUniform1i("u_gamma", 25);
						} else {
							if (dark) {
								program->SetUniform1i("u_gamma", 0);
							} else {
								program->SetUniform1i("u_gamma", 15);
							}
						}

						glBindVertexArray(m_vao);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						glBindVertexArray(0);

						program->Reset();

						m_font.Print(pos.x, pos.y - m_font.GetDescender(),
										it->path().native());
						dark = !dark;

						it++;
						i++;
					}
				}
			}
		} catch (const fs::filesystem_error& ex) {
			DBG_PRINT_MSG("Error: %s", ex.what());
		}

		return Accept;
	}

	ResponseType FileBrowser::MousePressEvent (const MouseEvent& event)
	{
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
		return Accept;
	}

	void FileBrowser::InitializeFileListOnce ()
	{
		namespace fs = boost::filesystem;

		std::string home = getenv("PWD");

		m_path = fs::path(home);

		try {
			if (fs::exists(m_path)) {

				if (fs::is_directory(m_path)) {

					int count = 0;
					fs::directory_iterator it(m_path);
					fs::directory_iterator it_end;
					while (it != it_end) {
						count++;
						it++;
					}

					int h = m_font.GetHeight();

					//unsigned total = std::max(300, count * h);

					set_size(400, (count + 2) * h);	// count "." and ".."

				} else {
					set_size(400, 300);
				}

			} else {
				set_size(400, 300);
			}
		} catch (const fs::filesystem_error& ex) {
			std::cerr << ex.what() << std::endl;
		}

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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		m_row->Reset();

		m_font.set_color(Color(0xF0F0F0FF));
	}

	bool FileBrowser::GetHighlightIndex(int y, unsigned int* index)
	{
		namespace fs = boost::filesystem;
		bool ret = false;

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

}

