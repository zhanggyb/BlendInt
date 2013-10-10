/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_THEME_HPP_
#define _BILO_THEME_HPP_

//#include <BILO/Tuple.hpp>
#include <BILO/Color.hpp>
#include <BILO/Font.hpp>

namespace BILO {

	class Interface;

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
			WidgetColors regular;
			WidgetColors tool;
			WidgetColors text;
			WidgetColors radio_button;
			WidgetColors option;
			WidgetColors toggle;
			WidgetColors number_field;
			WidgetColors number_slider;
			WidgetColors menu;
			WidgetColors pulldown;
			WidgetColors menu_back;
			WidgetColors menu_item;
			WidgetColors tooltip;
			WidgetColors box;
			WidgetColors scroll;
			WidgetColors progress;
			WidgetColors list_item;

			WidgetStateColors state;

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

		friend class Interface;

		static Theme* instance ();

		const ThemeUI* themeUI () const
		{
			return &themeUI_;
		}

		const FontStyle* fontStyle () const
		{
			return &fonts_;
		}

#ifdef DEBUG
	public:
#else
	private:
#endif

		static bool initialize ();

		static void release ();

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

} /* namespace BILO */
#endif /* THEME_H_ */
