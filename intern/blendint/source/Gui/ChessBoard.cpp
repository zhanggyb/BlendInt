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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Gui/ChessBoard.hpp>

#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	ChessBoard::ChessBoard(size_t cell_size)
	: AbstractForm(),
	  m_vao(0),
	  light_elements_(0),
	  dark_elements_(0),
	  m_cell_size(cell_size)
	{
		set_size(0, 0);	// TODO: use the default size in Interface
		InitOnce();
	}

	ChessBoard::~ChessBoard()
	{
		glDeleteVertexArrays(1, &m_vao);
	}
	
	void ChessBoard::SetCellSize (size_t cell_size)
	{
		if(cell_size == 0) return;

		m_cell_size = cell_size;

		std::vector<GLfloat> vertices;
		std::vector<unsigned int> light_indices;
		std::vector<unsigned int> dark_indices;

		GenerateCheckerVertices(size(), m_cell_size, &vertices, &light_indices, &dark_indices);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo->bind();
		m_vbo->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_vbo->reset();

		m_light_ibo->bind();
		m_light_ibo->set_data(light_indices.size() * sizeof(GLuint), &light_indices[0]);
		m_light_ibo->reset();

		m_dark_ibo->bind();
		m_dark_ibo->set_data(light_indices.size() * sizeof(GLuint), &dark_indices[0]);
		m_dark_ibo->reset();

		glBindVertexArray(0);
	}
	
	void ChessBoard::PerformSizeUpdate(const Size& size)
	{
		std::vector<GLfloat> vertices;
		std::vector<unsigned int> light_indices;
		std::vector<unsigned int> dark_indices;

		GenerateCheckerVertices(size, m_cell_size, &vertices,
						&light_indices, &dark_indices);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo->bind();
		m_vbo->set_data(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		m_light_ibo->bind();
		m_light_ibo->set_data(light_indices.size() * sizeof(GLuint), &light_indices[0]);

		m_dark_ibo->bind();
		m_dark_ibo->set_data(dark_indices.size() * sizeof(GLuint), &dark_indices[0]);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
		GLElementArrayBuffer::reset();

		light_elements_ = light_indices.size();
		dark_elements_ = dark_indices.size();

		set_size(size);
	}

	void ChessBoard::Draw (float x, float y, short gamma) const
	{
		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = AbstractWindow::shaders->widget_triangle_program();
		program->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_POSITION), x, y);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_GAMMA), gamma);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		Color color(0x999999FF);
		glVertexAttrib4fv(AttributeColor, color.data());

		glEnableVertexAttribArray(0);

		m_vbo->bind();	// bind ARRAY BUFFER
		m_light_ibo->bind();	// bind ELEMENT ARRAY BUFFER

		glVertexAttribPointer(AttributeCoord, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawElements(GL_TRIANGLES, light_elements_,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		color = 0x666666FF;

		glVertexAttrib4fv(AttributeColor, color.data());

		m_dark_ibo->bind();	// bind ELEMENT ARRAY BUFFER

		glVertexAttribPointer(AttributeCoord, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawElements(GL_TRIANGLES, dark_elements_,
						GL_UNSIGNED_INT, BUFFER_OFFSET(0));

		m_dark_ibo->reset();
		m_vbo->reset();

		glDisableVertexAttribArray(0);

		program->reset();

		glBindVertexArray(0);
	}
	
	void ChessBoard::GenerateCheckerVertices (size_t num,
					size_t cell_size,
					std::vector<GLfloat>* vertices,
					std::vector<GLuint>* indices)
	{
		vertices->resize(num * num * 2);
		indices->resize((num - 1) * (num - 1) * 3 * 2);

		for(size_t i = 0; i < num; i++)
		{
			for(size_t j = 0; j < num; j++)
			{
				(*vertices)[(i * num + j) * 2 + 0] = static_cast<float>(j * cell_size);
				(*vertices)[(i * num + j) * 2 + 1] = static_cast<float>(i * cell_size);
			}
		}

		/*
#ifdef DEBUG
		printf("v size: %ld\n", vertices->size());
		for(size_t i = 0; i < vertices->size(); i += 2)
		{
			printf("%f %f\n", (*vertices)[i], (*vertices)[i+1]);
		}
#endif
		*/

		size_t row = 0;
		size_t pos = 0;
		for(size_t i = 0; i < indices->size() / 3; i++)
		{
			if(i % 2 == 0) {
				(*indices)[i * 3 + 0] = row * num + pos;
				(*indices)[i * 3 + 1] = row * num + pos + 1;
				(*indices)[i * 3 + 2] = (row + 1) * num + pos + 1;
			} else {
				(*indices)[i * 3 + 0] = row * num + pos;
				(*indices)[i * 3 + 1] = (row + 1) * num + pos + 1;
				(*indices)[i * 3 + 2] = (row + 1) * num + pos;
				pos++;
			}

			if(pos == (num - 1)) {
				row++;
				pos = 0;
			}
		}

		/*
#ifdef DEBUG
		printf("i size: %ld\n", indices->size());

		for(size_t i = 0; i < indices->size(); i += 3)
		{
			printf("%d %d %d\n", (*indices)[i], (*indices)[i+1], (*indices)[i + 2]);
		}
#endif
		 */

	}
	
	void ChessBoard::GenerateCheckerVertices (const Size& size,
					size_t cell_size,
					std::vector<GLfloat>* vertices,
					std::vector<GLuint>* light_indices,
					std::vector<GLuint>* dark_indices)
	{
	    int rh = size.width() % cell_size;
	    int rv = size.height() % cell_size;

	    size_t hnum = size.width() / cell_size + (rh > 0 ? 2 : 1);
	    size_t vnum = size.height() / cell_size + (rv > 0 ? 2 : 1);

	    size_t light_num = 0;
	    size_t dark_num = 0;

	    size_t temp = (hnum - 1) * (vnum - 1);
	    if(temp % 2 == 1) {
	    	light_num = temp / 2 + 1;
	    } else {
	    	light_num = temp / 2;
	    }
	    dark_num = temp - light_num;

	    vertices->resize(hnum * vnum * 2);
	    light_indices->resize(light_num * 2 * 3);
	    dark_indices->resize(dark_num * 2 * 3);

	    int x = 0, y = 0;
	    for(size_t i = 0; i < vnum; i++)
	    {
	        for(size_t j = 0; j < hnum; j++)
	        {
	            x = j * cell_size;
	            y = i * cell_size;

	            x = x > size.width() ? size.width() : x;
	            y = y > size.height() ? size.height() : y;

	            (*vertices)[(i * hnum + j) * 2 + 0] = static_cast<float>(x);
	            (*vertices)[(i * hnum + j) * 2 + 1] = static_cast<float>(y);
	        }
	    }

	    size_t row = 0;
	    size_t pos = 0;
	    for(size_t i = 0; i < light_indices->size() / 3; i++)
	    {
	        if(i % 2 == 0) {
	            (*light_indices)[i * 3 + 0] = row * hnum + pos;
	            (*light_indices)[i * 3 + 1] = row * hnum + pos + 1;
	            (*light_indices)[i * 3 + 2] = (row + 1) * hnum + pos + 1;
	        } else {
	            (*light_indices)[i * 3 + 0] = row * hnum + pos;
	            (*light_indices)[i * 3 + 1] = (row + 1) * hnum + pos + 1;
	            (*light_indices)[i * 3 + 2] = (row + 1) * hnum + pos;
	            pos += 2;
	        }

	        if(pos >= (hnum - 1)) {
	            row++;
	            pos = (row % 2 == 0) ? 0 : 1;
	        }
	    }

	    row = 0;
	    pos = 1;
	    for(size_t i = 0; i < dark_indices->size() / 3; i++)
	    {
	        if(i % 2 == 0) {
	            (*dark_indices)[i * 3 + 0] = row * hnum + pos;
	            (*dark_indices)[i * 3 + 1] = row * hnum + pos + 1;
	            (*dark_indices)[i * 3 + 2] = (row + 1) * hnum + pos + 1;
	        } else {
	            (*dark_indices)[i * 3 + 0] = row * hnum + pos;
	            (*dark_indices)[i * 3 + 1] = (row + 1) * hnum + pos + 1;
	            (*dark_indices)[i * 3 + 2] = (row + 1) * hnum + pos;
	            pos += 2;
	        }

	        if(pos >= (hnum - 1)) {
	            row++;
	            pos = (row % 2 == 0) ? 1 : 0;
	        }
	    }

//#ifdef DEBUG
//		printf("v size: %ld\n", vertices->size());
//		for(size_t i = 0; i < vertices->size(); i += 2)
//		{
//			printf("%f %f\n", (*vertices)[i], (*vertices)[i+1]);
//		}
//
//	    printf("light i size: %ld\n", light_indices->size());
//
//	    for(size_t i = 0; i < light_indices->size(); i += 3)
//	    {
//	        printf("%d %d %d\n", (*light_indices)[i], (*light_indices)[i+1], (*light_indices)[i + 2]);
//	    }
//
//	    printf("dark i size: %ld\n", dark_indices->size());
//
//	    for(size_t i = 0; i < dark_indices->size(); i += 3)
//	    {
//	        printf("%d %d %d\n", (*dark_indices)[i], (*dark_indices)[i+1], (*dark_indices)[i + 2]);
//	    }
//#endif

	}

	void ChessBoard::InitOnce()
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_light_ibo.reset(new GLElementArrayBuffer);
		m_dark_ibo.reset(new GLElementArrayBuffer);

		m_vbo->generate();
		m_vbo->bind();
		m_vbo->set_data(0, 0);
		m_vbo->reset();

		m_light_ibo->generate();
		m_light_ibo->bind();
		m_light_ibo->set_data(0, 0, 0);
		m_light_ibo->reset();

		m_dark_ibo->generate();
		m_dark_ibo->bind();
		m_dark_ibo->set_data(0, 0, 0);
		m_dark_ibo->reset();

		glBindVertexArray(0);
	}

}
