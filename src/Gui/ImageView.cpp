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

#include <algorithm>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ImageView.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ImageView::ImageView ()
	: AbstractScrollable()
	{
		InitializeImageView();
	}

	ImageView::~ImageView ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	bool ImageView::Open (const char* filename)
	{
		bool retval = false;

		Image image;

		if(image.Read(filename)) {
			texture_->Bind();

			switch(image.channels()) {
				case 3:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 3);
					texture_->SetImage(0, GL_RGB, image.width(),
					        image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				case 4:
					glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
					texture_->SetImage(0, GL_RGBA, image.width(),
					        image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					        image.pixels());
					break;

				default:
					break;
			}

			texture_->Reset();

			image_size_.set_width(image.width());
			image_size_.set_height(image.height());

			AdjustImageArea(size());

			retval = true;
		}

		return retval;
	}

	void ImageView::Load (const RefPtr<Image>& image)
	{
	}

	void ImageView::Clear()
	{

	}

	bool ImageView::IsExpandX () const
	{
		return true;
	}

	bool ImageView::IsExpandY () const
	{
		return true;
	}

	Size ImageView::GetPreferredSize () const
	{
		if(texture_ && glIsTexture(texture_->texture())) {
			return image_size_;
		}

		return Size(400, 300);
	}

	void ImageView::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			AdjustScrollBarGeometries(request.position()->x(), request.position()->y(), size().width(), size().height());
		}

		ReportPositionUpdate(request);
	}

	void ImageView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			background_->Bind();
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			tool.SetInnerBufferData(background_.get());
			background_->Reset();

			AdjustImageArea (*request.size());

			AdjustScrollBarGeometries(position().x(), position().y(), request.size()->width(), request.size()->height());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType ImageView::Draw (const Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.208f, 0.208f, 0.208f, 1.0f);

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		glm::vec3 pos(0.f);
		pos.x = position().x() + (size().width() - checkerboard_->size().width()) / 2.f;
		pos.y = position().y() + (size().height() - checkerboard_->size().height()) / 2.f;

		// draw checkerboard
		checkerboard_->Draw(pos);

		glActiveTexture(GL_TEXTURE0);
		texture_->Bind();

		if (texture_->GetWidth() > 0) {
			program = Shaders::instance->image_program();
			program->Use();
			glUniform3f(Shaders::instance->image_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
			glUniform1i(Shaders::instance->image_uniform_texture(), 0);
			glUniform1i(Shaders::instance->image_uniform_gamma(), 0);

			glBindVertexArray(vaos_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}

		glBindVertexArray(0);
		texture_->Reset();
		program->Reset();

		DispatchDrawEvent(hbar(), profile);
		DispatchDrawEvent(vbar(), profile);

		return Accept;
	}
	
	void ImageView::InitializeImageView ()
	{
		set_size(400, 300);

		checkerboard_.reset(new CheckerBoard(20));
		checkerboard_->Resize(size());

		texture_.reset(new GLTexture2D);
		texture_->Generate();
		texture_->Bind();
		texture_->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		texture_->SetMinFilter(GL_LINEAR);
		texture_->SetMagFilter(GL_LINEAR);
		texture_->Reset();

		glGenVertexArrays(2, vaos_);
		glBindVertexArray(vaos_[0]);

		background_.reset(new GLArrayBuffer);
		background_->Generate();
		background_->Bind();

		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);
		tool.SetInnerBufferData(background_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		glBindVertexArray(vaos_[1]);

		GLfloat vertices[] = {
			0.f, 0.f, 		0.f, 1.f,
			400.f, 0.f, 	1.f, 1.f,
			0.f, 300.f,		0.f, 0.f,
			400.f, 300.f,	1.f, 0.f
		};

		plane_.reset(new GLArrayBuffer);
		plane_->Generate();
		plane_->Bind();
		plane_->SetData(sizeof(vertices), vertices);

		glEnableVertexAttribArray(Shaders::instance->image_attrib_coord());
		glEnableVertexAttribArray(Shaders::instance->image_attrib_uv());
		glVertexAttribPointer(Shaders::instance->image_attrib_coord(), 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(Shaders::instance->image_attrib_uv(), 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		GLArrayBuffer::Reset();

		AdjustScrollBarGeometries(position().x(), position().y(), size().width(), size().height());
	}

	ResponseType ImageView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ImageView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ImageView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ImageView::MousePressEvent (const MouseEvent& event)
	{
		if(hbar()->Contain(event.position())) {
			return DispatchMousePressEvent(hbar(), event);
		} else if (vbar()->Contain(event.position())) {
			return DispatchMousePressEvent(vbar(), event);
		}

		return Ignore;
	}

	ResponseType ImageView::MouseReleaseEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseReleaseEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseReleaseEvent(vbar(), event);
		}

		return Ignore;
	}

	ResponseType ImageView::MouseMoveEvent (const MouseEvent& event)
	{
		if(hbar()->pressed()) {
			return DispatchMouseMoveEvent(hbar(), event);
		} else if (vbar()->pressed()) {
			return DispatchMouseMoveEvent(vbar(), event);
		}

		return Ignore;
	}

	void ImageView::AdjustImageArea(const Size& size)
	{
		if(image_size_.width() == 0 || image_size_.height() == 0) {
			checkerboard_->Resize(size);
			return;
		}

		int w = std::min(size.width(), image_size_.width());
		int h = std::min(size.height(), image_size_.height());

		if(h == 0) {
			w = 0;
		} else {
			float ratio = (float)w / h;
			float ref_ratio = (float)image_size_.width() / image_size_.height();
			if(ratio > ref_ratio) {
				w = h * ref_ratio;
			} else if (ratio < ref_ratio) {
				h = w / ref_ratio;
			}
		}

		checkerboard_->Resize(w, h);

		GLfloat x = (size.width() - w) / 2.f;
		GLfloat y = (size.height() - h) / 2.f;

		GLfloat vertices[] = {
			x, y,	0.f, 1.f,
			x + (GLfloat)w, y,		1.f, 1.f,
			x, y + (GLfloat)h,		0.f, 0.f,
			x + (GLfloat)w, y + (GLfloat)h,		1.f, 0.f
		};

		plane_->Bind();
		plane_->SetData(sizeof(vertices), vertices);
		plane_->Reset();
	}

}
