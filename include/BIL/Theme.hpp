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

#ifndef _BIL_THEME_H_
#define _BIL_THEME_H_

//#include <BIL/Tuple.hpp>
#include <BIL/Color.hpp>
#include <BIL/Font.hpp>

namespace BIL {

	struct WidgetColors
	{
		WidgetColors ();

		Color outline;		// Outline
		Color item;				// Item
		Color inner;			// Inner
		Color inner_sel;	// Inner Selected
		Color text;				// Text
		Color text_sel;		// Text Selected
		bool shaded;			// Is Shaded
		short shadetop;			// Shade Top, -100 - 100
		short shadedown;	// Shade Down
		bool alpha_check;
	};

	struct WidgetStateColors
	{
		WidgetStateColors();

		Color inner_anim;	// Animated
		Color inner_anim_sel;	// Animated Selected
		Color inner_key;	// Keyframe
		Color inner_key_sel;	// Keyframe Selected
		Color inner_driven;	// Driven
		Color inner_driven_sel;	// Driven Selected
		float blend;
		float pad;
	};

	struct PanelColors
	{
		PanelColors ();

		Color header;
		Color back;
		bool show_header;
		bool show_back;
		int pad;
	};

	class Theme
	{
	public:
		// public structures

		struct ThemeUI
		{
			/* Interface Elements (buttons, menus, icons) */
			WidgetColors wcol_regular;
			WidgetColors wcol_tool;
			WidgetColors wcol_text;
			WidgetColors wcol_radio;
			WidgetColors wcol_option;
			WidgetColors wcol_toggle;
			WidgetColors wcol_num;
			WidgetColors wcol_numslider;
			WidgetColors wcol_menu;
			WidgetColors wcol_pulldown;
			WidgetColors wcol_menu_back;
			WidgetColors wcol_menu_item;
			WidgetColors wcol_tooltip;
			WidgetColors wcol_box;
			WidgetColors wcol_scroll;
			WidgetColors wcol_progress;
			WidgetColors wcol_list_item;

			WidgetStateColors wcol_state;

			PanelColors panel; /* depricated, but we keep it for do_versions (2.66.1) */

			/* fac: 0 - 1 for blend factor, width in pixels */
			float menu_shadow_fac;
			short menu_shadow_width;

			short pad;

			char iconfile[256];	// FILE_MAXFILE length
			float icon_alpha;

			/* Axis Colors */
			Color xaxis, yaxis, zaxis;
		};

		struct FontStyle {
			FontStyle();
			Font regular;
		};

	public:

		static Theme* instance ();

		static bool initialize ();

		static void release ();

		const ThemeUI* themeUI () const
		{
			return &themeUI_;
		}

		const FontStyle* fontStyle () const
		{
			return &fonts_;
		}

	private:

		/**
		 * @brief Default constructor
		 */
		Theme ();

		/**
		 * @brief Private destructor
		 *
		 * Move destructor to private to disallow simply delete
		 * use release() instead
		 */
		~Theme ();

		ThemeUI themeUI_;

		FontStyle fonts_;

		static Theme* theme;

		Theme (const Theme& orig);

		Theme& operator = (const Theme& orig);

	};

} /* namespace BIL */
#endif /* THEME_H_ */
