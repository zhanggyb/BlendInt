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

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <BlendInt/Gui/PixelIcon.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	PixelIcon::PixelIcon (unsigned int width, unsigned int height)
	: AbstractIcon(width, height),
	  vao_(0)
	{
		CreateVertexArray(width, height);
	}

	PixelIcon::PixelIcon (unsigned int width, unsigned int height, const unsigned char* pixels, const GLfloat* uv)
	: AbstractIcon(width, height),
	  vao_(0)
	{
		CreateVertexArray(width, height, uv);

		texture_.reset(new GLTexture2D);
		texture_->generate();
		texture_->bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		texture_->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		texture_->reset();
	}

	PixelIcon::PixelIcon (unsigned int width, unsigned int height,
					const RefPtr<GLTexture2D>& texture, const GLfloat* uv)
	: AbstractIcon(width, height),
	  vao_(0)
	{
		CreateVertexArray(width, height, uv);

		texture_ = texture;
	}

	PixelIcon::~PixelIcon ()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	void PixelIcon::SetPixels (unsigned int width, unsigned int height, const unsigned char* pixels, const GLfloat* uv)
	{
		assert(pixels);

		RefPtr<GLTexture2D> texture(new GLTexture2D);
		texture->generate();
		texture->bind();
		texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture->SetMinFilter(GL_LINEAR);
		texture->SetMagFilter(GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		texture->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		texture->reset();

		texture_ = texture;

		vbo_->bind();

		GLfloat* ptr = (GLfloat*)vbo_->map(GL_READ_WRITE);

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

		vbo_->unmap();
		vbo_->reset();

		set_size(width, height);
	}

	void PixelIcon::SetTexture (unsigned int width, unsigned int height, const RefPtr<GLTexture2D>& texture,
					const GLfloat* uv)
	{
		if(!texture) return;
		if(texture->id() == 0) return;

		texture_ = texture;

		vbo_->bind();

		GLfloat* ptr = (GLfloat*)vbo_->map(GL_READ_WRITE);

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

		vbo_->unmap();
		vbo_->reset();

		set_size(width, height);
	}

	void PixelIcon::Draw (float x, float y) const
	{
		Draw (x, y, 0);
	}

	void PixelIcon::Draw (float x, float y, short gamma) const
	{
		AbstractWindow::shaders->widget_image_program()->use();

		glUniform2f(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_POSITION), x, y);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_GAMMA), gamma);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_TEXTURE), 0);

		if(texture_) {
			texture_->bind();
			glBindVertexArray(vao_);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	void PixelIcon::PerformSizeUpdate(const Size& size)
	{
		// Pixel Icon cannot resized
	}

	void PixelIcon::CreateVertexArray (unsigned int width, unsigned int height, const GLfloat* uv)
	{
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		float x = width / 2.f;
		float y = height / 2.f;

		// (x, y) (u, v)
		std::vector<GLfloat> v(16, 0.f);

		v[0] = -x;
		v[1] = -y;

		v[4] = x;
		v[5] = -y;

		v[8] = -x;
		v[9] = y;

		v[12] = x;
		v[13] = y;

		if(uv == 0) {

			v[2] = 0.f;
			v[3] = 0.f;

			v[6] = 1.f;
			v[7] = 0.f;

			v[10] = 0.f;
			v[11] = 1.f;

			v[14] = 1.f;
			v[15] = 1.f;

		} else {

			v[2] = *(uv + 0);
			v[3] = *(uv + 1);

			v[6] = *(uv + 2);
			v[7] = *(uv + 3);

			v[10] = *(uv + 4);
			v[11] = *(uv + 5);

			v[14] = *(uv + 6);
			v[15] = *(uv + 7);
		}

		vbo_.reset(new GLArrayBuffer);
		vbo_->generate();
		vbo_->bind();
		vbo_->set_data(sizeof(GLfloat) * v.size(), &v[0]);

		glEnableVertexAttribArray(
		        AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_COORD));// 0: Coord
		glEnableVertexAttribArray(
		        AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_UV));// 1: Texture UV
		glVertexAttribPointer(
		        AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_COORD), 2,
		        GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), BUFFER_OFFSET(0));
		glVertexAttribPointer(
		        AbstractWindow::shaders->location(Shaders::WIDGET_IMAGE_UV), 2,
		        GL_FLOAT,
		        GL_FALSE, 4 * sizeof(GLfloat),
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		vbo_->reset();
	}

}
