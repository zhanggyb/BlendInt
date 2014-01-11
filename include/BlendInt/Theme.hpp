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

#ifndef _BLENDINT_THEME_HPP_
#define _BLENDINT_THEME_HPP_

//#include <BlendInt/Tuple.hpp>
#include <BlendInt/Color.hpp>
#include <BlendInt/Font.hpp>
#include <BlendInt/Types.hpp>

namespace BlendInt {

	class Interface;
	class ThemeManager;

	struct WidgetTheme
	{
		WidgetTheme ();

		Color outline;		// Outline
		Color item;				// Item
		Color inner;			// Inner
		Color inner_sel;	// Inner Selected
		Color text;				// Text
		Color text_sel;		// Text Selected
		bool shaded;			// Is Shaded
		Orientation shaderdir;	// shader orientation
		short shadetop;			// Shade Top, -100 - 100
		short shadedown;	// Shade Down
		bool alpha_check;
	};

	struct WidgetStateTheme
	{
		WidgetStateTheme();

		Color inner_anim;	// Animated
		Color inner_anim_sel;	// Animated Selected
		Color inner_key;	// Keyframe
		Color inner_key_sel;	// Keyframe Selected
		Color inner_driven;	// Driven
		Color inner_driven_sel;	// Driven Selected
		float blend;
		float pad;
	};

	struct PanelTheme
	{
		PanelTheme ();

		Color header;
		Color back;
		bool show_header;
		bool show_back;
		int pad;
	};

	struct Themes
	{
		/* Interface Elements (buttons, menus, icons) */
		WidgetTheme regular;
		WidgetTheme tool;
		WidgetTheme text;
		WidgetTheme radio_button;
		WidgetTheme option;
		WidgetTheme toggle;
		WidgetTheme number_field;
		WidgetTheme number_slider;
		WidgetTheme menu;
		WidgetTheme pulldown;
		WidgetTheme menu_back;
		WidgetTheme menu_item;
		WidgetTheme tooltip;
		WidgetTheme box;
		WidgetTheme scroll;
		WidgetTheme progress;
		WidgetTheme list_item;

		WidgetStateTheme state;

		PanelTheme panel; /* depricated, but we keep it for do_versions (2.66.1) */

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

	class ThemeManager
	{
	public:

		friend class Interface;

		static ThemeManager* instance ();

		const Themes* themes () const
		{
			return &m_themes;
		}

		const FontStyle* fontStyle () const
		{
			return &m_fonts;
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
		ThemeManager ();

		/**
		 * @brief Private destructor
		 *
		 * Move destructor to private to disallow simply delete
		 * use release() instead
		 */
		~ThemeManager ();

		Themes m_themes;

		FontStyle m_fonts;

		static ThemeManager* theme_manager;

		ThemeManager (const ThemeManager& orig);

		ThemeManager& operator = (const ThemeManager& orig);

	};

	inline const Themes* themes ()
	{
		return ThemeManager::instance()->themes();
	}

} /* namespace BlendInt */
#endif /* THEME_H_ */
