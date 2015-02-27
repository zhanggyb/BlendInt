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

#include <string>

#include <core/types.hpp>
#include <core/margin.hpp>

#include <opengl/gl-buffer.hpp>

#include <gui/text.hpp>
#include <gui/abstract-round-widget.hpp>

using namespace std;

namespace BlendInt {

	class Label: public AbstractRoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Label);

	public:

		Label (const String& label, Alignment alignment = AlignLeft);

		virtual ~Label ();

		/**
		 * @brief set the label text
		 * @param label
		 *
		 * call this function will reset the size and preferred size of this object
		 */
		void SetText (const String& text);

		/**
		 * @brief set the text font
		 * @param font
		 *
		 * call this function will reset the preferred size of this object
		 */
		void SetFont (const Font& font);

		void SetForeground (uint32_t color);

		void SetBackground (uint32_t color);

		virtual Size GetPreferredSize () const;

		virtual bool IsExpandX () const;

        static Margin kPadding;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual Response PerformMousePress (AbstractWindow* context);

		virtual Response PerformMouseRelease (AbstractWindow* context);

		virtual Response PerformMouseMove (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

	private:

		RefPtr<Text> text_;

        Alignment alignment_;

        GLuint vao_;
        GLBuffer<> vbo_;

        uint32_t foreground_;

        uint32_t background_;

    };

} /* namespace BlendInt */
