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
		: outline(0x191919FF),
		  item (0x191919FF),
		  inner (0x999999FF),
		  inner_sel(0x646464FF),
		  text (0x000000FF),
		  text_sel (0xFFFFFFFF),
		  shaded (false),
		  shadetop (0),
		  shadedown (0),
		  alpha_check (0)
	{
		inner_highlight.HighlightFrom (inner);
	}

	WidgetStateColors::WidgetStateColors()
		: inner_anim (0x73BE4CFF),
		  inner_anim_sel (0x5AA633FF),
		  inner_key (0xF0EB64FF),
		  inner_key_sel (0xD7D34BFF),
		  inner_driven (0xB400FFFF),
		  inner_driven_sel(0x9900E6FF),
		  blend (0.5),
		  pad (0.5)
	{

	}

	PanelColors::PanelColors()
		: header(0x00000019),
		  back(0x72727280),
		  show_header(false),
		  show_back(false),
		  pad(0)
	{

	}

	Theme::FontStyle::FontStyle()
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

	void Theme::initialize (void)
	{
		// Regular
		// _theme.wcol_regular	// use the default setting in struct constructor

		// Tool
		themeUI_.wcol_tool.outline = 0x191919FF;
		themeUI_.wcol_tool.item = 0x191919FF;
		themeUI_.wcol_tool.inner = 0x999999FF;
		themeUI_.wcol_tool.inner_highlight.HighlightFrom(0x999999FF);
		themeUI_.wcol_tool.inner_sel = 0x646464FF;
		themeUI_.wcol_tool.text = 0x000000FF;
		themeUI_.wcol_tool.text_sel = 0xFFFFFFFF;

		// Radio Buttons
		themeUI_.wcol_radio.outline = 0x000000FF;
		themeUI_.wcol_radio.item = 0xFFFFFFFF;
		themeUI_.wcol_radio.inner = 0x464646FF;
		themeUI_.wcol_radio.inner_highlight.HighlightFrom(0x464646FF);
		themeUI_.wcol_radio.inner_sel = 0x5680C2FF;
		themeUI_.wcol_radio.text = 0xFFFFFFFF;
		themeUI_.wcol_radio.text_sel = 0x000000FF;

		// Text
		themeUI_.wcol_text.outline = 0x191919FF;
		themeUI_.wcol_text.item = 0x5A5A5AFF;
		themeUI_.wcol_text.inner = 0x999999FF;
		themeUI_.wcol_text.inner_sel = 0x999999FF;
		themeUI_.wcol_text.text = 0x000000FF;
		themeUI_.wcol_text.text_sel = 0xFFFFFFFF;

		// Option
		themeUI_.wcol_option.outline = 0x000000FF;
		themeUI_.wcol_option.item = 0xFFFFFFFF;
		themeUI_.wcol_option.inner = 0x464646FF;
		themeUI_.wcol_option.inner_highlight.HighlightFrom(0x464646FF);
		themeUI_.wcol_option.inner_sel = 0x464646FF;
		themeUI_.wcol_option.text = 0x000000FF;
		themeUI_.wcol_option.text_sel = 0xFFFFFFFF;

		// Toggle
		themeUI_.wcol_toggle.outline = 0x191919FF;
		themeUI_.wcol_toggle.item = 0x191919FF;
		themeUI_.wcol_toggle.inner = 0x999999FF;
		themeUI_.wcol_toggle.inner_highlight.HighlightFrom(0x999999FF);
		themeUI_.wcol_toggle.inner_sel = 0x646464FF;
		themeUI_.wcol_toggle.text = 0x000000FF;
		themeUI_.wcol_toggle.text_sel = 0xFFFFFFFF;

		// Number Fields
		themeUI_.wcol_num.outline = 0x191919FF;
		themeUI_.wcol_num.item = 0xFAFAFAFF;
		themeUI_.wcol_num.inner = 0xB4B4B4FF;
		themeUI_.wcol_num.inner_highlight.HighlightFrom(0xB4B4B4FF);
		themeUI_.wcol_num.inner_sel = 0x999999FF;
		themeUI_.wcol_num.text = 0x000000FF;
		themeUI_.wcol_num.text_sel = 0xFFFFFFFF;

		// Value Slider
		themeUI_.wcol_numslider.outline = 0x191919FF;
		themeUI_.wcol_numslider.item = 0x808080FF;
		themeUI_.wcol_numslider.inner = 0xB4B4B4FF;
		themeUI_.wcol_numslider.inner_highlight.HighlightFrom(0xB4B4B4FF);
		themeUI_.wcol_numslider.inner_sel = 0x999999FF;
		themeUI_.wcol_numslider.text = 0x000000FF;
		themeUI_.wcol_numslider.text_sel = 0xFFFFFFFF;

		// Box
		themeUI_.wcol_box.outline = 0x191919FF;
		themeUI_.wcol_box.item = 0x191919FF;
		themeUI_.wcol_box.inner = 0x808080FF;
		themeUI_.wcol_box.inner_highlight.HighlightFrom(0x808080FF);
		themeUI_.wcol_box.inner_sel = 0x646464FF;
		themeUI_.wcol_box.text = 0x000000FF;
		themeUI_.wcol_box.text_sel = 0xFFFFFFFF;

		// Menu
		themeUI_.wcol_menu.outline = 0x000000FF;
		themeUI_.wcol_menu.item = 0xFFFFFFFF;
		themeUI_.wcol_menu.inner = 0x464646FF;
		themeUI_.wcol_menu.inner_highlight.HighlightFrom(0x464646FF);
		themeUI_.wcol_menu.inner_sel = 0x464646FF;
		themeUI_.wcol_menu.text = 0xFFFFFFFF;
		themeUI_.wcol_menu.text_sel = 0xCCCCCCFF;

		// Pulldown
		themeUI_.wcol_pulldown.outline = 0x000000FF;
		themeUI_.wcol_pulldown.item = 0xFFFFFFFF;
		themeUI_.wcol_pulldown.inner = 0x3F3F3FFF;
		themeUI_.wcol_pulldown.inner_highlight.HighlightFrom(0x3F3F3FFF);
		themeUI_.wcol_pulldown.inner_sel = 0x5680C2FF;
		themeUI_.wcol_pulldown.text = 0x000000FF;
		themeUI_.wcol_pulldown.text_sel = 0x000000FF;

		// Menu Back
		themeUI_.wcol_menu_back.outline = 0x000000FF;
		themeUI_.wcol_menu_back.item = 0x646464FF;
		themeUI_.wcol_menu_back.inner = 0x191919E6;
		themeUI_.wcol_menu_back.inner_highlight.HighlightFrom(0x191919E6);
		themeUI_.wcol_menu_back.inner_sel = 0x2D2D2DE6;
		themeUI_.wcol_menu_back.text = 0xA0A0A0FF;
		themeUI_.wcol_menu_back.text_sel = 0xFFFFFFFF;

		// Tooltip
		themeUI_.wcol_tooltip.outline = 0x000000FF;
		themeUI_.wcol_tooltip.item = 0x646464FF;
		themeUI_.wcol_tooltip.inner = 0x191919E6;
		themeUI_.wcol_tooltip.inner_highlight.HighlightFrom(0x191919E6);
		themeUI_.wcol_tooltip.inner_sel = 0x2D2D2DE6;
		themeUI_.wcol_tooltip.text = 0xA0A0A0FF;
		themeUI_.wcol_tooltip.text_sel = 0xFFFFFFFF;

		// Menu Item
		themeUI_.wcol_menu_item.outline = 0x000000FF;
		themeUI_.wcol_menu_item.item = 0xACACAC80;
		themeUI_.wcol_menu_item.inner = 0x00000000;
		themeUI_.wcol_menu_item.inner_highlight.HighlightFrom(0x00000000); // nothing
		themeUI_.wcol_menu_item.inner_sel = 0x5680C2FF;
		themeUI_.wcol_menu_item.text = 0xFFFFFFFF;
		themeUI_.wcol_menu_item.text_sel = 0x000000FF;


		// Scroll Bar
		themeUI_.wcol_scroll.outline = 0x323232FF;
		themeUI_.wcol_scroll.item = 0x808080FF;
		themeUI_.wcol_scroll.inner = 0x505050B4;
		themeUI_.wcol_scroll.inner_highlight.HighlightFrom(0x505050B4);
		themeUI_.wcol_scroll.inner_sel = 0x646464B4;
		themeUI_.wcol_scroll.text = 0x000000FF;
		themeUI_.wcol_scroll.text_sel = 0xFFFFFFFF;

		// Progress Bar
		themeUI_.wcol_progress.outline = 0x000000FF;
		themeUI_.wcol_progress.item = 0x444444FF;
		themeUI_.wcol_progress.inner = 0xBEBEBEFF;
		themeUI_.wcol_progress.inner_highlight.HighlightFrom(0xBEBEBEFF);
		themeUI_.wcol_progress.inner_sel = 0x646464B4;
		themeUI_.wcol_progress.text = 0x000000FF;
		themeUI_.wcol_progress.text_sel = 0xFFFFFFFF;

		// List Item
		themeUI_.wcol_list_item.outline = 0x000000FF;
		themeUI_.wcol_list_item.item = 0x000000FF;
		themeUI_.wcol_list_item.inner = 0x00000000;
		themeUI_.wcol_list_item.inner_highlight.HighlightFrom(0x00000000);
		themeUI_.wcol_list_item.inner_sel = 0x5680C2FF;
		themeUI_.wcol_list_item.text = 0x000000FF;
		themeUI_.wcol_list_item.text_sel = 0x000000FF;

		// State
		themeUI_.wcol_state.inner_anim = 0x73BE4CFF;
		themeUI_.wcol_state.inner_anim_sel = 0x5AA633FF;
		themeUI_.wcol_state.inner_key = 0xF0EB64FF;
		themeUI_.wcol_state.inner_key_sel = 0xD7D34BFF;
		themeUI_.wcol_state.inner_driven = 0xB400FFFF;
		themeUI_.wcol_state.inner_driven_sel = 0x9900E6FF;

		//_theme.panel.header = RGBAf();
		//_theme.panel.back = RGBAf();

		themeUI_.xaxis = 0xFF0000FF;
		themeUI_.yaxis = 0x00FF00FF;
		themeUI_.zaxis = 0x0000FFFF;
	}

} /* namespace BIL */
