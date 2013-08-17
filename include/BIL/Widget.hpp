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

/* used for transp checkers */
#define UI_TRANSP_DARK 100
#define UI_TRANSP_LIGHT 160

#define UI_TEXT_MARGIN_X 0.4f

#define WIDGET_AA_JITTER 8

namespace BIL {

	class Color;

	class Widget: public BIL::Drawable
	{
	public:

		// max as used by round_box__edges
		static const int WIDGET_CURVE_RESOLU = 9;
		static const int WIDGET_SIZE_MAX = WIDGET_CURVE_RESOLU * 4;

		struct Trias
		{
			unsigned int tot;
			float vec[16][2];
			const unsigned int (*index)[3];
		};

		struct Base
		{
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

	private:

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
