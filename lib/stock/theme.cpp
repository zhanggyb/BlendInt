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

#include <string.h>
#include <stdlib.h>

#include <boost/filesystem.hpp>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

#include <blendint/stock/theme.hpp>

namespace BlendInt {

using namespace rapidxml;

static void ParseUINode (const rapidxml::xml_node<>* node, ThemeData* data);

static void ParseColorSchemeNode (const rapidxml::xml_node<>* node, ThemeData* data);

static xml_node<>* AllocateThemeNode (xml_document<>& doc,
                                      const char* name,
                                      const ColorScheme& scheme);

Theme::Theme ()
{
  data_.dpi = 72;
  data_.pixel = 1;
  data_.shadow_fac = 0.5;
  data_.shadow_width = 12;
}

bool Theme::Load (const std::string& filepath)
{
  using namespace rapidxml;
  namespace fs = boost::filesystem;

  bool ret = false;

  fs::path path(filepath);
  if (fs::is_directory(path)) {
    DBG_PRINT_MSG("ERROR: %s is a directory", filepath.c_str());
    return ret;
  }

  if (fs::exists(path) && fs::is_regular_file(path)) {
    fs::file_status s = fs::status(path);
    if (!(s.permissions() & fs::owner_read)) {
      DBG_PRINT_MSG("ERROR: %s is not readable", filepath.c_str());
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
      if (ui_node) {
        ParseUINode(ui_node, &data_);
      }
      ret = true;
    } else {
      DBG_PRINT_MSG(
          "ERROR: <Theme> should be the only root node in theme file: %s",
          filepath.c_str());
    }
  } catch (std::exception& ex) {
    DBG_PRINT_MSG("ERROR: %s", ex.what());
  }

  return ret;
}

bool Theme::Save (const std::string& filepath)
{
  using namespace rapidxml;
  namespace fs = boost::filesystem;

  bool ret = false;

  boost::filesystem::path path(filepath);
  if (fs::is_directory(path)) {
    DBG_PRINT_MSG("ERROR: %s is a directory", filepath.c_str());
    return ret;
  }

  if (fs::exists(path) && fs::is_regular_file(path)) {
    fs::file_status s = fs::status(path);
    if (!(s.permissions() & fs::owner_write)) {
      DBG_PRINT_MSG("ERROR: %s is not writable", filepath.c_str());
      return ret;
    }
  }

  char buf[16];

  xml_document<> doc;
  xml_node<>* root = doc.allocate_node(
      node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
  doc.append_node(root);

  xml_node<>* node = doc.allocate_node(node_element, "Theme", "BlendInt");
  doc.append_node(node);

  xml_node<>* ui_node = doc.allocate_node(node_element, "ThemeUserInterface");

  char* value = 0;

  value = doc.allocate_string(data_.default_font);
  xml_attribute<>* attr = doc.allocate_attribute("default_font", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "%u", data_.dpi);
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("dpi", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "%hd", data_.pixel);
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("pixel", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "%g", data_.shadow_fac);
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("menu_shadow_fac", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "%hd", data_.shadow_width);
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("menu_shadow_width", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "#%02X%02X%02X%02X",
           data_.xaxis.uchar_red(),
           data_.xaxis.uchar_green(),
           data_.xaxis.uchar_blue(),
           data_.xaxis.uchar_alpha());
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("axis_x", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "#%02X%02X%02X%02X",
           data_.yaxis.uchar_red(),
           data_.yaxis.uchar_green(),
           data_.yaxis.uchar_blue(),
           data_.yaxis.uchar_alpha());
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("axis_y", value);
  ui_node->append_attribute(attr);

  snprintf(buf, 16, "#%02X%02X%02X%02X",
           data_.zaxis.uchar_red(),
           data_.zaxis.uchar_green(),
           data_.zaxis.uchar_blue(),
           data_.zaxis.uchar_alpha());
  value = doc.allocate_string(buf);
  attr = doc.allocate_attribute("axis_z", value);
  ui_node->append_attribute(attr);

  node->append_node(ui_node);

  xml_node<>* widget_color_node = AllocateThemeNode(doc, "color_scheme_regular",
                                                    data_.regular);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_tool", data_.tool);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_text", data_.text);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_push_button",
                                        data_.push_button);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_radio_button",
                                        data_.radio_button);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_option", data_.option);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_toggle", data_.toggle);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_number_field",
                                        data_.number_field);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_number_slider",
                                        data_.number_slider);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_menu", data_.menu);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_pulldown",
                                        data_.pulldown);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_menu_back",
                                        data_.menu_back);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_menu_item",
                                        data_.menu_item);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_tab", data_.tab);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_tooltip", data_.tooltip);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_box", data_.box);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_scroll", data_.scroll);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_progress",
                                        data_.progress);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_list_item",
                                        data_.list_item);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_dialog", data_.dialog);
  ui_node->append_node(widget_color_node);

  widget_color_node = AllocateThemeNode(doc, "color_scheme_node", data_.node);
  ui_node->append_node(widget_color_node);

  std::ofstream out(filepath.c_str());
  try {
    out << doc;
    ret = true;
  } catch (const std::exception& ex) {
    DBG_PRINT_MSG("ERROR: %s", ex.what());
  }

  return ret;
}

Theme::~Theme ()
{
}

void Theme::Reset ()
{
  // Regular
  data_.regular.outline = 0x191919FF;
  data_.regular.item = 0x191919FF;
  data_.regular.inner = 0x999999FF;
  data_.regular.inner_sel = 0x646464FF;
  data_.regular.text = 0x000000FF;
  data_.regular.text_sel = 0xFFFFFFFF;
  data_.regular.shaded = false;
  data_.regular.shadetop = 0;
  data_.regular.shadedown = 0;
  data_.regular.alpha_check = 0;

  // Tool
  data_.tool.outline = 0x191919FF;
  data_.tool.item = 0x191919FF;
  data_.tool.inner = 0x999999FF;
  data_.tool.inner_sel = 0x646464FF;
  data_.tool.text = 0x000000FF;
  data_.tool.text_sel = 0xFFFFFFFF;
  data_.tool.shaded = true;
  data_.tool.shadetop = 15;
  data_.tool.shadedown = -15;
  data_.tool.alpha_check = 0;

  // Radio Buttons
  data_.radio_button.outline = 0x000000FF;
  data_.radio_button.item = 0xFFFFFFFF;
  data_.radio_button.inner = 0x464646FF;
  data_.radio_button.inner_sel = 0x5680C2FF;
  data_.radio_button.text = 0xFFFFFFFF;
  data_.radio_button.text_sel = 0x000000FF;
  data_.radio_button.shaded = true;
  data_.radio_button.shadetop = 15;
  data_.radio_button.shadedown = -15;
  data_.radio_button.alpha_check = 0;

  // Text
  data_.text.outline = 0x191919FF;
  data_.text.item = 0x5A5A5AFF;
  data_.text.inner = 0x999999FF;
  data_.text.inner_sel = 0x999999FF;
  data_.text.text = 0x000000FF;
  data_.text.text_sel = 0xFFFFFFFF;
  data_.text.shaded = true;
  data_.text.shadetop = 0;
  data_.text.shadedown = 25;
  data_.text.alpha_check = 0;

  // Option
  data_.option.outline = 0x000000FF;
  data_.option.item = 0xFFFFFFFF;
  data_.option.inner = 0x464646FF;
  data_.option.inner_sel = 0x464646FF;
  data_.option.text = 0x000000FF;
  data_.option.text_sel = 0xFFFFFFFF;
  data_.option.shaded = true;
  data_.option.shadetop = 15;
  data_.option.shadedown = -15;
  data_.option.alpha_check = 0;

  // PushButton, same as regular
  data_.push_button.outline = 0x191919FF;
  data_.push_button.item = 0x191919FF;
  data_.push_button.inner = 0x999999FF;
  data_.push_button.inner_sel = 0x646464FF;
  data_.push_button.text = 0x000000FF;
  data_.push_button.text_sel = 0xFFFFFFFF;
  data_.push_button.shaded = false;
  data_.push_button.shadetop = 0;
  data_.push_button.shadedown = 0;
  data_.push_button.alpha_check = 0;

  // Toggle
  data_.toggle.outline = 0x191919FF;
  data_.toggle.item = 0x191919FF;
  data_.toggle.inner = 0x999999FF;
  data_.toggle.inner_sel = 0x646464FF;
  data_.toggle.text = 0x000000FF;
  data_.toggle.text_sel = 0xFFFFFFFF;
  data_.toggle.shaded = false;
  data_.toggle.shadetop = 0;
  data_.toggle.shadedown = 0;
  data_.toggle.alpha_check = 0;

  // Number Fields
  data_.number_field.outline = 0x191919FF;
  data_.number_field.item = 0xFAFAFAFF;
  data_.number_field.inner = 0xB4B4B4FF;
  data_.number_field.inner_sel = 0x999999FF;
  data_.number_field.text = 0x000000FF;
  data_.number_field.text_sel = 0xFFFFFFFF;
  data_.number_field.shaded = true;
  data_.number_field.shadetop = -20;
  data_.number_field.shadedown = 0;
  data_.number_field.alpha_check = 0;

  // Value Slider
  data_.number_slider.outline = 0x191919FF;
  data_.number_slider.item = 0x808080FF;
  data_.number_slider.inner = 0xB4B4B4FF;
  data_.number_slider.inner_sel = 0x999999FF;
  data_.number_slider.text = 0x000000FF;
  data_.number_slider.text_sel = 0xFFFFFFFF;
  data_.number_slider.shaded = true;
  data_.number_slider.shadetop = -20;
  data_.number_slider.shadedown = 0;
  data_.number_slider.alpha_check = 0;

  // Box
  data_.box.outline = 0x191919FF;
  data_.box.item = 0x191919FF;
  data_.box.inner = 0x808080FF;
  data_.box.inner_sel = 0x646464FF;
  data_.box.text = 0x000000FF;
  data_.box.text_sel = 0xFFFFFFFF;
  data_.box.shaded = false;
  data_.box.shadetop = 0;
  data_.box.shadedown = 0;
  data_.box.alpha_check = 0;

  // Menu
  data_.menu.outline = 0x000000FF;
  data_.menu.item = 0xFFFFFFFF;
  data_.menu.inner = 0x464646FF;
  data_.menu.inner_sel = 0x464646FF;
  data_.menu.text = 0xFFFFFFFF;
  data_.menu.text_sel = 0xCCCCCCFF;
  data_.menu.shaded = true;
  data_.menu.shadetop = 15;
  data_.menu.shadedown = -15;
  data_.menu.alpha_check = 0;

  // Pulldown
  data_.pulldown.outline = 0x000000FF;
  data_.pulldown.item = 0xFFFFFFFF;
  data_.pulldown.inner = 0x3F3F3FFF;
  data_.pulldown.inner_sel = 0x5680C2FF;
  data_.pulldown.text = 0x000000FF;
  data_.pulldown.text_sel = 0x000000FF;
  data_.pulldown.shaded = false;
  data_.pulldown.shadetop = 0;
  data_.pulldown.shadedown = 0;
  data_.pulldown.alpha_check = 0;

  // Menu Back
  data_.menu_back.outline = 0x000000FF;
  data_.menu_back.item = 0x646464FF;
  data_.menu_back.inner = 0x191919E6;
  data_.menu_back.inner_sel = 0x2D2D2DE6;
  data_.menu_back.text = 0xA0A0A0FF;
  data_.menu_back.text_sel = 0xFFFFFFFF;
  data_.menu_back.shaded = false;
  data_.menu_back.shadetop = 0;
  data_.menu_back.shadedown = 0;
  data_.menu_back.alpha_check = 0;

  // Menu Item
  data_.menu_item.outline = 0x000000FF;
  data_.menu_item.item = 0xACACAC80;
  data_.menu_item.inner = 0x00000000;
  data_.menu_item.inner_sel = 0x5680C2FF;
  data_.menu_item.text = 0xFFFFFFFF;
  data_.menu_item.text_sel = 0x000000FF;
  data_.menu_item.shaded = true;
  data_.menu_item.shadetop = 38;
  data_.menu_item.shadedown = 0;
  data_.menu_item.alpha_check = 0;

  // Tab, this influence TabButton, TabHeader
  data_.tab.outline = 0x000000FF;
  data_.tab.item = 0xFFFFFFFF;
  data_.tab.inner = 0x464646FF;
  data_.tab.inner_sel = 0x5680C2FF;
  data_.tab.text = 0xFFFFFFFF;
  data_.tab.text_sel = 0x000000FF;
  data_.tab.shaded = true;
  data_.tab.shadetop = 15;
  data_.tab.shadedown = -15;
  data_.tab.alpha_check = 0;

  // Tooltip
  data_.tooltip.outline = 0x000000FF;
  data_.tooltip.item = 0x646464FF;
  data_.tooltip.inner = 0x191919E6;
  data_.tooltip.inner_sel = 0x2D2D2DE6;
  data_.tooltip.text = 0xA0A0A0FF;
  data_.tooltip.text_sel = 0xFFFFFFFF;
  data_.tooltip.shaded = false;
  data_.tooltip.shadetop = 0;
  data_.tooltip.shadedown = 0;
  data_.tooltip.alpha_check = 0;

  // Scroll Bar
  data_.scroll.outline = 0x323232FF;
  data_.scroll.item = 0x808080FF;
  data_.scroll.inner = 0x505050B4;
  data_.scroll.inner_sel = 0x646464B4;
  data_.scroll.text = 0x000000FF;
  data_.scroll.text_sel = 0xFFFFFFFF;
  data_.scroll.shaded = true;
  data_.scroll.shadetop = 5 + 20;
  data_.scroll.shadedown = -5;
  data_.scroll.alpha_check = 0;

  // Progress Bar
  data_.progress.outline = 0x000000FF;
  data_.progress.item = 0x444444FF;
  data_.progress.inner = 0xBEBEBEFF;
  data_.progress.inner_sel = 0x646464B4;
  data_.progress.text = 0x000000FF;
  data_.progress.text_sel = 0xFFFFFFFF;
  data_.progress.shaded = false;
  data_.progress.shadetop = 0;
  data_.progress.shadedown = 0;
  data_.progress.alpha_check = 0;

  // List Item
  data_.list_item.outline = 0x000000FF;
  data_.list_item.item = 0x000000FF;
  data_.list_item.inner = 0x00000000;
  data_.list_item.inner_sel = 0x5680C2FF;
  data_.list_item.text = 0x000000FF;
  data_.list_item.text_sel = 0x000000FF;
  data_.tooltip.shaded = false;
  data_.tooltip.shadetop = 0;
  data_.tooltip.shadedown = 0;
  data_.tooltip.alpha_check = 0;

  // Dialog
  data_.dialog.outline = 0x000000FF;
  data_.dialog.item = 0x646464FF;
  data_.dialog.inner = 0x727272FF;
  data_.dialog.inner_sel = 0x2D2D2DE6;
  data_.dialog.text = 0xA0A0A0FF;
  data_.dialog.text_sel = 0xFFFFFFFF;
  data_.dialog.shaded = false;
  data_.dialog.shadetop = 0;
  data_.dialog.shadedown = 0;
  data_.dialog.alpha_check = 0;

  // Decoration
  data_.node.outline = 0x000000FF;
  data_.node.item = 0x646464FF;
  data_.node.inner = 0x686868FF;
  data_.node.inner_sel = 0x2D2D2DE6;
  data_.node.text = 0xA0A0A0FF;
  data_.node.text_sel = 0xFFFFFFFF;
  data_.node.shaded = true;
  data_.node.shadetop = 15;
  data_.node.shadedown = 0;
  data_.node.alpha_check = 0;

#ifdef DEBUG
  strncpy(data_.default_font, "Source Han Sans CN", 256);
#else
  strncpy (data_.default_font, "Droid Sans", 256);
#endif

  data_.dpi = 72;

  //_theme.panel.header = RGBAf();
  //_theme.panel.back = RGBAf();
  data_.shadow_fac = 0.5f;
  data_.shadow_width = 9;

  // TODO: check if retina in Mac OS
  data_.pixel = 1;

  data_.xaxis = 0xFF0000FF;
  data_.yaxis = 0x00FF00FF;
  data_.zaxis = 0x0000FFFF;
}

void ParseUINode (const rapidxml::xml_node<>* node, ThemeData* data)
{
  unsigned int tmp = 0;

  for (rapidxml::xml_attribute<>* attrib = node->first_attribute();
      attrib != NULL; attrib = attrib->next_attribute()) {
    if (strcmp("default_font", attrib->name()) == 0) {

      strncpy(data->default_font, attrib->value(), 256);

    } else if (strcmp("dpi", attrib->name()) == 0) {

      if (sscanf(attrib->value(), "%u", &tmp) == 1) {
        data->dpi = tmp;
      }

    } else if (strcmp("pixel", attrib->name()) == 0) {

      short v = 0;
      if (sscanf(attrib->value(), "%hd", &v) == 1) {
        data->pixel = v;
      }

    } else if (strcmp("menu_shadow_fac", attrib->name()) == 0) {

      float v = 0.f;
      if (sscanf(attrib->value(), "%f", &v) == 1) {
        data->shadow_fac = v;
      }

    } else if (strcmp("menu_shadow_width", attrib->name()) == 0) {

      short w = 0;

      if (sscanf(attrib->value(), "%hd", &w) == 1) {
        data->shadow_width = w;
      }

    } else if (strcmp("axis_x", attrib->name()) == 0) {

      if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
        data->xaxis = tmp;
      }

    } else if (strcmp("axis_y", attrib->name()) == 0) {

      if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
        data->yaxis = tmp;
      }

    } else if (strcmp("axis_z", attrib->name()) == 0) {

      if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
        data->zaxis = tmp;
      }

    }
  }

  for (rapidxml::xml_node<>* sub = node->first_node(); sub != 0;
      sub = sub->next_sibling()) {
    ParseColorSchemeNode(sub, data);
  }
}

void ParseColorSchemeNode (const rapidxml::xml_node<>* node, ThemeData* data)
{
  ColorScheme* p = 0;

  if (strcmp("color_scheme_regular", node->name()) == 0) {
    p = &data->regular;
  } else if (strcmp("color_scheme_tool", node->name()) == 0) {
    p = &data->tool;
  } else if (strcmp("color_scheme_text", node->name()) == 0) {
    p = &data->text;
  } else if (strcmp("color_scheme_push_button", node->name()) == 0) {
    p = &data->push_button;
  } else if (strcmp("color_scheme_radio_button", node->name()) == 0) {
    p = &data->radio_button;
  } else if (strcmp("color_scheme_option", node->name()) == 0) {
    p = &data->option;
  } else if (strcmp("color_scheme_toggle", node->name()) == 0) {
    p = &data->toggle;
  } else if (strcmp("color_scheme_number_field", node->name()) == 0) {
    p = &data->number_field;
  } else if (strcmp("color_scheme_number_slider", node->name()) == 0) {
    p = &data->number_slider;
  } else if (strcmp("color_scheme_menu", node->name()) == 0) {
    p = &data->menu;
  } else if (strcmp("color_scheme_pulldown", node->name()) == 0) {
    p = &data->pulldown;
  } else if (strcmp("color_scheme_menu_back", node->name()) == 0) {
    p = &data->menu_back;
  } else if (strcmp("color_scheme_menu_item", node->name()) == 0) {
    p = &data->menu_item;
  } else if (strcmp("color_scheme_tab", node->name()) == 0) {
    p = &data->tab;
  } else if (strcmp("color_scheme_tooltip", node->name()) == 0) {
    p = &data->tooltip;
  } else if (strcmp("color_scheme_box", node->name()) == 0) {
    p = &data->box;
  } else if (strcmp("color_scheme_scroll", node->name()) == 0) {
    p = &data->scroll;
  } else if (strcmp("color_scheme_progress", node->name()) == 0) {
    p = &data->progress;
  } else if (strcmp("color_scheme_list_item", node->name()) == 0) {
    p = &data->list_item;
  } else if (strcmp("color_scheme_dialog", node->name()) == 0) {
    p = &data->dialog;
  } else if (strcmp("color_scheme_node", node->name()) == 0) {
    p = &data->node;
  }

  if (p == 0) return;

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
        for (int i = 0; i < 7; i++) {
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

xml_node<>* AllocateThemeNode (xml_document<>& doc,
                               const char* name,
                               const ColorScheme& scheme)
{
  using namespace rapidxml;
  char buf[16];
  char* value = 0;

  xml_node<>* widget_color_node = doc.allocate_node(node_element, name);
  xml_node<>* colors_node = doc.allocate_node(node_element,
                                              "ThemeWidgetColors");

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.outline.uchar_red(),
           scheme.outline.uchar_green(), scheme.outline.uchar_blue(),
           scheme.outline.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("outline", value));

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.inner.uchar_red(),
           scheme.inner.uchar_green(), scheme.inner.uchar_blue(),
           scheme.inner.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("inner", value));

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.inner_sel.uchar_red(),
           scheme.inner_sel.uchar_green(), scheme.inner_sel.uchar_blue(),
           scheme.inner_sel.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("inner_sel", value));

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.item.uchar_red(),
           scheme.item.uchar_green(), scheme.item.uchar_blue(),
           scheme.item.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("item", value));

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.text.uchar_red(),
           scheme.text.uchar_green(), scheme.text.uchar_blue(),
           scheme.text.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("text", value));

  snprintf(buf, 16, "#%02X%02X%02X%02X", scheme.text_sel.uchar_red(),
           scheme.text_sel.uchar_green(), scheme.text_sel.uchar_blue(),
           scheme.text_sel.uchar_alpha());
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("text_sel", value));

  snprintf(buf, 16, "%s", scheme.shaded ? "TRUE" : "FALSE");
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("show_shaded", value));

  snprintf(buf, 16, "%hd", scheme.shadetop);
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("shadetop", value));

  snprintf(buf, 16, "%hd", scheme.shadedown);
  value = doc.allocate_string(buf);
  colors_node->append_attribute(doc.allocate_attribute("shadedown", value));

  widget_color_node->append_node(colors_node);

  return widget_color_node;
}

} /* namespace BlendInt */
