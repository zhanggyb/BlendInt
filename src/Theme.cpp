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
			theme->themeUI_.tool.outline = 0x191919FF;
			theme->themeUI_.tool.item = 0x191919FF;
			theme->themeUI_.tool.inner = 0x999999FF;
			theme->themeUI_.tool.inner_sel = 0x646464FF;
			theme->themeUI_.tool.text = 0x000000FF;
			theme->themeUI_.tool.text_sel = 0xFFFFFFFF;
			theme->themeUI_.tool.shaded = true;
			theme->themeUI_.tool.shadetop = 15;
			theme->themeUI_.tool.shadedown = -15;

			// Radio Buttons
			theme->themeUI_.radio_button.outline = 0x000000FF;
			theme->themeUI_.radio_button.item = 0xFFFFFFFF;
			theme->themeUI_.radio_button.inner = 0x464646FF;
			theme->themeUI_.radio_button.inner_sel = 0x5680C2FF;
			theme->themeUI_.radio_button.text = 0xFFFFFFFF;
			theme->themeUI_.radio_button.text_sel = 0x000000FF;
			theme->themeUI_.radio_button.shaded = true;
			theme->themeUI_.radio_button.shadetop = 15;
			theme->themeUI_.radio_button.shadedown = -15;

			// Text
			theme->themeUI_.text.outline = 0x191919FF;
			theme->themeUI_.text.item = 0x5A5A5AFF;
			theme->themeUI_.text.inner = 0x999999FF;
			theme->themeUI_.text.inner_sel = 0x999999FF;
			theme->themeUI_.text.text = 0x000000FF;
			theme->themeUI_.text.text_sel = 0xFFFFFFFF;
			theme->themeUI_.text.shaded = true;
			theme->themeUI_.text.shadetop = 0;
			theme->themeUI_.text.shadedown = 25;

			// Option
			theme->themeUI_.option.outline = 0x000000FF;
			theme->themeUI_.option.item = 0xFFFFFFFF;
			theme->themeUI_.option.inner = 0x464646FF;
			theme->themeUI_.option.inner_sel = 0x464646FF;
			theme->themeUI_.option.text = 0x000000FF;
			theme->themeUI_.option.text_sel = 0xFFFFFFFF;
			theme->themeUI_.option.shaded = true;
			theme->themeUI_.option.shadetop = 15;
			theme->themeUI_.option.shadedown = -15;

			// Toggle
			theme->themeUI_.toggle.outline = 0x191919FF;
			theme->themeUI_.toggle.item = 0x191919FF;
			theme->themeUI_.toggle.inner = 0x999999FF;
			theme->themeUI_.toggle.inner_sel = 0x646464FF;
			theme->themeUI_.toggle.text = 0x000000FF;
			theme->themeUI_.toggle.text_sel = 0xFFFFFFFF;

			// Number Fields
			theme->themeUI_.number_field.outline = 0x191919FF;
			theme->themeUI_.number_field.item = 0xFAFAFAFF;
			theme->themeUI_.number_field.inner = 0xB4B4B4FF;
			theme->themeUI_.number_field.inner_sel = 0x999999FF;
			theme->themeUI_.number_field.text = 0x000000FF;
			theme->themeUI_.number_field.text_sel = 0xFFFFFFFF;
			theme->themeUI_.number_field.shaded = true;
			theme->themeUI_.number_field.shadetop = -20;
			theme->themeUI_.number_field.shadedown = 0;

			// Value Slider
			theme->themeUI_.number_slider.outline = 0x191919FF;
			theme->themeUI_.number_slider.item = 0x808080FF;
			theme->themeUI_.number_slider.inner = 0xB4B4B4FF;
			theme->themeUI_.number_slider.inner_sel = 0x999999FF;
			theme->themeUI_.number_slider.text = 0x000000FF;
			theme->themeUI_.number_slider.text_sel = 0xFFFFFFFF;
			theme->themeUI_.number_slider.shaded = true;
			theme->themeUI_.number_slider.shadetop = -20;
			theme->themeUI_.number_slider.shadedown = 0;

			// Box
			theme->themeUI_.box.outline = 0x191919FF;
			theme->themeUI_.box.item = 0x191919FF;
			theme->themeUI_.box.inner = 0x808080FF;
			theme->themeUI_.box.inner_sel = 0x646464FF;
			theme->themeUI_.box.text = 0x000000FF;
			theme->themeUI_.box.text_sel = 0xFFFFFFFF;

			// Menu
			theme->themeUI_.menu.outline = 0x000000FF;
			theme->themeUI_.menu.item = 0xFFFFFFFF;
			theme->themeUI_.menu.inner = 0x464646FF;
			theme->themeUI_.menu.inner_sel = 0x464646FF;
			theme->themeUI_.menu.text = 0xFFFFFFFF;
			theme->themeUI_.menu.text_sel = 0xCCCCCCFF;
			theme->themeUI_.menu.shaded = true;
			theme->themeUI_.menu.shadetop = 15;
			theme->themeUI_.menu.shadedown = -15;

			// Pulldown
			theme->themeUI_.pulldown.outline = 0x000000FF;
			theme->themeUI_.pulldown.item = 0xFFFFFFFF;
			theme->themeUI_.pulldown.inner = 0x3F3F3FFF;
			theme->themeUI_.pulldown.inner_sel = 0x5680C2FF;
			theme->themeUI_.pulldown.text = 0x000000FF;
			theme->themeUI_.pulldown.text_sel = 0x000000FF;

			// Menu Back
			theme->themeUI_.menu_back.outline = 0x000000FF;
			theme->themeUI_.menu_back.item = 0x646464FF;
			theme->themeUI_.menu_back.inner = 0x191919E6;
			theme->themeUI_.menu_back.inner_sel = 0x2D2D2DE6;
			theme->themeUI_.menu_back.text = 0xA0A0A0FF;
			theme->themeUI_.menu_back.text_sel = 0xFFFFFFFF;

			// Tooltip
			theme->themeUI_.tooltip.outline = 0x000000FF;
			theme->themeUI_.tooltip.item = 0x646464FF;
			theme->themeUI_.tooltip.inner = 0x191919E6;
			theme->themeUI_.tooltip.inner_sel = 0x2D2D2DE6;
			theme->themeUI_.tooltip.text = 0xA0A0A0FF;
			theme->themeUI_.tooltip.text_sel = 0xFFFFFFFF;

			// Menu Item
			theme->themeUI_.menu_item.outline = 0x000000FF;
			theme->themeUI_.menu_item.item = 0xACACAC80;
			theme->themeUI_.menu_item.inner = 0x00000000;
			theme->themeUI_.menu_item.inner_sel = 0x5680C2FF;
			theme->themeUI_.menu_item.text = 0xFFFFFFFF;
			theme->themeUI_.menu_item.text_sel = 0x000000FF;
			theme->themeUI_.menu_item.shaded = true;
			theme->themeUI_.menu_item.shadetop = 38;
			theme->themeUI_.menu_item.shadedown = 0;

			// Scroll Bar
			theme->themeUI_.scroll.outline = 0x323232FF;
			theme->themeUI_.scroll.item = 0x808080FF;
			theme->themeUI_.scroll.inner = 0x505050B4;
			theme->themeUI_.scroll.inner_sel = 0x646464B4;
			theme->themeUI_.scroll.text = 0x000000FF;
			theme->themeUI_.scroll.text_sel = 0xFFFFFFFF;
			theme->themeUI_.scroll.shaded = true;
			theme->themeUI_.scroll.shadetop = 5;
			theme->themeUI_.scroll.shadedown = -5;

			// Progress Bar
			theme->themeUI_.progress.outline = 0x000000FF;
			theme->themeUI_.progress.item = 0x444444FF;
			theme->themeUI_.progress.inner = 0xBEBEBEFF;
			theme->themeUI_.progress.inner_sel = 0x646464B4;
			theme->themeUI_.progress.text = 0x000000FF;
			theme->themeUI_.progress.text_sel = 0xFFFFFFFF;

			// List Item
			theme->themeUI_.list_item.outline = 0x000000FF;
			theme->themeUI_.list_item.item = 0x000000FF;
			theme->themeUI_.list_item.inner = 0x00000000;
			theme->themeUI_.list_item.inner_sel = 0x5680C2FF;
			theme->themeUI_.list_item.text = 0x000000FF;
			theme->themeUI_.list_item.text_sel = 0x000000FF;

			// State
			theme->themeUI_.state.inner_anim = 0x73BE4CFF;
			theme->themeUI_.state.inner_anim_sel = 0x5AA633FF;
			theme->themeUI_.state.inner_key = 0xF0EB64FF;
			theme->themeUI_.state.inner_key_sel = 0xD7D34BFF;
			theme->themeUI_.state.inner_driven = 0xB400FFFF;
			theme->themeUI_.state.inner_driven_sel = 0x9900E6FF;

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
