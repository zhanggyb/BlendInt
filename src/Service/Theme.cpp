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

#include <boost/filesystem.hpp>
#include <rapidxml_print.hpp>
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
		namespace fs = boost::filesystem;

		bool ret = false;

		fs::path path(filepath);
		if(fs::is_directory(path)) {
			std::cerr << "Error: " << filepath << " is a directory" << std::endl;
			return ret;
		}

		if(fs::exists(path) && fs::is_regular_file(path)) {
			fs::file_status s = fs::status(path);
			if(!(s.permissions() & fs::owner_read)) {
				std::cerr << "Error: " << filepath << " is not readable" << std::endl;
				return ret;
			}
		}

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
				ret = true;
			} else {
				std::cerr
								<< "<Theme> should be the only root node in theme file: "
								<< filepath << std::endl;
			}
		} catch (std::exception& ex) {
			std::cerr << "Error: " << ex.what() << std::endl;
		}

		return ret;
	}

	bool Theme::Save (const std::string& filepath)
	{
		using namespace rapidxml;
		namespace fs = boost::filesystem;

		bool ret = false;

		boost::filesystem::path path(filepath);
		if(fs::is_directory(path)) {
			std::cerr << "Error: " << filepath << " is a directory" << std::endl;
			return ret;
		}

		if(fs::exists(path) && fs::is_regular_file(path)) {
			std::cerr << "Warning: " << filepath << " exists, overwrite it" << std::endl;

			fs::file_status s = fs::status(path);
			if(!(s.permissions() & fs::owner_write)) {
				std::cerr << "Error: " << filepath << " is not writable" << std::endl;
				return ret;
			}
		}

		char buf[16];

		xml_document<> doc;
		xml_node<>* root = doc.allocate_node(node_pi,
						doc.allocate_string(
										"xml version='1.0' encoding='utf-8'"));
		doc.append_node(root);

		xml_node<>* node = doc.allocate_node(node_element, "Theme", "BlendInt");
		doc.append_node(node);

		xml_node<>* ui_node = doc.allocate_node(node_element, "ThemeUserInterface");

		char* value = 0;

		snprintf(buf, 16, "%u", m_dpi);
		value = doc.allocate_string(buf);
		xml_attribute<>* attr = doc.allocate_attribute("dpi", value);
		ui_node->append_attribute(attr);

		snprintf(buf, 16, "%g", m_menu_shadow_fac);
		value = doc.allocate_string(buf);
		attr = doc.allocate_attribute("menu_shadow_fac", value);
		ui_node->append_attribute(attr);

		snprintf(buf, 16, "%hd", m_menu_shadow_width);
		value = doc.allocate_string(buf);
		attr = doc.allocate_attribute("menu_shadow_width", value);
		ui_node->append_attribute(attr);

		snprintf(buf, 16, "#%02X%02X%02X%02X", m_xaxis.r(), m_xaxis.g(), m_xaxis.b(), m_xaxis.a());
		value = doc.allocate_string(buf);
		attr = doc.allocate_attribute("axis_x", value);
		ui_node->append_attribute(attr);

		snprintf(buf, 16, "#%02X%02X%02X%02X", m_yaxis.r(), m_yaxis.g(), m_yaxis.b(), m_yaxis.a());
		value = doc.allocate_string(buf);
		attr = doc.allocate_attribute("axis_y", value);
		ui_node->append_attribute(attr);

		snprintf(buf, 16, "#%02X%02X%02X%02X", m_zaxis.r(), m_zaxis.g(), m_zaxis.b(), m_zaxis.a());
		value = doc.allocate_string(buf);
		attr = doc.allocate_attribute("axis_z", value);
		ui_node->append_attribute(attr);

		node->append_node(ui_node);

		xml_node<>* widget_color_node = AllocateWidgetThemeNode(doc, "wcol_regular", m_regular);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_tool", m_tool);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_text", m_text);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_radio_button", m_radio_button);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_option", m_option);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_toggle", m_toggle);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_number_field", m_number_field);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_number_slider", m_number_slider);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_menu", m_menu);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_pulldown", m_pulldown);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_menu_back", m_menu_back);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_menu_item", m_menu_item);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_tab", m_tab);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_tooltip", m_tooltip);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_box", m_box);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_scroll", m_scroll);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_progress", m_progress);
		ui_node->append_node(widget_color_node);

		widget_color_node = AllocateWidgetThemeNode(doc, "wcol_list_item", m_list_item);
		ui_node->append_node(widget_color_node);

		std::ofstream out(filepath.c_str());
		try {
			out << doc;
			ret = true;
		} catch (const std::exception& ex) {
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

		m_xaxis = 0xFF0000FF;
		m_yaxis = 0x00FF00FF;
		m_zaxis = 0x0000FFFF;
	}

	void Theme::ParseUINode (const rapidxml::xml_node<>* node)
	{
		unsigned int tmp = 0;

		for (rapidxml::xml_attribute<>* attrib =
				node->first_attribute(); attrib != NULL;
		        attrib = attrib->next_attribute())
		{
			if(strcmp("dpi", attrib->name()) == 0) {

				if(sscanf(attrib->value(), "%u", &tmp) == 1) {
					m_dpi = tmp;
				}

			} else if(strcmp("menu_shadow_fac", attrib->name()) == 0) {

				float v = 0.f;

				if(sscanf(attrib->value(), "%f", &v) == 1) {
					m_menu_shadow_fac = v;
				}

			} else if(strcmp("menu_shadow_width", attrib->name()) == 0) {

				short w = 0;

				if(sscanf(attrib->value(), "%hd", &w) == 1) {
					m_menu_shadow_width = w;
				}

			} else if(strcmp("axis_x", attrib->name()) == 0) {

				if(sscanf(attrib->value(), "#%x", &tmp) == 1) {
					m_xaxis = tmp;
				}

			} else if(strcmp("axis_y", attrib->name()) == 0) {

				if(sscanf(attrib->value(), "#%x", &tmp) == 1) {
					m_yaxis = tmp;
				}

			} else if(strcmp("axis_z", attrib->name()) == 0) {

				if(sscanf(attrib->value(), "#%x", &tmp) == 1) {
					m_zaxis = tmp;
				}

			}
		}

		for (rapidxml::xml_node<>* sub = node->first_node();
				sub != 0; sub = sub->next_sibling())
		{
			ParseWidgetColorNode(sub);
		}
	}

	void Theme::ParseWidgetColorNode (const rapidxml::xml_node<>* node)
	{
		WidgetTheme* p = 0;

		if (strcmp("wcol_regular", node->name()) == 0) {
			p = &m_regular;
		} else if (strcmp("wcol_tool", node->name()) == 0) {
			p = &m_tool;
		} else if (strcmp("wcol_text", node->name()) == 0) {
			p = &m_text;
		} else if (strcmp("wcol_radio_button", node->name()) == 0) {
			p = &m_radio_button;
		} else if (strcmp("wcol_option", node->name()) == 0) {
			p = &m_option;
		} else if (strcmp("wcol_toggle", node->name()) == 0) {
			p = &m_toggle;
		} else if (strcmp("wcol_number_field", node->name()) == 0) {
			p = &m_number_field;
		} else if (strcmp("wcol_number_slider", node->name()) == 0) {
			p = &m_number_slider;
		} else if (strcmp("wcol_menu", node->name()) == 0) {
			p = &m_menu;
		} else if (strcmp("wcol_pulldown", node->name()) == 0) {
			p = &m_pulldown;
		} else if (strcmp("wcol_menu_back", node->name()) == 0) {
			p = &m_menu_back;
		} else if (strcmp("wcol_menu_item", node->name()) == 0) {
			p = &m_menu_item;
		} else if (strcmp("wcol_tab", node->name()) == 0) {
			p = &m_tab;
		} else if (strcmp("wcol_tooltip", node->name()) == 0) {
			p = &m_tooltip;
		} else if (strcmp("wcol_box", node->name()) == 0) {
			p = &m_box;
		} else if (strcmp("wcol_scroll", node->name()) == 0) {
			p = &m_scroll;
		} else if (strcmp("wcol_progress", node->name()) == 0) {
			p = &m_progress;
		} else if (strcmp("wcol_list_item", node->name()) == 0) {
			p = &m_list_item;
		}

		if (p == 0)
			return;

		rapidxml::xml_node<>* widget_color_node = node->last_node(
		        "ThemeWidgetColors");
		unsigned int tmp = 0;

		if (widget_color_node) {
			for (rapidxml::xml_attribute<>* attrib =
			        widget_color_node->first_attribute(); attrib != NULL;
			        attrib = attrib->next_attribute()) {

				if (strcmp("outline", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->outline = tmp;
					}

				} else if (strcmp("inner", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->inner = tmp;
					}

				} else if (strcmp("inner_sel", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->inner_sel = tmp;
					}

				} else if (strcmp("item", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->item = tmp;
					}

				} else if (strcmp("text", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->text = tmp;
					}

				} else if (strcmp("text_sel", attrib->name()) == 0) {

					if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
						p->text_sel = tmp;
					}

				} else if (strcmp("show_shaded", attrib->name()) == 0) {

					char buf[8];
					strncpy(buf, attrib->value(), 8);
					for(int i = 0; i < 7; i++) {
						buf[i] = tolower(buf[i]);
					}
					buf[7] = '\0';

					if (strcmp(buf, "true") == 0) {
						p->shaded = true;
					} else {
						p->shaded = false;
					}

				} else if (strcmp("shadetop", attrib->name()) == 0) {

					short v;
					if (sscanf(attrib->value(), "%hd", &v) == 1) {
						p->shadetop = v;
					}

				} else if (strcmp("shadedown", attrib->name()) == 0) {

					short v;
					if (sscanf(attrib->value(), "%hd", &v) == 1) {
						p->shadedown = v;
					}

				}
			}
		}
	}
	
	rapidxml::xml_node<>* Theme::AllocateWidgetThemeNode (rapidxml::xml_document<>& doc,
					const char* name,
					const WidgetTheme& wtheme)
	{
		using namespace rapidxml;
		char buf[16];
		char* value = 0;

		xml_node<>* widget_color_node = doc.allocate_node(node_element, name);
		xml_node<>* colors_node = doc.allocate_node(node_element, "ThemeWidgetColors");

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.outline.r(),
						wtheme.outline.g(),
						wtheme.outline.b(),
						wtheme.outline.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("outline", value));

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.inner.r(),
						wtheme.inner.g(),
						wtheme.inner.b(),
						wtheme.inner.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("inner", value));

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.inner_sel.r(),
						wtheme.inner_sel.g(),
						wtheme.inner_sel.b(),
						wtheme.inner_sel.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("inner_sel", value));

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.item.r(),
						wtheme.item.g(),
						wtheme.item.b(),
						wtheme.item.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("item", value));

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.text.r(),
						wtheme.text.g(),
						wtheme.text.b(),
						wtheme.text.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("text", value));

		snprintf(buf, 16, "#%02X%02X%02X%02X",
						wtheme.text_sel.r(),
						wtheme.text_sel.g(),
						wtheme.text_sel.b(),
						wtheme.text_sel.a());
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("text_sel", value));

		snprintf(buf, 16, "%s", wtheme.shaded ? "TRUE" : "FALSE");
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("show_shaded", value));

		snprintf(buf, 16, "%hd", wtheme.shadetop);
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("shadetop", value));

		snprintf(buf, 16, "%hd", wtheme.shadedown);
		value = doc.allocate_string(buf);
		colors_node->append_attribute(doc.allocate_attribute("shadedown", value));

		widget_color_node->append_node(colors_node);

		return widget_color_node;
	}

} /* namespace BlendInt */
