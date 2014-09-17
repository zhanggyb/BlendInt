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

#ifndef _BLENDINT_STOCK_THEME_HPP_
#define _BLENDINT_STOCK_THEME_HPP_

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Gui/Font.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

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
			return box_;
		}
		
		unsigned int dpi () const
		{
			return dpi_;
		}
		
		short pixel () const
		{
			return pixel_;
		}

		const WidgetTheme& list_item () const
		{
			return list_item_;
		}
		
		const WidgetTheme& menu () const
		{
			return menu_;
		}
		
		const WidgetTheme& menu_back () const
		{
			return menu_back_;
		}
		
		const WidgetTheme& menu_item () const
		{
			return menu_item_;
		}
		
		const WidgetTheme& number_field () const
		{
			return number_field_;
		}
		
		const WidgetTheme& number_slider () const
		{
			return number_slider_;
		}
		
		const WidgetTheme& option () const
		{
			return option_;
		}

		const WidgetTheme& progress () const
		{
			return progress_;
		}
		
		const WidgetTheme& pulldown () const
		{
			return pulldown_;
		}
		
		const WidgetTheme& radio_button () const
		{
			return radio_button_;
		}
		
		const WidgetTheme& regular () const
		{
			return regular_;
		}
		
		const WidgetTheme& scroll () const
		{
			return scroll_;
		}
		
		const WidgetTheme& tab () const
		{
			return tab_;
		}
		
		const WidgetTheme& text () const
		{
			return text_;
		}
		
		const WidgetTheme& toggle () const
		{
			return toggle_;
		}
		
		const WidgetTheme& tool () const
		{
			return tool_;
		}
		
		const WidgetTheme& tooltip () const
		{
			return tooltip_;
		}
		
		float menu_shadow_fac1 () const
		{
			return m_shadow_fac;
		}
		
		short menu_shadow_width1 () const
		{
			return shadow_width_;
		}
		
		float shadow_fac () const
		{
			return m_shadow_fac;
		}
		
		short shadow_offset_x () const
		{
			return shadow_offset_x_;
		}

		short shadow_offset_y () const
		{
			return shadow_offset_y_;
		}

		short shadow_width () const
		{
			return shadow_width_;
		}

		const RefPtr<GLTexture2D>& shadow_texture () const
		{
			return shadow_texture_;
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

		void GenerateShadowTexture ();

		/* Interface Elements (buttons, menus, icons) */
		WidgetTheme regular_;
		WidgetTheme tool_;
		WidgetTheme text_;
		WidgetTheme radio_button_;
		WidgetTheme option_;
		WidgetTheme toggle_;
		WidgetTheme number_field_;
		WidgetTheme number_slider_;
		WidgetTheme menu_;
		WidgetTheme pulldown_;
		WidgetTheme menu_back_;
		WidgetTheme menu_item_;
		WidgetTheme tab_;
		WidgetTheme tooltip_;
		WidgetTheme box_;
		WidgetTheme scroll_;
		WidgetTheme progress_;
		WidgetTheme list_item_;

		/** Font DPI */
		unsigned int dpi_;

		/** How many pixels for 1 border */
		short pixel_;

		/* fac: 0 - 1 for blend factor, width in pixels */
		float m_shadow_fac;

		short shadow_offset_x_;

		short shadow_offset_y_;

		short shadow_width_;

		//short pad;

		//char iconfile[256];	// FILE_MAXFILE length
		//float icon_alpha;

		/* Axis Colors */
		Color xaxis_, yaxis_, zaxis_;

		RefPtr<GLTexture2D> shadow_texture_;
	};

} /* namespace BlendInt */
#endif /* THEME_H_ */
