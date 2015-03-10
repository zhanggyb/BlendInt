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

#include "editor-window.hpp"

namespace BlendInt {

  EditorWindow::EditorWindow (int width, int height, const char* name)
  : Window(width, height, name), dev_msg_(0)
  {

    events()->connect(this->resized(), this, &EditorWindow::OnResize);

    // show a message box
    dev_msg_ = new MessageBox("Note", "This UI editor is still under development");
    AddFrame(dev_msg_);
    dev_msg_->MoveTo((size().width() - dev_msg_->size().width()) / 2,
                 (size().height() - dev_msg_->size().height()) / 2);
    events()->connect(dev_msg_->destroyed(), this,
                      &EditorWindow::OnMessageBoxDestroyed);
  }

  EditorWindow::~EditorWindow ()
  {
  }

  void EditorWindow::OnResize (Window* window, const Size& size)
  {
    if (dev_msg_) {
      dev_msg_->MoveTo((size.width() - dev_msg_->size().width()) / 2,
                   (size.height() - dev_msg_->size().height()) / 2);
    }
  }

  void EditorWindow::OnMessageBoxDestroyed (AbstractFrame* sender)
  {
    assert(sender == dev_msg_);
    dev_msg_ = 0;
  }

}
