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

#pragma once

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <core/types.hpp>
#include <core/color.hpp>

#include <gui/font.hpp>

namespace BlendInt {

	class Theme;

	struct ColorScheme
	{
		ColorScheme ();

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

	class Theme
	{
		DISALLOW_COPY_AND_ASSIGN(Theme);

	public:

		/**
		 * @brief Load theme from an existing xml file
		 */
		bool Load (const std::string& filepath);

		/**
		 * @brief Export and save the config into an xml file
		 */
		bool Save (const std::string& filepath);

		/**
		 * @brief Reset to the default setting
		 */
		void Reset ();

		const ColorScheme& box () const
		{
			return box_;
		}

		const ColorScheme& list_item () const
		{
			return list_item_;
		}

		const ColorScheme& menu () const
		{
			return menu_;
		}

		const ColorScheme& menu_back () const
		{
			return menu_back_;
		}

		const ColorScheme& menu_item () const
		{
			return menu_item_;
		}

		const ColorScheme& number_field () const
		{
			return number_field_;
		}

		const ColorScheme& number_slider () const
		{
			return number_slider_;
		}

		const ColorScheme& option () const
		{
			return option_;
		}

		const ColorScheme& progress () const
		{
			return progress_;
		}

		const ColorScheme& pulldown () const
		{
			return pulldown_;
		}

		const ColorScheme& push_button () const
		{
		  return push_button_;
		}

		const ColorScheme& radio_button () const
		{
			return radio_button_;
		}

		const ColorScheme& regular () const
		{
			return regular_;
		}

		const ColorScheme& scroll () const
		{
			return scroll_;
		}

		const ColorScheme& tab () const
		{
			return tab_;
		}

		const ColorScheme& text () const
		{
			return text_;
		}

		const ColorScheme& toggle () const
		{
			return toggle_;
		}

		const ColorScheme& tool () const
		{
			return tool_;
		}

		const ColorScheme& tooltip () const
		{
			return tooltip_;
		}

		const ColorScheme& dialog () const
		{
			return dialog_;
		}

		const ColorScheme& decoration () const
		{
			return decoration_;
		}

		const char* default_font () const
		{
			return default_font_;
		}

		unsigned int dpi () const
		{
			return dpi_;
		}

		short pixel () const
		{
			return pixel_;
		}

		float menu_shadow_fac1 () const
		{
			return shadow_fac_;
		}

		short menu_shadow_width1 () const
		{
			return shadow_width_;
		}

		float shadow_fac () const
		{
			return shadow_fac_;
		}

		short shadow_width () const
		{
			return shadow_width_;
		}

	private:

		friend class AbstractWindow;

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

		void ParseUINode (const rapidxml::xml_node<>* node);

		void ParseColorSchemeNode (const rapidxml::xml_node<>* node);

		rapidxml::xml_node<>* AllocateThemeNode (rapidxml::xml_document<>& doc, const char* name, const ColorScheme& wtheme);

		/* Interface Elements (buttons, menus, icons) */
		ColorScheme regular_;
		ColorScheme tool_;
		ColorScheme text_;
    ColorScheme push_button_;
		ColorScheme radio_button_;
		ColorScheme option_;
		ColorScheme toggle_;
		ColorScheme number_field_;
		ColorScheme number_slider_;
		ColorScheme menu_;
		ColorScheme pulldown_;
		ColorScheme menu_back_;
		ColorScheme menu_item_;
		ColorScheme tab_;
		ColorScheme tooltip_;
		ColorScheme box_;
		ColorScheme scroll_;
		ColorScheme progress_;
		ColorScheme list_item_;
		ColorScheme dialog_;
		ColorScheme decoration_;

		/** Default font */
		char default_font_[256];

		/** Font DPI */
		unsigned int dpi_;

		/** How many pixels for 1 border */
		short pixel_;

		/* fac: 0 - 1 for blend factor, width in pixels */
		// NOT USED
		float shadow_fac_;

		short shadow_width_;

		//short pad;

		//char iconfile[256];	// FILE_MAXFILE length
		//float icon_alpha;

		/* Axis Colors */
		Color xaxis_, yaxis_, zaxis_;
	};

} /* namespace BlendInt */
