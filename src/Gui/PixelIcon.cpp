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

#include <vector>

#include <BlendInt/Gui/PixelIcon.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	PixelIcon::PixelIcon (int width, int height)
	: Icon(width, height),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		/*
			// x, 			y, 					u, 				v
			0.f, 			0.f, 				0.0, 			0.0,
			(GLfloat)width, 0.f,				1.0,			0.0,
			0.f, 			(GLfloat)height, 	0.0,			1.0,
			(GLfloat)width, (GLfloat)height, 	1.0,			1.0
		*/
		std::vector<GLfloat> vertices(16, 0.f);

		vertices[4] = (GLfloat)width;
		vertices[9] = (GLfloat)height;
		vertices[12] = (GLfloat)width;
		vertices[13] = (GLfloat)height;

		//vertices[2] = 0.f;
		//vertices[3] = 0.f;
		vertices[6] = 1.f;
		//vertices[7] = 0.f;
		//vertices[10] = 0.f;
		vertices[11] = 1.f;
		vertices[14] = 1.f;
		vertices[15] = 1.f;

		buffer_.reset(new GLArrayBuffer);
		buffer_->Generate();
		buffer_->Bind();
		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->image_attrib_coord());	// 0: Coord
		glEnableVertexAttribArray(Shaders::instance->image_attrib_uv());// 1: Texture UV
		glVertexAttribPointer(Shaders::instance->image_attrib_coord(), 2,
				GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->image_attrib_uv(), 2, GL_FLOAT,
				GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);

		buffer_->Reset();
	}

	PixelIcon::PixelIcon (int width, int height, const unsigned char* pixels, const GLfloat* uv)
	: Icon(width, height),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

		/*
			// x, 			y, 					u, 				v
			0.f, 			0.f, 				0.0, 			0.0,
			(GLfloat)width, 0.f,				1.0,			0.0,
			0.f, 			(GLfloat)height, 	0.0,			1.0,
			(GLfloat)width, (GLfloat)height, 	1.0,			1.0
		*/
		std::vector<GLfloat> vertices(16, 0.f);

		vertices[4] = (GLfloat)width;
		vertices[9] = (GLfloat)height;
		vertices[12] = (GLfloat)width;
		vertices[13] = (GLfloat)height;

		if(uv) {
			vertices[2] = *(uv + 0);
			vertices[3] = *(uv + 1);
			vertices[6] = *(uv + 2);
			vertices[7] = *(uv + 3);
			vertices[10] = *(uv + 4);
			vertices[11] = *(uv + 5);
			vertices[14] = *(uv + 6);
			vertices[15] = *(uv + 7);
		} else {
			//vertices[2] = 0.f;
			//vertices[3] = 0.f;
			vertices[6] = 1.f;
			//vertices[7] = 0.f;
			//vertices[10] = 0.f;
			vertices[11] = 1.f;
			vertices[14] = 1.f;
			vertices[15] = 1.f;
		}

		buffer_.reset(new GLArrayBuffer);
		buffer_->Generate();
		buffer_->Bind();
		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->image_attrib_coord());	// 0: Coord
		glEnableVertexAttribArray(Shaders::instance->image_attrib_uv());// 1: Texture UV
		glVertexAttribPointer(Shaders::instance->image_attrib_coord(), 2,
		        GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->image_attrib_uv(), 2, GL_FLOAT,
		        GL_FALSE, 4 * sizeof(GLfloat),
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);

		buffer_->Reset();

		texture_.reset(new GLTexture2D);
		texture_->Generate();
		texture_->Bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		texture_->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		texture_->Reset();
	}

	PixelIcon::PixelIcon (int width, int height,
					const RefPtr<GLTexture2D>& texture, const GLfloat* uv)
	: Icon(width, height),
	  vao_(0)
	{
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

		/*
			// x, 			y, 					u, 				v
			0.f, 			0.f, 				0.0, 			0.0,
			(GLfloat)width, 0.f,				1.0,			0.0,
			0.f, 			(GLfloat)height, 	0.0,			1.0,
			(GLfloat)width, (GLfloat)height, 	1.0,			1.0
		*/
		std::vector<GLfloat> vertices(16, 0.f);

		vertices[4] = (GLfloat)width;
		vertices[9] = (GLfloat)height;
		vertices[12] = (GLfloat)width;
		vertices[13] = (GLfloat)height;

		if(uv) {
			vertices[2] = *(uv + 0);
			vertices[3] = *(uv + 1);
			vertices[6] = *(uv + 2);
			vertices[7] = *(uv + 3);
			vertices[10] = *(uv + 4);
			vertices[11] = *(uv + 5);
			vertices[14] = *(uv + 6);
			vertices[15] = *(uv + 7);
		} else {
			//vertices[2] = 0.f;
			//vertices[3] = 0.f;
			vertices[6] = 1.f;
			//vertices[7] = 0.f;
			//vertices[10] = 0.f;
			vertices[11] = 1.f;
			vertices[14] = 1.f;
			vertices[15] = 1.f;
		}

		buffer_.reset(new GLArrayBuffer);
		buffer_->Generate();
		buffer_->Bind();
		buffer_->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);

		glEnableVertexAttribArray(Shaders::instance->image_attrib_coord());	// 0: Coord
		glEnableVertexAttribArray(Shaders::instance->image_attrib_uv());// 1: Texture UV
		glVertexAttribPointer(Shaders::instance->image_attrib_coord(), 2,
		        GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->image_attrib_uv(), 2, GL_FLOAT,
		        GL_FALSE, 4 * sizeof(GLfloat),
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		buffer_->Reset();
		glBindVertexArray(0);

		texture_ = texture;
	}

	PixelIcon::~PixelIcon ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void PixelIcon::SetPixels (int width, int height, const unsigned char* pixels, const GLfloat* uv)
	{
		assert(pixels);

		RefPtr<GLTexture2D> texture(new GLTexture2D);
		texture->Generate();
		texture->Bind();
		texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture->SetMinFilter(GL_LINEAR);
		texture->SetMagFilter(GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		texture->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		texture->Reset();

		texture_ = texture;

		buffer_->Bind();

		GLfloat* ptr = (GLfloat*)buffer_->Map(GL_READ_WRITE);

		*(ptr + 4) = (GLfloat)width;
		*(ptr + 9) = (GLfloat)height;
		*(ptr + 12) = (GLfloat)width;
		*(ptr + 13) = (GLfloat)height;

		if(uv) {
			*(ptr + 2) = *(uv + 0);
			*(ptr + 3) = *(uv + 1);
			*(ptr + 6) = *(uv + 2);
			*(ptr + 7) = *(uv + 3);
			*(ptr + 10) = *(uv + 4);
			*(ptr + 11) = *(uv + 5);
			*(ptr + 14) = *(uv + 6);
			*(ptr + 15) = *(uv + 7);
		} else {
			*(ptr + 2) = 0.f;
			*(ptr + 3) = 0.f;
			*(ptr + 6) = 1.f;
			*(ptr + 7) = 0.f;
			*(ptr + 10) = 0.f;
			*(ptr + 11) = 1.f;
			*(ptr + 14) = 1.f;
			*(ptr + 15) = 1.f;
		}

		buffer_->Unmap();
		buffer_->Reset();

		set_size(width, height);
	}

	void PixelIcon::SetTexture (int width, int height, const RefPtr<GLTexture2D>& texture,
					const GLfloat* uv)
	{
		if(!texture) return;
		if(texture->texture() == 0) return;

		texture_ = texture;

		buffer_->Bind();

		GLfloat* ptr = (GLfloat*)buffer_->Map(GL_READ_WRITE);

		*(ptr + 4) = (GLfloat)width;
		*(ptr + 9) = (GLfloat)height;
		*(ptr + 12) = (GLfloat)width;
		*(ptr + 13) = (GLfloat)height;

		if(uv) {
			*(ptr + 2) = *(uv + 0);
			*(ptr + 3) = *(uv + 1);
			*(ptr + 6) = *(uv + 2);
			*(ptr + 7) = *(uv + 3);
			*(ptr + 10) = *(uv + 4);
			*(ptr + 11) = *(uv + 5);
			*(ptr + 14) = *(uv + 6);
			*(ptr + 15) = *(uv + 7);
		} else {
			*(ptr + 2) = 0.f;
			*(ptr + 3) = 0.f;
			*(ptr + 6) = 1.f;
			*(ptr + 7) = 0.f;
			*(ptr + 10) = 0.f;
			*(ptr + 11) = 1.f;
			*(ptr + 14) = 1.f;
			*(ptr + 15) = 1.f;
		}

		buffer_->Unmap();
		buffer_->Reset();

		set_size(width, height);
	}

	void PixelIcon::Draw (const glm::vec3& pos, short gamma)
	{
		if(texture_) {
			RefPtr<GLSLProgram> program = Shaders::instance->image_program();
			program->Use();

			glUniform3fv(Shaders::instance->image_uniform_position(), 1, glm::value_ptr(pos));
			glUniform1i(Shaders::instance->image_uniform_gamma(), gamma);

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(Shaders::instance->image_uniform_texture(), 0);

			texture_->Bind();
			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
			texture_->Reset();

			program->Reset();

		}
	}
	
}
