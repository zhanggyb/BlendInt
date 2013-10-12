/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
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

#include <BlendInt/Theme.hpp>

namespace BlendInt {

	ThemeManager* ThemeManager::theme_manager = 0;

	ThemeManager* ThemeManager::instance (void)
	{
		if (!theme_manager) {
			std::cerr << "The theme is not initialized successfully! Exit"
			        << endl;
			exit(EXIT_FAILURE);
		}

		return theme_manager;
	}

	bool ThemeManager::initialize ()
	{
		bool result = true;

		if (!theme_manager) {
			theme_manager = new ThemeManager;
		}

		if (!theme_manager)
			result = false;

		if (theme_manager) {
			// Regular
			// _theme.wcol_regular	// use the default setting in struct constructor

			// Tool
			theme_manager->m_themes.tool.outline = 0x191919FF;
			theme_manager->m_themes.tool.item = 0x191919FF;
			theme_manager->m_themes.tool.inner = 0x999999FF;
			theme_manager->m_themes.tool.inner_sel = 0x646464FF;
			theme_manager->m_themes.tool.text = 0x000000FF;
			theme_manager->m_themes.tool.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.tool.shaded = true;
			theme_manager->m_themes.tool.shadetop = 15;
			theme_manager->m_themes.tool.shadedown = -15;

			// Radio Buttons
			theme_manager->m_themes.radio_button.outline = 0x000000FF;
			theme_manager->m_themes.radio_button.item = 0xFFFFFFFF;
			theme_manager->m_themes.radio_button.inner = 0x464646FF;
			theme_manager->m_themes.radio_button.inner_sel = 0x5680C2FF;
			theme_manager->m_themes.radio_button.text = 0xFFFFFFFF;
			theme_manager->m_themes.radio_button.text_sel = 0x000000FF;
			theme_manager->m_themes.radio_button.shaded = true;
			theme_manager->m_themes.radio_button.shadetop = 15;
			theme_manager->m_themes.radio_button.shadedown = -15;

			// Text
			theme_manager->m_themes.text.outline = 0x191919FF;
			theme_manager->m_themes.text.item = 0x5A5A5AFF;
			theme_manager->m_themes.text.inner = 0x999999FF;
			theme_manager->m_themes.text.inner_sel = 0x999999FF;
			theme_manager->m_themes.text.text = 0x000000FF;
			theme_manager->m_themes.text.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.text.shaded = true;
			theme_manager->m_themes.text.shadetop = 0;
			theme_manager->m_themes.text.shadedown = 25;

			// Option
			theme_manager->m_themes.option.outline = 0x000000FF;
			theme_manager->m_themes.option.item = 0xFFFFFFFF;
			theme_manager->m_themes.option.inner = 0x464646FF;
			theme_manager->m_themes.option.inner_sel = 0x464646FF;
			theme_manager->m_themes.option.text = 0x000000FF;
			theme_manager->m_themes.option.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.option.shaded = true;
			theme_manager->m_themes.option.shadetop = 15;
			theme_manager->m_themes.option.shadedown = -15;

			// Toggle
			theme_manager->m_themes.toggle.outline = 0x191919FF;
			theme_manager->m_themes.toggle.item = 0x191919FF;
			theme_manager->m_themes.toggle.inner = 0x999999FF;
			theme_manager->m_themes.toggle.inner_sel = 0x646464FF;
			theme_manager->m_themes.toggle.text = 0x000000FF;
			theme_manager->m_themes.toggle.text_sel = 0xFFFFFFFF;

			// Number Fields
			theme_manager->m_themes.number_field.outline = 0x191919FF;
			theme_manager->m_themes.number_field.item = 0xFAFAFAFF;
			theme_manager->m_themes.number_field.inner = 0xB4B4B4FF;
			theme_manager->m_themes.number_field.inner_sel = 0x999999FF;
			theme_manager->m_themes.number_field.text = 0x000000FF;
			theme_manager->m_themes.number_field.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.number_field.shaded = true;
			theme_manager->m_themes.number_field.shadetop = -20;
			theme_manager->m_themes.number_field.shadedown = 0;

			// Value Slider
			theme_manager->m_themes.number_slider.outline = 0x191919FF;
			theme_manager->m_themes.number_slider.item = 0x808080FF;
			theme_manager->m_themes.number_slider.inner = 0xB4B4B4FF;
			theme_manager->m_themes.number_slider.inner_sel = 0x999999FF;
			theme_manager->m_themes.number_slider.text = 0x000000FF;
			theme_manager->m_themes.number_slider.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.number_slider.shaded = true;
			theme_manager->m_themes.number_slider.shadetop = -20;
			theme_manager->m_themes.number_slider.shadedown = 0;

			// Box
			theme_manager->m_themes.box.outline = 0x191919FF;
			theme_manager->m_themes.box.item = 0x191919FF;
			theme_manager->m_themes.box.inner = 0x808080FF;
			theme_manager->m_themes.box.inner_sel = 0x646464FF;
			theme_manager->m_themes.box.text = 0x000000FF;
			theme_manager->m_themes.box.text_sel = 0xFFFFFFFF;

			// Menu
			theme_manager->m_themes.menu.outline = 0x000000FF;
			theme_manager->m_themes.menu.item = 0xFFFFFFFF;
			theme_manager->m_themes.menu.inner = 0x464646FF;
			theme_manager->m_themes.menu.inner_sel = 0x464646FF;
			theme_manager->m_themes.menu.text = 0xFFFFFFFF;
			theme_manager->m_themes.menu.text_sel = 0xCCCCCCFF;
			theme_manager->m_themes.menu.shaded = true;
			theme_manager->m_themes.menu.shadetop = 15;
			theme_manager->m_themes.menu.shadedown = -15;

			// Pulldown
			theme_manager->m_themes.pulldown.outline = 0x000000FF;
			theme_manager->m_themes.pulldown.item = 0xFFFFFFFF;
			theme_manager->m_themes.pulldown.inner = 0x3F3F3FFF;
			theme_manager->m_themes.pulldown.inner_sel = 0x5680C2FF;
			theme_manager->m_themes.pulldown.text = 0x000000FF;
			theme_manager->m_themes.pulldown.text_sel = 0x000000FF;

			// Menu Back
			theme_manager->m_themes.menu_back.outline = 0x000000FF;
			theme_manager->m_themes.menu_back.item = 0x646464FF;
			theme_manager->m_themes.menu_back.inner = 0x191919E6;
			theme_manager->m_themes.menu_back.inner_sel = 0x2D2D2DE6;
			theme_manager->m_themes.menu_back.text = 0xA0A0A0FF;
			theme_manager->m_themes.menu_back.text_sel = 0xFFFFFFFF;

			// Tooltip
			theme_manager->m_themes.tooltip.outline = 0x000000FF;
			theme_manager->m_themes.tooltip.item = 0x646464FF;
			theme_manager->m_themes.tooltip.inner = 0x191919E6;
			theme_manager->m_themes.tooltip.inner_sel = 0x2D2D2DE6;
			theme_manager->m_themes.tooltip.text = 0xA0A0A0FF;
			theme_manager->m_themes.tooltip.text_sel = 0xFFFFFFFF;

			// Menu Item
			theme_manager->m_themes.menu_item.outline = 0x000000FF;
			theme_manager->m_themes.menu_item.item = 0xACACAC80;
			theme_manager->m_themes.menu_item.inner = 0x00000000;
			theme_manager->m_themes.menu_item.inner_sel = 0x5680C2FF;
			theme_manager->m_themes.menu_item.text = 0xFFFFFFFF;
			theme_manager->m_themes.menu_item.text_sel = 0x000000FF;
			theme_manager->m_themes.menu_item.shaded = true;
			theme_manager->m_themes.menu_item.shadetop = 38;
			theme_manager->m_themes.menu_item.shadedown = 0;

			// Scroll Bar
			theme_manager->m_themes.scroll.outline = 0x323232FF;
			theme_manager->m_themes.scroll.item = 0x808080FF;
			theme_manager->m_themes.scroll.inner = 0x505050B4;
			theme_manager->m_themes.scroll.inner_sel = 0x646464B4;
			theme_manager->m_themes.scroll.text = 0x000000FF;
			theme_manager->m_themes.scroll.text_sel = 0xFFFFFFFF;
			theme_manager->m_themes.scroll.shaded = true;
			theme_manager->m_themes.scroll.shadetop = 5;
			theme_manager->m_themes.scroll.shadedown = -5;

			// Progress Bar
			theme_manager->m_themes.progress.outline = 0x000000FF;
			theme_manager->m_themes.progress.item = 0x444444FF;
			theme_manager->m_themes.progress.inner = 0xBEBEBEFF;
			theme_manager->m_themes.progress.inner_sel = 0x646464B4;
			theme_manager->m_themes.progress.text = 0x000000FF;
			theme_manager->m_themes.progress.text_sel = 0xFFFFFFFF;

			// List Item
			theme_manager->m_themes.list_item.outline = 0x000000FF;
			theme_manager->m_themes.list_item.item = 0x000000FF;
			theme_manager->m_themes.list_item.inner = 0x00000000;
			theme_manager->m_themes.list_item.inner_sel = 0x5680C2FF;
			theme_manager->m_themes.list_item.text = 0x000000FF;
			theme_manager->m_themes.list_item.text_sel = 0x000000FF;

			// State
			theme_manager->m_themes.state.inner_anim = 0x73BE4CFF;
			theme_manager->m_themes.state.inner_anim_sel = 0x5AA633FF;
			theme_manager->m_themes.state.inner_key = 0xF0EB64FF;
			theme_manager->m_themes.state.inner_key_sel = 0xD7D34BFF;
			theme_manager->m_themes.state.inner_driven = 0xB400FFFF;
			theme_manager->m_themes.state.inner_driven_sel = 0x9900E6FF;

			//_theme.panel.header = RGBAf();
			//_theme.panel.back = RGBAf();

			theme_manager->m_themes.xaxis = 0xFF0000FF;
			theme_manager->m_themes.yaxis = 0x00FF00FF;
			theme_manager->m_themes.zaxis = 0x0000FFFF;
		}

		return result;
	}

	void ThemeManager::release (void)
	{
		if (theme_manager) {
			delete theme_manager;
			theme_manager = 0;
		}
	}

	WidgetTheme::WidgetTheme ()
			: outline(0x191919FF), item(0x191919FF), inner(0x999999FF), inner_sel(
			        0x646464FF), text(0x000000FF), text_sel(0xFFFFFFFF), shaded(
			        false), shadetop(0), shadedown(0), alpha_check(0)
	{
	}

	WidgetStateTheme::WidgetStateTheme ()
			: inner_anim(0x73BE4CFF), inner_anim_sel(0x5AA633FF), inner_key(
			        0xF0EB64FF), inner_key_sel(0xD7D34BFF), inner_driven(
			        0xB400FFFF), inner_driven_sel(0x9900E6FF), blend(0.5), pad(
			        0.5)
	{

	}

	PanelTheme::PanelTheme ()
			: header(0x00000019), back(0x72727280), show_header(false), show_back(
			        false), pad(0)
	{

	}

	ThemeManager::FontStyle::FontStyle ()
	{
		// TODO: init fonts in fontconfig
		regular = Font("Droid Sans", 9);
	}

	ThemeManager::ThemeManager ()
	{
		// TODO Auto-generated constructor stub

	}

	ThemeManager::~ThemeManager ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BlendInt */
