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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ToolButton.hpp>
#include <BlendInt/Gui/VertexTool.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

#include <BlendInt/Core/Image.hpp>

namespace BlendInt {

	using Stock::Shaders;

	static const unsigned char pixel_data[24 * 24 * 4 + 1] = {
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0g\313\25Cg\313\25Cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg"
	  "\313\25cg\313\25cg\313\25cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\246"
	  "g\313\25\246g\313\25\246g\313\25\213g\313\25cg\313\25Cg\313\25C\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0g\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\272g\313\25\311"
	  "g\313\25\325g\313\25\325g\313\25\311g\313\25\272g\313\25\246g\313\25\213"
	  "g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\246g\313\25\311g"
	  "\313\25\337g\313\25\355g\313\25\363g\313\25\363g\313\25\355g\313\25\337g"
	  "\313\25\311g\313\25\246g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246"
	  "g\313\25\311g\313\25\347g\313\25\365g\313\25\366g\313\25\367g\313\25\367"
	  "g\313\25\366g\313\25\365g\313\25\347g\313\25\311g\313\25\246g\313\25cg\313"
	  "\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313"
	  "\25Cg\313\25\213g\313\25\272g\313\25\337g\313\25\365g\313\25\367g\313\25"
	  "\370g\313\25\371g\313\25\371g\313\25\370g\313\25\367g\313\25\365g\313\25"
	  "\337g\313\25\272g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246g\313\25\311g\313\25\355"
	  "g\313\25\366g\313\25\370g\313\25\371g\313\25\372g\313\25\372g\313\25\371"
	  "g\313\25\370g\313\25\366g\313\25\355g\313\25\311g\313\25\246g\313\25cg\313"
	  "\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313"
	  "\25\246g\313\25\325g\313\25\363g\313\25\367g\313\25\371g\313\25\372g\313"
	  "\25\372g\313\25\372g\313\25\372g\313\25\371g\313\25\367g\313\25\363g\313"
	  "\25\325g\313\25\246g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\246g\313\25\325g\313\25\363g\313\25"
	  "\367g\313\25\371g\313\25\372g\313\25\372g\313\25\372g\313\25\372g\313\25"
	  "\371g\313\25\367g\313\25\363g\313\25\325g\313\25\246g\313\25cg\313\25C\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25"
	  "\246g\313\25\311g\313\25\355g\313\25\366g\313\25\370g\313\25\371g\313\25"
	  "\372g\313\25\372g\313\25\371g\313\25\370g\313\25\366g\313\25\355g\313\25"
	  "\311g\313\25\246g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\272g\313\25\337g\313\25\365"
	  "g\313\25\367g\313\25\370g\313\25\371g\313\25\371g\313\25\370g\313\25\367"
	  "g\313\25\365g\313\25\337g\313\25\272g\313\25\213g\313\25C\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25"
	  "\246g\313\25\311g\313\25\347g\313\25\365g\313\25\366g\313\25\367g\313\25"
	  "\367g\313\25\366g\313\25\365g\313\25\347g\313\25\311g\313\25\246g\313\25"
	  "cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25\213g\313\25\246g\313\25\311g\313\25\337g\313"
	  "\25\355g\313\25\363g\313\25\363g\313\25\355g\313\25\337g\313\25\311g\313"
	  "\25\246g\313\25\213g\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg\313\25cg\313\25\213g\313\25"
	  "\246g\313\25\272g\313\25\311g\313\25\325g\313\25\325g\313\25\311g\313\25"
	  "\272g\313\25\246g\313\25\213g\313\25cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0g\313\25Cg"
	  "\313\25Cg\313\25cg\313\25\213g\313\25\246g\313\25\246g\313\25\246g\313\25"
	  "\246g\313\25\213g\313\25cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25Cg\313\25cg\313\25cg\313\25cg\313\25cg\313\25"
	  "Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0g\313\25Cg\313\25Cg\313\25Cg\313\25C\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
	  "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",
	};


	ToolButton::ToolButton ()
	: AbstractButton()
	{
		set_size(24, 24);
		set_round_type(RoundAll);

		InitializeToolButton();
	}

	ToolButton::~ToolButton ()
	{
		glDeleteVertexArrays(2, vaos_);
	}

	void ToolButton::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), DefaultBorderWidth(), RoundAll, 5);
			inner_->Bind();
			tool.SetInnerBufferData(inner_.get());
			outer_->Bind();
			tool.SetOuterBufferData(outer_.get());

			set_size(*request.size());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	ResponseType ToolButton::Draw (const Profile& profile)
	{
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->Use();

		glm::vec3 pos((GLfloat)position().x(), (GLfloat)position().y(), 0.f);

		glUniform3fv(Shaders::instance->triangle_uniform_position(), 1, glm::value_ptr(pos));
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		if (down()) {
			glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
			        Theme::instance->regular().inner_sel.data());
		} else {
			if (hover()) {
				glUniform1i(Shaders::instance->triangle_uniform_gamma(), 15);
			}

			glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
					Theme::instance->regular().inner.data());
		}

		glBindVertexArray(vaos_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.f, 0.f,
		        0.f, 1.f);

		glBindVertexArray(vaos_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform3f(Shaders::instance->triangle_uniform_position(),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);

			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->Reset();

		pos.x += (size().width() - icon_->size().width()) / 2;
		pos.y += (size().height() - icon_->size().height()) / 2;

		if(hover()) {
			icon_->Draw(pos, 15);
		} else {
			icon_->Draw(pos, 0);
		}

		return Accept;
	}

	void ToolButton::SetAction (const String& text)
	{
		RefPtr<Action> action(new Action(text));

		action_ = action;
	}

	void ToolButton::SetAction (const String& text, const String& shortcut)
	{
		RefPtr<Action> action(new Action(text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<Icon>& icon, const String& text)
	{
		RefPtr<Action> action(new Action(icon, text));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<Icon>& icon, const String& text,
	        const String& shortcut)
	{
		RefPtr<Action> action(new Action(icon, text, shortcut));

		action_ = action;
	}

	void ToolButton::SetAction (const RefPtr<Action>& item)
	{
		action_ = item;
	}

	Size ToolButton::GetPreferredSize() const
	{
		return Size(24, 24);
	}

	void ToolButton::InitializeToolButton ()
	{
		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), RoundAll, 5.f);

		glGenVertexArrays(2, vaos_);

		glBindVertexArray(vaos_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->Generate();
		inner_->Bind();
		tool.SetInnerBufferData(inner_.get());
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vaos_[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->Generate();
		outer_->Bind();
		tool.SetOuterBufferData(outer_.get());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);

		// demo
		//icon_.reset(new PixelIcon(24, 24, pixel_data));

		Image image;

		if(!image.Read("blender_icons16.png")) {
			icon_.reset(new PixelIcon(24, 24, pixel_data));
		} else {

			RefPtr<TextureAtlas2D> texture(new TextureAtlas2D);
			texture->Generate(image.width(),
					image.height(),
					16,
					16,
					5,
					10,
					5,
					5);
			texture->Bind();
			texture->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			texture->SetMinFilter(GL_LINEAR);
			texture->SetMagFilter(GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			texture->SetImage(0,
					GL_RGBA,
					image.width(),
					image.height(),
			        0,
			        GL_RGBA,
			        GL_UNSIGNED_BYTE,
			        image.pixels());

			texture->Reset();

			GLfloat x1 = (GLfloat)5 / image.width();
			GLfloat y1 = (GLfloat)10 / image.height();
			GLfloat x2 = (GLfloat)(5 + 16) / image.width();
			GLfloat y2 = (GLfloat)(10 + 16) / image.height();

			GLfloat uv[] = {
					x1, y2,
					x2, y2,
					x1, y1,
					x2, y1
			};

			icon_.reset(new PixelIcon(16, 16, texture, uv));
		}

	}

}
