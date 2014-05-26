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

#ifndef _BLENDINT_GUI_TABBUTTON_HPP_
#define _BLENDINT_GUI_TABBUTTON_HPP_

#include <BlendInt/Core/String.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>

namespace BlendInt {

	class TabButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(TabButton);

	public:

		TabButton ();

		TabButton (const String& text);

		virtual ~TabButton ();

		void SetText (const String& text);

		void SetFont (const Font& font);

		const String& text () const {return m_text;}

		size_t text_length () const {return m_text_length;}

		const Font& font () const {return m_font;}

		virtual Size GetPreferredSize () const;

#ifdef DEBUG
		void GenerateTabButtonVertices (const Size& size, float border,
						std::vector<GLfloat>* inner,
						std::vector<GLfloat>* outer);
#endif

	protected:

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

#ifndef DEBUG
		void GenerateTabButtonVertices (const Size& size, float border,
						std::vector<GLfloat>* inner,
						std::vector<GLfloat>* outer);
#endif

		inline double sin_curve (double x, double amplitude, double shift_x, double shift_y);

		inline void set_text (const String& text)
		{
			m_text = text;
		}

		inline void set_font (const Font& font)
		{
			m_font = font;
		}

		void set_text_length (size_t length)
		{
			m_text_length = length;
		}

		void set_pen (int x, int y)
		{
			m_font.set_pen(x, y);
		}

	private:

		void InitializeTabButton ();

		void InitializeTabButton (const String& text);

		void GenerateTabButtonBuffers (const Size& size,
						GLArrayBuffer* inner_buffer,
						GLArrayBuffer* outer_buffer);

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;

		size_t m_text_length;	// How many text to be printed, depends on the button size

		String m_text;

		Font m_font;

	};

}

#endif /* _BLENDINT_GUI_TABBUTTON_HPP_ */
