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

#include <assert.h>

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

#include <BlendInt/Gui/AbstractForm.hpp>

#include <BlendInt/Utilities-inl.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	int AbstractForm::default_border_width = 1;

	const float AbstractForm::cornervec[WIDGET_CURVE_RESOLU][2] = {
					{ 0.0, 0.0 },
					{ 0.195, 0.02 },
					{ 0.383, 0.067 },
					{ 0.55,	0.169 },
					{ 0.707, 0.293 },
					{ 0.831, 0.45 },
					{ 0.924, 0.617 },
					{ 0.98, 0.805 },
					{ 1.0, 1.0 } };

	const Jitter AbstractForm::kJit;

	void AbstractForm::SetDefaultBorderWidth (int border)
	{
		default_border_width = border;
	}

	int AbstractForm::DefaultBorderWidth ()
	{
		return default_border_width;
	}

	AbstractForm::AbstractForm () :
					Object()
	{
	}

	AbstractForm::AbstractForm (const AbstractForm& orig)
	{
		m_size = orig.size();
	}

	AbstractForm::~AbstractForm ()
	{

	}

	void AbstractForm::DrawTriangleFan(const GLint attrib, const GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0,
						buffer->GetBufferSize()
										/ (2 * sizeof(GLfloat)));

		buffer->Reset();
	}

	void AbstractForm::DrawTriangleFan(const RefPtr<GLSLProgram>& program, const glm::mat4& mvp, const GLint attrib, GLArrayBuffer* buffer)
	{
		glm::mat4 jitter_matrix;

		buffer->Bind();

		glVertexAttribPointer(attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		for (Jitter::const_iterator it = kJit.begin(); it != kJit.end(); it++) {
			jitter_matrix = glm::translate(glm::mat4(1.0),
							glm::vec3((*it), 0.f));
			program->SetUniformMatrix4fv("MVP", 1, GL_FALSE,
							glm::value_ptr(mvp * jitter_matrix));
			glDrawArrays(GL_TRIANGLE_FAN, 0,
							buffer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));
		}

		buffer->Reset();
	}


	void AbstractForm::DrawShadedTriangleFan(const GLint coord, const GLint color, GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(coord, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(0)					 // offset of first element
							  );

		glVertexAttribPointer(color, // attribute
							  4,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  sizeof(GLfloat) * 6,				 // stride
							  BUFFER_OFFSET(2 * sizeof(GLfloat))					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_FAN, 0, buffer->GetBufferSize() / (6 * sizeof(GLfloat)));

		buffer->Reset();
	}
	
	void AbstractForm::Resize (int width, int height)
	{
		if(m_size.width() == width && m_size.height() == height) return;

		Size new_size (width, height);

		UpdateGeometry(UpdateRequest(FormSize, &new_size));
		set_size(width, height);
	}
	
	void AbstractForm::Resize (const Size& size)
	{
		if(m_size == size) return;

		UpdateGeometry(UpdateRequest(FormSize, &size));
		set_size(size);
	}

	void AbstractForm::DrawTriangleStrip (
					const GLint attrib,
					GLArrayBuffer* buffer)
	{
		buffer->Bind();

		glVertexAttribPointer(attrib, // attribute
							  2,			// number of elements per vertex, here (x,y)
							  GL_FLOAT,			 // the type of each element
							  GL_FALSE,			 // take our values as-is
							  0,				 // no extra data between each position
							  0					 // offset of first element
							  );

		glDrawArrays(GL_TRIANGLE_STRIP, 0,
							buffer->GetBufferSize()
											/ (2 * sizeof(GLfloat)));

		buffer->Reset();
	}

}
