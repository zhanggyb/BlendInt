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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Decoration::Decoration()
	: AbstractDecoration(),
	  space_(4),
	  close_button_(nullptr)
	{
		// create close button
		close_button_ = Manage(new CloseButton);

		set_size(200, 5 + 5 + close_button_->size().height());
		close_button_->MoveTo(5, size().height() - close_button_->size().height() - 5);

		PushBackSubView(close_button_);

		set_round_type(RoundTopLeft | RoundTopRight);

		events()->connect(close_button_->clicked(), this, &Decoration::OnCloseButtonClicked);
	}

	Decoration::~Decoration ()
	{
	}

	bool Decoration::IsExpandX () const
	{
		return true;
	}

	bool Decoration::IsExpandY () const
	{
		return true;
	}

	Size Decoration::GetPreferredSize () const
	{
		return Size(200, close_button_->size().height() + 5 + 5);
	}

	void Decoration::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			int x = 5;
			int y = (size().height() - close_button_->size().height()) / 2;
			close_button_->MoveTo(x, y);

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Decoration::Draw (const Context* context)
	{
		return Ignore;
	}

	void Decoration::OnCloseButtonClicked(AbstractButton* button)
	{
		fire_close_triggered();
	}

}
