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

#include <BIL/Tuple.hpp>

namespace BIL {

	struct WidgetColors
	{
		WidgetColors ();

		RGBAf outline;		// Outline
		RGBAf inner;			// Inner
		RGBAf inner_sel;	// Inner Selected
		RGBAf item;				// Item
		RGBAf text;				// Text
		RGBAf text_sel;		// Text Selected
		bool shaded;			// Is Shaded
		short shadetop;		// Shade Top
		short shadedown;	// Shade Down
		bool alpha_check;
	};

	struct WidgetStateColors
	{
		WidgetStateColors();

		RGBAf inner_anim;	// Animated
		RGBAf inner_anim_sel;	// Animated Selected
		RGBAf inner_key;	// Keyframe
		RGBAf inner_key_sel;	// Keyframe Selected
		RGBAf inner_driven;	// Driven
		RGBAf inner_driven_sel;	// Driven Selected
		float blend;
		float pad;
	};

	struct PanelColors
	{
		PanelColors ();

		RGBAf header;
		RGBAf back;
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
			WidgetColors wcol_regular, wcol_tool, wcol_text;
			WidgetColors wcol_radio, wcol_option, wcol_toggle;
			WidgetColors wcol_num, wcol_numslider;
			WidgetColors wcol_menu, wcol_pulldown, wcol_menu_back,
			        wcol_menu_item, wcol_tooltip;
			WidgetColors wcol_box, wcol_scroll, wcol_progress, wcol_list_item;

			WidgetStateColors wcol_state;

			PanelColors panel; /* depricated, but we keep it for do_versions (2.66.1) */

			/* fac: 0 - 1 for blend factor, width in pixels */
			float menu_shadow_fac;
			short menu_shadow_width;

			short pad;

			char iconfile[256];	// FILE_MAXFILE length
			float icon_alpha;

			/* Axis Colors */
			RGBAf xaxis, yaxis, zaxis;
		};

	public:

		static Theme* instance (void);

		static bool release (void);

		void initialize (void);

		const ThemeUI* getThemeUI (void) const
		{
			return &_themeUI;
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
		virtual ~Theme ();

	private:
		// member functions disabled

		Theme (const Theme& orig);

		Theme& operator = (const Theme& orig);

	private:
		// static member variables

		static Theme* theme;

	private:

		ThemeUI _themeUI;
	};

} /* namespace BIL */
#endif /* THEME_H_ */
