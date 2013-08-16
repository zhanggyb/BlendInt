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

#include <iostream>
#include <string.h>

#include <BIL/Theme.hpp>

namespace BIL {

	Theme* Theme::theme = NULL;

	Theme* Theme::instance (void)
	{
		if (theme == NULL) {
			theme = new Theme;
		}

		return theme;
	}

	bool Theme::release (void)
	{
		if (theme == NULL)
			return false;

		delete theme;
		theme = NULL;

		return true;
	}

	WidgetColors::WidgetColors()
		: outline(0.098, 0.098, 0.098),
		  inner (0.6, 0.6, 0.6),
		  inner_sel(0.392, 0.392, 0.392),
		  item (0.098, 0.098, 0.098),
		  text (0.0, 0.0, 0.0),
		  text_sel (1.0, 1.0, 1.0),
		  shaded (false),
		  shadetop (0),
		  shadedown (0),
		  alpha_check (0)
	{
	}

	WidgetStateColors::WidgetStateColors()
		: inner_anim (0.451, 0.745, 0.298),
		  inner_anim_sel (0.353, 0.651, 0.2),
		  inner_key (0.941, 0.922, 0.392),
		  inner_key_sel (0.843, 0.827, 0.294),
		  inner_driven (0.706, 0.0, 1.0),
		  inner_driven_sel(0.6, 0.0, 0.902),
		  blend (0.5),
		  pad (0.5)
	{

	}

	PanelColors::PanelColors()
		: header(0.0, 0.0, 0.0, 0.1),
		  back(0.45, 0.45, 0.45, 0.5),
		  show_header(false),
		  show_back(false),
		  pad(0)
	{

	}

	Theme::Theme ()
	{
		// TODO Auto-generated constructor stub

	}

	Theme::~Theme ()
	{
		// TODO Auto-generated destructor stub
	}

	void Theme::initialize (void)
	{
		// Regular
		// _theme.wcol_regular	// use the default setting in struct constructor

		// Tool
		themeUI_.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_tool.inner = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_tool.inner_sel = RGBAf(0.392, 0.392, 0.392);
		themeUI_.wcol_tool.item = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_tool.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_tool.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Text
		themeUI_.wcol_text.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_text.inner = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_text.inner_sel = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_text.item = RGBAf(0.353, 0.353, 0.353);
		themeUI_.wcol_text.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_text.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Radio Buttons
		themeUI_.wcol_radio.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_radio.inner = RGBAf(0.275, 0.275, 0.275);
		themeUI_.wcol_radio.inner_sel = RGBAf(0.337, 0.502, 0.761);
		themeUI_.wcol_radio.item = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_radio.text = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_radio.text_sel = RGBAf(0.0, 0.0, 0.0);

		// Option
		themeUI_.wcol_option.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_option.inner = RGBAf(0.275, 0.275, 0.275);
		themeUI_.wcol_option.inner_sel = RGBAf(0.275, 0.275, 0.275);
		themeUI_.wcol_option.item = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_option.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_option.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Toggle
		themeUI_.wcol_toggle.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_toggle.inner = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_toggle.inner_sel = RGBAf(0.392, 0.392, 0.392);
		themeUI_.wcol_toggle.item = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_toggle.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_toggle.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Number Fields
		themeUI_.wcol_num.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_num.inner = RGBAf(0.706, 0.706, 0.706);
		themeUI_.wcol_num.inner_sel = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_num.item = RGBAf(0.353, 0.353, 0.353);
		themeUI_.wcol_num.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_num.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Value Slider
		themeUI_.wcol_numslider.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_numslider.inner = RGBAf(0.706, 0.706, 0.706);
		themeUI_.wcol_numslider.inner_sel = RGBAf(0.6, 0.6, 0.6);
		themeUI_.wcol_numslider.item = RGBAf(0.502, 0.502, 0.502);
		themeUI_.wcol_numslider.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_numslider.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Menu
		themeUI_.wcol_menu.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_menu.inner = RGBAf(0.275, 0.275, 0.275);
		themeUI_.wcol_menu.inner_sel = RGBAf(0.275, 0.275, 0.275);
		themeUI_.wcol_menu.item = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_menu.text = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_menu.text_sel = RGBAf(0.8, 0.8, 0.8);

		// Pulldown
		themeUI_.wcol_pulldown.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_pulldown.inner = RGBAf(0.247, 0.247, 0.247);
		themeUI_.wcol_pulldown.inner_sel = RGBAf(0.337, 0.502, 0.761);
		themeUI_.wcol_pulldown.item = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_pulldown.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_pulldown.text_sel = RGBAf(0.0, 0.0, 0.0);

		// Menu Back
		themeUI_.wcol_menu_back.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_menu_back.inner = RGBAf(0.098, 0.098, 0.098, 0.902);
		themeUI_.wcol_menu_back.inner_sel = RGBAf(0.176, 0.176, 0.176, 0.902);
		themeUI_.wcol_menu_back.item = RGBAf(0.392, 0.392, 0.392);
		themeUI_.wcol_menu_back.text = RGBAf(0.627, 0.627, 0.627);
		themeUI_.wcol_menu_back.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Menu Item
		themeUI_.wcol_menu_item.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_menu_item.inner = RGBAf(0.0, 0.0, 0.0, 0.0);
		themeUI_.wcol_menu_item.inner_sel = RGBAf(0.337, 0.502, 0.761);
		themeUI_.wcol_menu_item.item = RGBAf(0.675, 0.675, 0.675, 0.502);
		themeUI_.wcol_menu_item.text = RGBAf(1.0, 1.0, 1.0);
		themeUI_.wcol_menu_item.text_sel = RGBAf(0.0, 0.0, 0.0);

		// Tooltip
		themeUI_.wcol_tooltip.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_tooltip.inner = RGBAf(0.098, 0.098, 0.098, 0.902);
		themeUI_.wcol_tooltip.inner_sel = RGBAf(0.176, 0.176, 0.176, 0.902);
		themeUI_.wcol_tooltip.item = RGBAf(0.392, 0.392, 0.392);
		themeUI_.wcol_tooltip.text = RGBAf(0.627, 0.627, 0.627);
		themeUI_.wcol_tooltip.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Box
		themeUI_.wcol_box.outline = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_box.inner = RGBAf(0.502, 0.502, 0.502);
		themeUI_.wcol_box.inner_sel = RGBAf(0.392, 0.392, 0.392);
		themeUI_.wcol_box.item = RGBAf(0.098, 0.098, 0.098);
		themeUI_.wcol_box.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_box.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Scroll Bar
		themeUI_.wcol_scroll.outline = RGBAf(0.196, 0.196, 0.196);
		themeUI_.wcol_scroll.inner = RGBAf(0.314, 0.314, 0.314, 0.706);
		themeUI_.wcol_scroll.inner_sel = RGBAf(0.392, 0.392, 0.392, 0.706);
		themeUI_.wcol_scroll.item = RGBAf(0.052, 0.052, 0.052);
		themeUI_.wcol_scroll.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_scroll.text_sel = RGBAf(1.0, 1.0, 1.0);

		// Progress Bar
		themeUI_.wcol_progress.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_progress.inner = RGBAf(0.745, 0.745, 0.745);
		themeUI_.wcol_progress.inner_sel = RGBAf(0.392, 0.392, 0.392, 0.706);
		themeUI_.wcol_progress.item = RGBAf(0.267, 0.267, 0.267);
		themeUI_.wcol_progress.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_progress.text_sel = RGBAf(1.0, 1.0, 1.0);

		// List Item
		themeUI_.wcol_list_item.outline = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_list_item.inner = RGBAf(0.0, 0.0, 0.0, 0.0);
		themeUI_.wcol_list_item.inner_sel = RGBAf(0.337, 0.502, 0.761);
		themeUI_.wcol_list_item.item = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_list_item.text = RGBAf(0.0, 0.0, 0.0);
		themeUI_.wcol_list_item.text_sel = RGBAf(0.0, 0.0, 0.0);

		// State
		themeUI_.wcol_state.inner_anim = RGBAf(0.451, 0.745, 0.298);
		themeUI_.wcol_state.inner_anim_sel = RGBAf(0.353, 0.651, 0.2);
		themeUI_.wcol_state.inner_key = RGBAf(0.941, 0.922, 0.392);
		themeUI_.wcol_state.inner_key_sel = RGBAf(0.843, 0.827, 0.294);
		themeUI_.wcol_state.inner_driven = RGBAf(0.706, 0.0, 1.0);
		themeUI_.wcol_state.inner_driven_sel = RGBAf(0.6, 0.0, 0.902);

		//_theme.panel.header = RGBAf();
		//_theme.panel.back = RGBAf();

		themeUI_.xaxis = RGBAf(1.0, 0.0, 0.0);
		themeUI_.yaxis = RGBAf(0.0, 1.0, 0.0);
		themeUI_.zaxis = RGBAf(0.0, 0.0, 1.0);
	}

} /* namespace BIL */
