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

#include <BlendInt/Gui/MessageBox.hpp>
#include <BlendInt/Gui/LinearLayout.hpp>

namespace BlendInt {
    
    MessageBox::MessageBox(const String& title, const String& description)
    : AbstractDialog(),
      title_(0),
      description_(0),
      close_(0)
    {
		shadow_.reset(new FrameShadow(size(), round_type(), round_radius()));

		title_ = new Label(title, AlignCenter);
		description_ = new Label(description);
		close_ = new CloseButton;

		events()->connect(close_->clicked(), this, &MessageBox::OnClose);

		LinearLayout* hlayout = new LinearLayout(Horizontal);
		hlayout->SetMargin(Margin(0, 0, 0, 0));
		hlayout->SetSpace(0);
		hlayout->AddWidget(close_);
		hlayout->AddWidget(title_);

		LinearLayout* vlayout = new LinearLayout(Vertical);
		vlayout->AddWidget(hlayout);
		vlayout->AddWidget(description_);

		vlayout->Resize(size());

		PushBackSubView(vlayout);
    }
    
    MessageBox::~MessageBox()
    {
        
    }

    void MessageBox::UpdateLayout()
    {
		shadow_->Resize(size());

		ResizeSubView(first_subview(), size());
    }
    
    ResponseType BlendInt::MessageBox::Draw (AbstractWindow* context)
    {
    	shadow_->Draw(position().x(), position().y());

    	return AbstractDialog::Draw(context);
    }

	void MessageBox::OnClose (AbstractButton* btn)
	{
		delete this;
	}

}
