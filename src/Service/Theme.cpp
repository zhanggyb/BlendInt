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

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	Theme* Theme::instance = 0;

	bool Theme::Initialize ()
	{
		bool result = false;

		if (!instance) {
			instance = new Theme;
		}

		if (instance) {
			instance->Reset();
			result = true;
		}

		return result;
	}

	void Theme::Release (void)
	{
		if (instance) {
			delete instance;
			instance = 0;
		}
	}

	WidgetTheme::WidgetTheme ()
			: outline(0x191919FF), item(0x191919FF), inner(0x999999FF), inner_sel(
			        0x646464FF), text(0x000000FF), text_sel(0xFFFFFFFF), shaded(
			        false), shadetop(0), shadedown(0), alpha_check(0)
	{
	}

	Theme::Theme ()
	: m_dpi(96),
	  m_menu_shadow_fac(0.5),
	  m_menu_shadow_width(12)
	{
		// TODO Auto-generated constructor stub

	}
	
	bool Theme::Load (const std::string& filepath)
	{
		using namespace rapidxml;

		bool ret = false;

		try {
			rapidxml::file<> fdoc(filepath.c_str());
			xml_document<> doc;
			doc.parse<0>(fdoc.data());

			// get the root node
			xml_node<>* root = doc.last_node("Theme");

			if (root) {
				xml_node<>* ui_node = root->last_node("ThemeUserInterface");
				if(ui_node) {
					ParseUINode(ui_node);
				}

			} else {
				std::cerr << "<Theme> should be the only root node in theme file: " << filepath << std::endl;
			}
		} catch (std::exception& ex) {
			std::cerr << "Error: " << ex.what() << std::endl;
		}


		return ret;
	}

	Theme::~Theme ()
	{
		// TODO Auto-generated destructor stub
	}

	void Theme::Reset()
	{
		// Regular
		// _theme.wcol_regular	// use the default setting in struct constructor

		// Tool
		m_tool.outline = 0x191919FF;
		m_tool.item = 0x191919FF;
		m_tool.inner = 0x999999FF;
		m_tool.inner_sel = 0x646464FF;
		m_tool.text = 0x000000FF;
		m_tool.text_sel = 0xFFFFFFFF;
		m_tool.shaded = true;
		m_tool.shadetop = 15;
		m_tool.shadedown = -15;

		// Radio Buttons
		m_radio_button.outline = 0x000000FF;
		m_radio_button.item = 0xFFFFFFFF;
		m_radio_button.inner = 0x464646FF;
		m_radio_button.inner_sel = 0x5680C2FF;
		m_radio_button.text = 0xFFFFFFFF;
		m_radio_button.text_sel = 0x000000FF;
		m_radio_button.shaded = true;
		m_radio_button.shadetop = 15;
		m_radio_button.shadedown = -15;

		// Text
		m_text.outline = 0x191919FF;
		m_text.item = 0x5A5A5AFF;
		m_text.inner = 0x999999FF;
		m_text.inner_sel = 0x999999FF;
		m_text.text = 0x000000FF;
		m_text.text_sel = 0xFFFFFFFF;
		m_text.shaded = true;
		m_text.shadetop = 0;
		m_text.shadedown = 25;

		// Option
		m_option.outline = 0x000000FF;
		m_option.item = 0xFFFFFFFF;
		m_option.inner = 0x464646FF;
		m_option.inner_sel = 0x464646FF;
		m_option.text = 0x000000FF;
		m_option.text_sel = 0xFFFFFFFF;
		m_option.shaded = true;
		m_option.shadetop = 15;
		m_option.shadedown = -15;

		// Toggle
		m_toggle.outline = 0x191919FF;
		m_toggle.item = 0x191919FF;
		m_toggle.inner = 0x999999FF;
		m_toggle.inner_sel = 0x646464FF;
		m_toggle.text = 0x000000FF;
		m_toggle.text_sel = 0xFFFFFFFF;

		// Number Fields
		m_number_field.outline = 0x191919FF;
		m_number_field.item = 0xFAFAFAFF;
		m_number_field.inner = 0xB4B4B4FF;
		m_number_field.inner_sel = 0x999999FF;
		m_number_field.text = 0x000000FF;
		m_number_field.text_sel = 0xFFFFFFFF;
		m_number_field.shaded = true;
		m_number_field.shadetop = -20;
		m_number_field.shadedown = 0;

		// Value Slider
		m_number_slider.outline = 0x191919FF;
		m_number_slider.item = 0x808080FF;
		m_number_slider.inner = 0xB4B4B4FF;
		m_number_slider.inner_sel = 0x999999FF;
		m_number_slider.text = 0x000000FF;
		m_number_slider.text_sel = 0xFFFFFFFF;
		m_number_slider.shaded = true;
		m_number_slider.shadetop = -20;
		m_number_slider.shadedown = 0;

		// Box
		m_box.outline = 0x191919FF;
		m_box.item = 0x191919FF;
		m_box.inner = 0x808080FF;
		m_box.inner_sel = 0x646464FF;
		m_box.text = 0x000000FF;
		m_box.text_sel = 0xFFFFFFFF;

		// Menu
		m_menu.outline = 0x000000FF;
		m_menu.item = 0xFFFFFFFF;
		m_menu.inner = 0x464646FF;
		m_menu.inner_sel = 0x464646FF;
		m_menu.text = 0xFFFFFFFF;
		m_menu.text_sel = 0xCCCCCCFF;
		m_menu.shaded = true;
		m_menu.shadetop = 15;
		m_menu.shadedown = -15;

		// Pulldown
		m_pulldown.outline = 0x000000FF;
		m_pulldown.item = 0xFFFFFFFF;
		m_pulldown.inner = 0x3F3F3FFF;
		m_pulldown.inner_sel = 0x5680C2FF;
		m_pulldown.text = 0x000000FF;
		m_pulldown.text_sel = 0x000000FF;

		// Menu Back
		m_menu_back.outline = 0x000000FF;
		m_menu_back.item = 0x646464FF;
		m_menu_back.inner = 0x191919E6;
		m_menu_back.inner_sel = 0x2D2D2DE6;
		m_menu_back.text = 0xA0A0A0FF;
		m_menu_back.text_sel = 0xFFFFFFFF;

		// Menu Item
		m_menu_item.outline = 0x000000FF;
		m_menu_item.item = 0xACACAC80;
		m_menu_item.inner = 0x00000000;
		m_menu_item.inner_sel = 0x5680C2FF;
		m_menu_item.text = 0xFFFFFFFF;
		m_menu_item.text_sel = 0x000000FF;
		m_menu_item.shaded = true;
		m_menu_item.shadetop = 38;
		m_menu_item.shadedown = 0;

		// Tab
		m_tab.outline = 0xBFBFBFFF;
		m_tab.item = 0x4F4F4FFF;
		m_tab.inner = 0x999999FF;
		m_tab.inner_sel = 0x999999FF;
		m_tab.text = 0x000000FF;
		m_tab.text_sel = 0xFFFFFFFF;

		// Tooltip
		m_tooltip.outline = 0x000000FF;
		m_tooltip.item = 0x646464FF;
		m_tooltip.inner = 0x191919E6;
		m_tooltip.inner_sel = 0x2D2D2DE6;
		m_tooltip.text = 0xA0A0A0FF;
		m_tooltip.text_sel = 0xFFFFFFFF;

		// Scroll Bar
		m_scroll.outline = 0x323232FF;
		m_scroll.item = 0x808080FF;
		m_scroll.inner = 0x505050B4;
		m_scroll.inner_sel = 0x646464B4;
		m_scroll.text = 0x000000FF;
		m_scroll.text_sel = 0xFFFFFFFF;
		m_scroll.shaded = true;
		m_scroll.shadetop = 5 + 20;
		m_scroll.shadedown = -5;

		// Progress Bar
		m_progress.outline = 0x000000FF;
		m_progress.item = 0x444444FF;
		m_progress.inner = 0xBEBEBEFF;
		m_progress.inner_sel = 0x646464B4;
		m_progress.text = 0x000000FF;
		m_progress.text_sel = 0xFFFFFFFF;

		// List Item
		m_list_item.outline = 0x000000FF;
		m_list_item.item = 0x000000FF;
		m_list_item.inner = 0x00000000;
		m_list_item.inner_sel = 0x5680C2FF;
		m_list_item.text = 0x000000FF;
		m_list_item.text_sel = 0x000000FF;

		//_theme.panel.header = RGBAf();
		//_theme.panel.back = RGBAf();
		m_menu_shadow_fac = 0.5f;
		m_menu_shadow_width = 12;

		m_dpi = 96;

		xaxis = 0xFF0000FF;
		yaxis = 0x00FF00FF;
		zaxis = 0x0000FFFF;
	}

	void Theme::ParseUINode (const rapidxml::xml_node<>* node)
	{
		std::cout << node->name() << std::endl;

		for (rapidxml::xml_node<>* sub = node->first_node();
				sub != 0; sub = sub->next_sibling())
		{
			ParseWidgetColorNode(sub);
		}
	}

	void Theme::ParseWidgetColorNode (const rapidxml::xml_node<>* node)
	{
		rapidxml::xml_node<>* widget_color_node = node->last_node("ThemeWidgetColors");

		if (widget_color_node) {
			for (rapidxml::xml_attribute<>* attrib =
			        widget_color_node->first_attribute(); attrib != NULL;
			        attrib = attrib->next_attribute()) {
				std::cout << attrib->name() << " = " << attrib->value()
				        << std::endl;
			}
		}
	}

} /* namespace BlendInt */
