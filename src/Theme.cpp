/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <BILO/Theme.hpp>

namespace BILO {

	Theme* Theme::theme = 0;

	Theme* Theme::instance (void)
	{
		if (!theme) {
			std::cerr << "The theme is not initialized successfully! Exit"
			        << endl;
			exit(EXIT_FAILURE);
		}

		return theme;
	}

	bool Theme::initialize ()
	{
		bool result = true;

		if (!theme) {
			theme = new Theme;
		}

		if (!theme)
			result = false;

		if (theme) {
			// Regular
			// _theme.wcol_regular	// use the default setting in struct constructor

			// Tool
			theme->themeUI_.wcol_tool.outline = 0x191919FF;
			theme->themeUI_.wcol_tool.item = 0x191919FF;
			theme->themeUI_.wcol_tool.inner = 0x999999FF;
			theme->themeUI_.wcol_tool.inner_sel = 0x646464FF;
			theme->themeUI_.wcol_tool.text = 0x000000FF;
			theme->themeUI_.wcol_tool.text_sel = 0xFFFFFFFF;

			// Radio Buttons
			theme->themeUI_.wcol_radio.outline = 0x000000FF;
			theme->themeUI_.wcol_radio.item = 0xFFFFFFFF;
			theme->themeUI_.wcol_radio.inner = 0x464646FF;
			theme->themeUI_.wcol_radio.inner_sel = 0x5680C2FF;
			theme->themeUI_.wcol_radio.text = 0xFFFFFFFF;
			theme->themeUI_.wcol_radio.text_sel = 0x000000FF;

			// Text
			theme->themeUI_.wcol_text.outline = 0x191919FF;
			theme->themeUI_.wcol_text.item = 0x5A5A5AFF;
			theme->themeUI_.wcol_text.inner = 0x999999FF;
			theme->themeUI_.wcol_text.inner_sel = 0x999999FF;
			theme->themeUI_.wcol_text.text = 0x000000FF;
			theme->themeUI_.wcol_text.text_sel = 0xFFFFFFFF;

			// Option
			theme->themeUI_.wcol_option.outline = 0x000000FF;
			theme->themeUI_.wcol_option.item = 0xFFFFFFFF;
			theme->themeUI_.wcol_option.inner = 0x464646FF;
			theme->themeUI_.wcol_option.inner_sel = 0x464646FF;
			theme->themeUI_.wcol_option.text = 0x000000FF;
			theme->themeUI_.wcol_option.text_sel = 0xFFFFFFFF;

			// Toggle
			theme->themeUI_.wcol_toggle.outline = 0x191919FF;
			theme->themeUI_.wcol_toggle.item = 0x191919FF;
			theme->themeUI_.wcol_toggle.inner = 0x999999FF;
			theme->themeUI_.wcol_toggle.inner_sel = 0x646464FF;
			theme->themeUI_.wcol_toggle.text = 0x000000FF;
			theme->themeUI_.wcol_toggle.text_sel = 0xFFFFFFFF;

			// Number Fields
			theme->themeUI_.wcol_num.outline = 0x191919FF;
			theme->themeUI_.wcol_num.item = 0xFAFAFAFF;
			theme->themeUI_.wcol_num.inner = 0xB4B4B4FF;
			theme->themeUI_.wcol_num.inner_sel = 0x999999FF;
			theme->themeUI_.wcol_num.text = 0x000000FF;
			theme->themeUI_.wcol_num.text_sel = 0xFFFFFFFF;

			// Value Slider
			theme->themeUI_.wcol_numslider.outline = 0x191919FF;
			theme->themeUI_.wcol_numslider.item = 0x808080FF;
			theme->themeUI_.wcol_numslider.inner = 0xB4B4B4FF;
			theme->themeUI_.wcol_numslider.inner_sel = 0x999999FF;
			theme->themeUI_.wcol_numslider.text = 0x000000FF;
			theme->themeUI_.wcol_numslider.text_sel = 0xFFFFFFFF;

			// Box
			theme->themeUI_.wcol_box.outline = 0x191919FF;
			theme->themeUI_.wcol_box.item = 0x191919FF;
			theme->themeUI_.wcol_box.inner = 0x808080FF;
			theme->themeUI_.wcol_box.inner_sel = 0x646464FF;
			theme->themeUI_.wcol_box.text = 0x000000FF;
			theme->themeUI_.wcol_box.text_sel = 0xFFFFFFFF;

			// Menu
			theme->themeUI_.wcol_menu.outline = 0x000000FF;
			theme->themeUI_.wcol_menu.item = 0xFFFFFFFF;
			theme->themeUI_.wcol_menu.inner = 0x464646FF;
			theme->themeUI_.wcol_menu.inner_sel = 0x464646FF;
			theme->themeUI_.wcol_menu.text = 0xFFFFFFFF;
			theme->themeUI_.wcol_menu.text_sel = 0xCCCCCCFF;

			// Pulldown
			theme->themeUI_.wcol_pulldown.outline = 0x000000FF;
			theme->themeUI_.wcol_pulldown.item = 0xFFFFFFFF;
			theme->themeUI_.wcol_pulldown.inner = 0x3F3F3FFF;
			theme->themeUI_.wcol_pulldown.inner_sel = 0x5680C2FF;
			theme->themeUI_.wcol_pulldown.text = 0x000000FF;
			theme->themeUI_.wcol_pulldown.text_sel = 0x000000FF;

			// Menu Back
			theme->themeUI_.wcol_menu_back.outline = 0x000000FF;
			theme->themeUI_.wcol_menu_back.item = 0x646464FF;
			theme->themeUI_.wcol_menu_back.inner = 0x191919E6;
			theme->themeUI_.wcol_menu_back.inner_sel = 0x2D2D2DE6;
			theme->themeUI_.wcol_menu_back.text = 0xA0A0A0FF;
			theme->themeUI_.wcol_menu_back.text_sel = 0xFFFFFFFF;

			// Tooltip
			theme->themeUI_.wcol_tooltip.outline = 0x000000FF;
			theme->themeUI_.wcol_tooltip.item = 0x646464FF;
			theme->themeUI_.wcol_tooltip.inner = 0x191919E6;
			theme->themeUI_.wcol_tooltip.inner_sel = 0x2D2D2DE6;
			theme->themeUI_.wcol_tooltip.text = 0xA0A0A0FF;
			theme->themeUI_.wcol_tooltip.text_sel = 0xFFFFFFFF;

			// Menu Item
			theme->themeUI_.wcol_menu_item.outline = 0x000000FF;
			theme->themeUI_.wcol_menu_item.item = 0xACACAC80;
			theme->themeUI_.wcol_menu_item.inner = 0x00000000;
			theme->themeUI_.wcol_menu_item.inner_sel = 0x5680C2FF;
			theme->themeUI_.wcol_menu_item.text = 0xFFFFFFFF;
			theme->themeUI_.wcol_menu_item.text_sel = 0x000000FF;

			// Scroll Bar
			theme->themeUI_.wcol_scroll.outline = 0x323232FF;
			theme->themeUI_.wcol_scroll.item = 0x808080FF;
			theme->themeUI_.wcol_scroll.inner = 0x505050B4;
			theme->themeUI_.wcol_scroll.inner_sel = 0x646464B4;
			theme->themeUI_.wcol_scroll.text = 0x000000FF;
			theme->themeUI_.wcol_scroll.text_sel = 0xFFFFFFFF;

			// Progress Bar
			theme->themeUI_.wcol_progress.outline = 0x000000FF;
			theme->themeUI_.wcol_progress.item = 0x444444FF;
			theme->themeUI_.wcol_progress.inner = 0xBEBEBEFF;
			theme->themeUI_.wcol_progress.inner_sel = 0x646464B4;
			theme->themeUI_.wcol_progress.text = 0x000000FF;
			theme->themeUI_.wcol_progress.text_sel = 0xFFFFFFFF;

			// List Item
			theme->themeUI_.wcol_list_item.outline = 0x000000FF;
			theme->themeUI_.wcol_list_item.item = 0x000000FF;
			theme->themeUI_.wcol_list_item.inner = 0x00000000;
			theme->themeUI_.wcol_list_item.inner_sel = 0x5680C2FF;
			theme->themeUI_.wcol_list_item.text = 0x000000FF;
			theme->themeUI_.wcol_list_item.text_sel = 0x000000FF;

			// State
			theme->themeUI_.wcol_state.inner_anim = 0x73BE4CFF;
			theme->themeUI_.wcol_state.inner_anim_sel = 0x5AA633FF;
			theme->themeUI_.wcol_state.inner_key = 0xF0EB64FF;
			theme->themeUI_.wcol_state.inner_key_sel = 0xD7D34BFF;
			theme->themeUI_.wcol_state.inner_driven = 0xB400FFFF;
			theme->themeUI_.wcol_state.inner_driven_sel = 0x9900E6FF;

			//_theme.panel.header = RGBAf();
			//_theme.panel.back = RGBAf();

			theme->themeUI_.xaxis = 0xFF0000FF;
			theme->themeUI_.yaxis = 0x00FF00FF;
			theme->themeUI_.zaxis = 0x0000FFFF;

		}

		return result;
	}

	void Theme::release (void)
	{
		if (theme) {
			delete theme;
			theme = 0;
		}
	}

	WidgetColors::WidgetColors ()
			: outline(0x191919FF), item(0x191919FF), inner(0x999999FF), inner_sel(
			        0x646464FF), text(0x000000FF), text_sel(0xFFFFFFFF), shaded(
			        false), shadetop(0), shadedown(0), alpha_check(0)
	{
	}

	WidgetStateColors::WidgetStateColors ()
			: inner_anim(0x73BE4CFF), inner_anim_sel(0x5AA633FF), inner_key(
			        0xF0EB64FF), inner_key_sel(0xD7D34BFF), inner_driven(
			        0xB400FFFF), inner_driven_sel(0x9900E6FF), blend(0.5), pad(
			        0.5)
	{

	}

	PanelColors::PanelColors ()
			: header(0x00000019), back(0x72727280), show_header(false), show_back(
			        false), pad(0)
	{

	}

	Theme::FontStyle::FontStyle ()
	{
		// TODO: init fonts in fontconfig
		regular = Font("Droid Sans", 9);
	}

	Theme::Theme ()
	{
		// TODO Auto-generated constructor stub

	}

	Theme::~Theme ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BILO */
