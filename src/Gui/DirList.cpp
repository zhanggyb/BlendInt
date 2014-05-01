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

#include <BlendInt/Gui/DirList.hpp>

#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	DirList::DirList ()
	: m_vao(0)
	{
		set_expand_x(true);
		set_expand_y(true);

		glGenVertexArrays(1, &m_vao);
		InitializeFileListOnce();
	}

	DirList::~DirList ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	ResponseType DirList::Draw (const RedrawEvent& event)
	{
		namespace fs = boost::filesystem;

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();

		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(
						event.projection_matrix() * event.view_matrix(), pos);

		unsigned int i = 0;

		int h = size().height();
		glm::mat4 local_mvp;

		h -= m_font.get_height();
		local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));

		glBindVertexArray(m_vao);
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
		glEnableVertexAttribArray(0);
		DrawTriangleStrip(0, m_row.get());
		glDisableVertexAttribArray(0);
		program->Reset();
		glBindVertexArray(0);

		m_font.Print(mvp, 0, h - m_font.get_descender(),
							".");
		i++;

		h -= m_font.get_height();
		local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));

		glBindVertexArray(m_vao);
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
		glEnableVertexAttribArray(0);
		DrawTriangleStrip(0, m_row.get());
		glDisableVertexAttribArray(0);
		program->Reset();
		glBindVertexArray(0);

		m_font.Print(mvp, 0, h - m_font.get_descender(),
							"..");
		i++;

		fs::path p(m_path);

		try {
			if (fs::exists(p)) {

				if (fs::is_regular_file(p)) {

					m_font.Print(mvp, 0, h - m_font.get_descender(),
									p.native());

				} else if (fs::is_directory(p)) {

					bool dark = false;

					fs::directory_iterator end_it;
					fs::directory_iterator it(p);

					while (it != end_it) {

						h -= m_font.get_height();

						local_mvp = glm::translate(mvp, glm::vec3(0.f, h, 0.f));
						glBindVertexArray(m_vao);

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

						glEnableVertexAttribArray(0);

						DrawTriangleStrip(0, m_row.get());

						glDisableVertexAttribArray(0);

						program->Reset();

						glBindVertexArray(0);

						m_font.Print(mvp, 0, h - m_font.get_descender(),
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
	
	ResponseType DirList::MousePressEvent (const MouseEvent& event)
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
	
	ResponseType DirList::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	void DirList::InitializeFileListOnce ()
	{
		// Set size:
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

					int h = m_font.get_height();

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

		glBindVertexArray(m_vao);

		GLfloat verts[] = {
						0.f, 0.f,
						(GLfloat)size().width(), 0.f,
						0.f, (GLfloat)m_font.get_height(),
						(GLfloat)size().width(), (GLfloat)m_font.get_height()
		};

		m_row.reset(new GLArrayBuffer);
		m_row->Generate();
		m_row->Bind();
		m_row->SetData(sizeof(verts), verts);
		m_row->Reset();

		glBindVertexArray(0);
	}

	bool DirList::GetHighlightIndex(int y, unsigned int* index)
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
		unsigned cell_height = m_font.get_height();
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
