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
 * License along with BIL.	If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_WIDGET_H_
#define _BIL_WIDGET_H_

#include <BIL/Drawable.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Color.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Rect.hpp>

/* used for transp checkers */
#define UI_TRANSP_DARK 100
#define UI_TRANSP_LIGHT 160

#define UI_TEXT_MARGIN_X 0.4f

namespace BIL {

	class Color;

	class Widget: public BIL::Drawable
	{
	public:

		// max as used by round_box__edges
		static const int WIDGET_CURVE_RESOLU = 9;
		static const int WIDGET_SIZE_MAX = WIDGET_CURVE_RESOLU * 4;

		struct WidgetTriangle
		{
			WidgetTriangle();

			unsigned int tot;
			float vec[16][2];
			const unsigned int (*index)[3];
		};

		struct WidgetBase
		{
			WidgetBase();

			int totvert, halfwayvert;
			float outer_v[WIDGET_SIZE_MAX][2];
			float inner_v[WIDGET_SIZE_MAX][2];
			float inner_uv[WIDGET_SIZE_MAX][2];

			bool inner, outline, emboss, shadedir; /* set on/off */

			WidgetTriangle tria1;
			WidgetTriangle tria2;
		};

	public:

		Widget (Traceable * parent = NULL);

		virtual ~Widget ();

		virtual void Render (void);

	protected:

		void DrawAntiTriangle (float x1,
						   float y1,
						   float x2,
						   float y2,
						   float x3,
						   float y3);

		void DrawAntiRoundbox (int mode,
							   float minx,
							   float miny,
							   float maxx,
							   float maxy,
							   float rad,
							   bool use_alpha);

		void DrawTrias (const WidgetTriangle* tria);

		void DrawWidgetBase (WidgetBase* wtb, WidgetColors* wcol);
		
		void DrawWidgetBaseOutline (const WidgetBase* wtb);

	protected:
		// Events

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

		virtual void InputMethodEvent (unsigned int character);

		virtual void CursorEnterEvent (int entered);

	private:	// member functions

		/**
		 * @brief Calculate round box shadow edges
		 * @param vert
		 * @param rect
		 * @param rad
		 * @param roundboxalign
		 * @param step
		 * @return vertex number used in Base::totvert
		 */
		int CalculateRoundBoxShadowEdges(float (*vert)[2],
					const Recti& rect, float rad, int roundboxalign, float step);

		void CalculateRoundBoxEdges (int roundboxalign, const Recti& rect, float rad, float radi, WidgetBase *wt);

		void CalculateRoundBoxEdges (int roundboxalign, const Recti& rect, float rad, WidgetBase *wt);

		/* based on button rect, return scaled array of triangles */
		void CalculateTriangleNumbers (const Recti& rect, float triasize, char where, WidgetTriangle *tria);

		void CalculateScrollCircle(const Recti& rect, float triasize, char where, WidgetTriangle *tria);

		void CalculateMenuTriangle (const Recti& rect, WidgetTriangle *tria);

		void CalculateCheckTriangle (const Recti& rect, WidgetTriangle *tria);

		void verts_to_quad_strip(const int totvert, float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2], WidgetBase *wtb);

		void verts_to_quad_strip_open(const int totvert, float quad_strip[WIDGET_SIZE_MAX * 2][2], WidgetBase *wtb);

	private:

		WidgetBase base_;

		// converted colors for state
		WidgetColors colors_;

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
