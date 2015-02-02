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

#pragma once

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/AbstractDialog.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>
#include <BlendInt/Gui/CloseButton.hpp>
#include <BlendInt/Gui/Label.hpp>

namespace BlendInt {
    
    class MessageBox: public AbstractDialog
    {
    public:
        
        MessageBox (const String& title, const String& description);
        
        virtual ~MessageBox ();
        
    protected:
        
		virtual ResponseType Draw (AbstractWindow* context);

        virtual void UpdateLayout ();

    private:

        void OnClose (AbstractButton* btn);

        GLuint vao_[2];

        GLBuffer<ARRAY_BUFFER, 2> vbo_;

		RefPtr<FrameShadow> shadow_;

		Label* title_;

		Label* description_;

		CloseButton* close_;

    };
    
}
