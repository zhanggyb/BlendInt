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

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <BlendInt/Core/Color.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Core/Types.hpp>

namespace BlendInt {

	class Interface;

	class Theme;

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
		short shadetop;			// Shade Top, -100 - 100
		short shadedown;	// Shade Down
		bool alpha_check;
	};

	class Theme
	{
		DISALLOW_COPY_AND_ASSIGN(Theme);

	public:

		static Theme* instance;

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

		const WidgetTheme& box () const
		{
			return m_box;
		}
		
		unsigned int dpi () const
		{
			return m_dpi;
		}
		
		short pixel () const
		{
			return m_pixel;
		}

		const WidgetTheme& list_item () const
		{
			return m_list_item;
		}
		
		const WidgetTheme& menu () const
		{
			return m_menu;
		}
		
		const WidgetTheme& menu_back () const
		{
			return m_menu_back;
		}
		
		const WidgetTheme& menu_item () const
		{
			return m_menu_item;
		}
		
		const WidgetTheme& number_field () const
		{
			return m_number_field;
		}
		
		const WidgetTheme& number_slider () const
		{
			return m_number_slider;
		}
		
		const WidgetTheme& option () const
		{
			return m_option;
		}

		const WidgetTheme& progress () const
		{
			return m_progress;
		}
		
		const WidgetTheme& pulldown () const
		{
			return m_pulldown;
		}
		
		const WidgetTheme& radio_button () const
		{
			return m_radio_button;
		}
		
		const WidgetTheme& regular () const
		{
			return m_regular;
		}
		
		const WidgetTheme& scroll () const
		{
			return m_scroll;
		}
		
		const WidgetTheme& tab () const
		{
			return m_tab;
		}
		
		const WidgetTheme& text () const
		{
			return m_text;
		}
		
		const WidgetTheme& toggle () const
		{
			return m_toggle;
		}
		
		const WidgetTheme& tool () const
		{
			return m_tool;
		}
		
		const WidgetTheme& tooltip () const
		{
			return m_tooltip;
		}
		
		float menu_shadow_fac1 () const
		{
			return m_shadow_fac;
		}
		
		short menu_shadow_width1 () const
		{
			return m_shadow_width;
		}
		
		float shadow_fac () const
		{
			return m_shadow_fac;
		}
		
		short shadow_width () const
		{
			return m_shadow_width;
		}

	private:

		friend class Interface;

		static bool Initialize ();

		static void Release ();

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

		void ParseWidgetColorNode (const rapidxml::xml_node<>* node);

		rapidxml::xml_node<>* AllocateWidgetThemeNode (rapidxml::xml_document<>& doc, const char* name, const WidgetTheme& wtheme);

		/* Interface Elements (buttons, menus, icons) */
		WidgetTheme m_regular;
		WidgetTheme m_tool;
		WidgetTheme m_text;
		WidgetTheme m_radio_button;
		WidgetTheme m_option;
		WidgetTheme m_toggle;
		WidgetTheme m_number_field;
		WidgetTheme m_number_slider;
		WidgetTheme m_menu;
		WidgetTheme m_pulldown;
		WidgetTheme m_menu_back;
		WidgetTheme m_menu_item;
		WidgetTheme m_tab;
		WidgetTheme m_tooltip;
		WidgetTheme m_box;
		WidgetTheme m_scroll;
		WidgetTheme m_progress;
		WidgetTheme m_list_item;

		/** Font DPI */
		unsigned int m_dpi;

		/** How many pixels for 1 border */
		short m_pixel;

		/* fac: 0 - 1 for blend factor, width in pixels */
		float m_shadow_fac;
		short m_shadow_width;

		//short pad;

		//char iconfile[256];	// FILE_MAXFILE length
		//float icon_alpha;

		/* Axis Colors */
		Color m_xaxis, m_yaxis, m_zaxis;
	};

} /* namespace BlendInt */
#endif /* THEME_H_ */
