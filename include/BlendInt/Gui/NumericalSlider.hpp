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

#ifndef _BLENDINT_GUI_NUMBERSLIDER_HPP_
#define _BLENDINT_GUI_NUMBERSLIDER_HPP_

#include <BlendInt/Core/Margin.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/AbstractSlider.hpp>

namespace BlendInt {

	class NumericalSlider: public AbstractSlider<double>
	{
	DISALLOW_COPY_AND_ASSIGN(NumericalSlider);

	public:

		NumericalSlider (Orientation orientation = Horizontal);

		virtual ~NumericalSlider ();

		void SetTitle (const String& title);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual void UpdateSlider (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		/**
		 * @brief Generate vertices for drawing the inner slider bar
		 * @param[in] size The size of this widget
		 * @param[in] border The border width of this widget
		 * @param[in] round_type The round corner type of this widget
		 * @param[in] radius The round coner radius of this widget
		 * @param[out] slide_verts The vertices output for drawing the slider bar
		 */
		void GenerateSliderVertices (const Size& size,
						float border,
						int round_type,
						float radius,
						std::vector<GLfloat>& slide_verts);

	private:

		void InitOnce ();

		/**
		 * @brief VertexArray objects used in this widget
		 *
		 * [0] - inner buffer
		 * [1] - outer buffer
		 * [2] - slider bar buffer
		 */
		GLuint m_vao[3];

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;

		RefPtr<GLArrayBuffer> m_slider;

		Font m_font;

		String m_title;

		static Margin default_numberslider_padding;
	};

}

#endif /* _BLENDINT_GUI_NUMBERSLIDER_HPP_ */
