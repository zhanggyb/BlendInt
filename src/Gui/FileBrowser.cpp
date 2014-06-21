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
		InitializeFileListOnce();
	}

	FileBrowser::~FileBrowser ()
	{
		glDeleteVertexArrays(1, &m_vao);
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

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(
						event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();

		unsigned int i = 0;

		int h = size().height();
		glm::mat4 local_mvp;

		h -= m_font.GetHeight();
		local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
						glm::value_ptr(local_mvp));
		program->SetUniform1i("AA", 0);
		if(i == m_index) {
			program->SetVertexAttrib4f("Color", 0.475f,
							0.475f, 0.475f, 0.75f);
		} else {
			program->SetVertexAttrib4f("Color", 0.375f,
							0.375f, 0.375f, 0.75f);
		}

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		program->Reset();

		m_font.Print(mvp, 0, h - m_font.GetDescender(),	String("."));
		i++;

		h -= m_font.GetHeight();
		local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
						glm::value_ptr(local_mvp));
		program->SetUniform1i("AA", 0);
		if(i == m_index) {
			program->SetVertexAttrib4f("Color", 0.475f,
							0.475f, 0.475f, 0.75f);
		} else {
			program->SetVertexAttrib4f("Color", 0.325f,
							0.325f, 0.325f, 0.75f);
		}

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		program->Reset();

		m_font.Print(mvp, 0, h - m_font.GetDescender(), String(".."));
		i++;

		fs::path p(m_path);

		try {
			if (fs::exists(p)) {

				if (fs::is_regular_file(p)) {

					m_font.Print(mvp, 0, h - m_font.GetDescender(),
									p.native());

				} else if (fs::is_directory(p)) {

					bool dark = false;

					fs::directory_iterator end_it;
					fs::directory_iterator it(p);

					while (it != end_it) {

						h -= m_font.GetHeight();

						local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));

						program->Use();

						program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
										glm::value_ptr(local_mvp));
						program->SetUniform1i("AA", 0);

						if(i == m_index) {
							program->SetVertexAttrib4f("Color", 0.475f,
											0.475f, 0.475f, 0.75f);
						} else {
							if (dark) {
								program->SetVertexAttrib4f("Color", 0.325f,
												0.325f, 0.325f, 0.75f);
							} else {
								program->SetVertexAttrib4f("Color", 0.375f,
												0.375f, 0.375f, 0.75f);
							}
						}

						glBindVertexArray(m_vao);
						glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
						glBindVertexArray(0);

						program->Reset();

						m_font.Print(mvp, 0, h - m_font.GetDescender(),
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
					m_path = parent;
				}
			}

			if (m_index >= 2) {

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

							if (fs::is_directory(it->path())) {
								m_path = it->path();
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

					unsigned total = std::max(300, count * h);

					set_size(400, total);

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
