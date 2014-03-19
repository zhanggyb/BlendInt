/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/CheckerBoard.hpp>

#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	CheckerBoard::CheckerBoard()
	: AbstractResizableForm(),
	  m_vao(0),
	  m_cell_size(100)
	{
		set_size(800, 600);	// TODO: use the default size in Interface
		InitOnce();
	}

	CheckerBoard::~CheckerBoard()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void CheckerBoard::SetCellSize (size_t size)
	{
		if(size == 0) return;
	}
	
	bool CheckerBoard::Update (const UpdateRequest& request)
	{
		return true;
	}
	
	void CheckerBoard::Draw (const glm::mat4& mvp)
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_form_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		float r = 0.1, g = 0.1, b = 0.1, a = 0.125;
		program->SetVertexAttrib4f("color", r, g, b, a);

		glEnableVertexAttribArray(0);

		m_vbo->Bind();	// bind ARRAY BUFFER
		m_ibo->Bind();	// bind ELEMENT ARRAY BUFFER

		glVertexAttribPointer(0, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawElements(GL_TRIANGLES, m_ibo->vertices() * 3,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		m_vbo->Reset();
		m_ibo->Reset();

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);
	}

	void CheckerBoard::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_ibo.reset(new GLElementArrayBuffer);

		size_t num = std::max(size().width(), size().height());
		num = num / m_cell_size + 1;

		std::vector<GLfloat> vertices;
		std::vector<unsigned int> indices;

		vertices.resize(num * num * 2);
		indices.resize((num - 1) * (num - 1) * 3 * 2);

		for(size_t i = 0; i < num; i++)
		{
			for(size_t j = 0; j < num; j++)
			{
				vertices[(i * num + j) * 2 + 0] = static_cast<float>(j * m_cell_size);
				vertices[(i * num + j) * 2 + 1] = static_cast<float>(i * m_cell_size);
			}
		}

#ifdef DEBUG
		printf("v size: %ld\n", vertices.size());
		for(size_t i = 0; i < vertices.size(); i += 2)
		{
			printf("%f %f\n", vertices[i], vertices[i+1]);
		}
#endif

		size_t row = 0;
		size_t pos = 0;
		for(size_t i = 0; i < indices.size() / 3; i++)
		{
			if(i % 2 == 0) {
				indices[i * 3 + 0] = row * num + pos;
				indices[i * 3 + 1] = row * num + pos + 1;
				indices[i * 3 + 2] = (row + 1) * num + pos + 1;
			} else {
				indices[i * 3 + 0] = row * num + pos;
				indices[i * 3 + 1] = (row + 1) * num + pos + 1;
				indices[i * 3 + 2] = (row + 1) * num + pos;
				pos++;
			}

			if(pos == (num - 1)) {
				row++;
				pos = 0;
			}
		}

#ifdef DEBUG
		printf("i size: %ld\n", indices.size());

		for(size_t i = 0; i < indices.size(); i += 3)
		{
			printf("%d %d %d\n", indices[i], indices[i+1], indices[i + 2]);
		}
#endif

		m_vbo->Generate();
		m_vbo->Bind();
		m_vbo->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_vbo->Reset();

		m_ibo->Generate();
		m_ibo->Bind();
		m_ibo->SetData(indices.size() / 3, 3 * sizeof(GLuint), &indices[0]);
		m_ibo->Reset();

		glBindVertexArray(0);
	}

}
