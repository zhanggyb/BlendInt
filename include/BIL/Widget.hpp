/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_WIDGET_H_
#define _BIL_WIDGET_H_

#include <BIL/Drawable.hpp>
#include <BIL/Theme.hpp>

#include <BIL/Utilities-inl.hpp>

#define FTOCHAR(val) ((val) <= 0.0f) ? 0 : (((val) > (1.0f - 0.5f / 255.0f)) ? 255 : (char)((255.0f * (val)) + 0.5f))

/* used for transp checkers */
#define UI_TRANSP_DARK 100
#define UI_TRANSP_LIGHT 160

#define UI_TEXT_MARGIN_X 0.4f

#define WIDGET_AA_JITTER 8

namespace BIL {

	// TODO: rename to Widget, confused to controller in MVC

	class Widget: public BIL::Drawable
	{
	public:

		// max as used by round_box__edges
		static const int WIDGET_CURVE_RESOLU = 9;
		static const int WIDGET_SIZE_MAX = WIDGET_CURVE_RESOLU * 4;

		struct Trias {
			unsigned int tot;
			float vec[16][2];
			const unsigned int (*index)[3];
		};

		struct Base {
			int totvert, halfwayvert;
			float outer_v[WIDGET_SIZE_MAX][2];
			float inner_v[WIDGET_SIZE_MAX][2];
			float inner_uv[WIDGET_SIZE_MAX][2];
	
			short inner, outline, emboss; /* set on/off */
			short shadedir;
	
			Trias tria1;
			Trias tria2;
		};

		Widget (Traceable * parent = NULL);

		virtual ~Widget ();

		virtual void Render (void);

	protected:

		void DrawTrias (const Trias* tria);

		void DrawWidgetBase (const Base* wtb, const WidgetColors* wcol);

	protected:					// Events

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);
		
		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void InputMethodEvent (unsigned int character);

		virtual void CursorEnterEvent (int entered);

	private:

		// Prepare shade colors
		/**
		 * @brief Prepare shade colors
		 * @param[in] color the initial color
		 * @param[in] shadetop shade of top: -100 - 100
		 * @param[in] shadedown shade of down: -100 - 100
		 * @param[out] coltop Color at the top
		 * @param[out] coldown Color at the bottom
		 */
		inline void set_shade_colors (const RGBA& color,
									  short shadetop,
									  short shadedown,
									  RGBA* top_color,
									  RGBA* bottom_color)
		{
			top_color->rgba.r = correct_in_scope (color.rgba.r + shadetop, 0, 255);
			top_color->rgba.g = correct_in_scope (color.rgba.g + shadetop, 0, 255);
			top_color->rgba.b = correct_in_scope (color.rgba.g + shadetop, 0, 255);
			top_color->rgba.a = color.rgba.a;

			bottom_color->rgba.r = correct_in_scope (color.rgba.r + shadedown, 0, 255);
			bottom_color->rgba.g = correct_in_scope (color.rgba.g + shadedown, 0, 255);
			bottom_color->rgba.b = correct_in_scope (color.rgba.b + shadedown, 0, 255);
			bottom_color->rgba.a = color.rgba.a;			
		}


		/*
		  static void shadecolors4(char coltop[4], char *coldown, const char *color, short shadetop, short shadedown)
		  {
	
		  coltop[0] = CLAMPIS(color[0] + shadetop, 0, 255);
		  coltop[1] = CLAMPIS(color[1] + shadetop, 0, 255);
		  coltop[2] = CLAMPIS(color[2] + shadetop, 0, 255);
		  coltop[3] = color[3];

		  coldown[0] = CLAMPIS(color[0] + shadedown, 0, 255);
		  coldown[1] = CLAMPIS(color[1] + shadedown, 0, 255);
		  coldown[2] = CLAMPIS(color[2] + shadedown, 0, 255);
		  coldown[3] = color[3];
		  }
		*/
		
		/**
		 * @brief Preapare shade colors for round box
		 */
		inline void set_round_box_shade_colors (const RGBAf& color1,
												const RGBAf& color2,
												float fac,
												RGBAf* color_output)
		{
			float faci = correct_in_scope (fac, static_cast<float>(0.0), static_cast<float>(1.0));
			float facm = 1.0 - faci;

			color_output->rgba.r = correct_in_scope((faci * color1.rgba.r + facm * color2.rgba.r),
													static_cast<float>(0.0),
													static_cast<float>(1.0));
			color_output->rgba.g = correct_in_scope((faci * color1.rgba.g + facm * color2.rgba.g),
													static_cast<float>(0.0),
													static_cast<float>(1.0));
			color_output->rgba.b = correct_in_scope((faci * color1.rgba.b + facm * color2.rgba.b),
													static_cast<float>(0.0),
													static_cast<float>(1.0));
			color_output->rgba.a = correct_in_scope((faci * color1.rgba.a + facm * color2.rgba.a),
													static_cast<float>(0.0),
													static_cast<float>(1.0));;
		}

		/*
		static void round_box_shade_col4_r(unsigned char col_r[4], const char col1[4], const char col2[4], const float fac)
		{
			const int faci = FTOCHAR(fac);
			const int facm = 255 - faci;

			col_r[0] = (faci * col1[0] + facm * col2[0]) >> 8;
			col_r[1] = (faci * col1[1] + facm * col2[1]) >> 8;
			col_r[2] = (faci * col1[2] + facm * col2[2]) >> 8;
			col_r[3] = (faci * col1[3] + facm * col2[3]) >> 8;
		}
		*/

		/*
		inline void copy_v2_v2(float r[2], const float a[2])
		{
			r[0] = a[0];
			r[1] = a[1];
		}
		*/

		/*
		inline void widget_verts_to_quad_strip(Base *wtb, const int totvert, float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2])
		{
			int a;
			for (a = 0; a < totvert; a++) {
				copy_v2_v2(quad_strip[a * 2], wtb->outer_v[a]);
				copy_v2_v2(quad_strip[a * 2 + 1], wtb->inner_v[a]);
			}
			copy_v2_v2(quad_strip[a * 2], wtb->outer_v[0]);
			copy_v2_v2(quad_strip[a * 2 + 1], wtb->inner_v[0]);
		}
		*/

	};

} /* namespace BIL */
#endif /* _BIL_WIDGET_H_ */
