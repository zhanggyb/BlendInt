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

		virtual void PerformOrientationUpdate (Orientation orientation);

		virtual void PerformMinimumUpdate (double minimum);

		virtual void PerformMaximumUpdate (double maximum);

		virtual void PerformValueUpdate (double value);

		virtual void PerformStepUpdate (double step);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const Profile& profile);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		/**
		 *  Generate vertices for drawing the inner slider bar
		 *  size The size of this widget
		 *  border The border width of this widget
		 *  round_type The round corner type of this widget
		 *  radius The round coner radius of this widget
		 *  slide_verts The vertices output for drawing the slider bar
		 */

		void GenerateSliderVertices (const Size& out_size,
						float border,
						int round_type,
						float out_radius,
						double value,
						double minimum,
						double maximum,
						std::vector<GLfloat>& left_vertices,
						std::vector<GLfloat>& right_vertices);

	private:

		void InitOnce ();

		float GetSlidePosition (float border, double value);

		void GenerateLeftSliderVertices (float minx, float maxx, float miny,
						float maxy, int round_type, float radius,
						std::vector<GLfloat>& verts);

		void GenerateRightSliderVertices (float minx, float maxx, float miny,
						float maxy, int round_type, float radius,
						std::vector<GLfloat>& verts);

		void GenerateSliderVertices (float minx, float maxx, float miny,
						float maxy, int round_type, float radius,
						std::vector<GLfloat>& verts);

		/**
		 * @brief VertexArray objects used in this widget
		 *
		 * [0] - inner buffer
		 * [1] - outer buffer
		 * [2] - slider bar buffer
		 */
		GLuint m_vao[4];

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;

		RefPtr<GLArrayBuffer> m_slider1;
		RefPtr<GLArrayBuffer> m_slider2;

		Font m_font;

		String m_title;

		bool m_right;

		static Margin default_numberslider_padding;
	};

}

#endif /* _BLENDINT_GUI_NUMBERSLIDER_HPP_ */
