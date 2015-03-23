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
#include <rapidxml_print.hpp>

#include <stock/theme.hpp>

namespace BlendInt {

  ColorScheme::ColorScheme ()
  : outline(0x191919FF),
    item(0x191919FF),
    inner(0x999999FF),
    inner_sel(0x646464FF),
    text(0x000000FF),
    text_sel(0xFFFFFFFF),
    shaded(false),
    shadetop(0),
    shadedown(0),
    alpha_check(0)
  {
  }

  Theme::Theme ()
  : dpi_(72),
    pixel_(1),
    shadow_fac_(0.5),
    shadow_width_(12)
  {
  }

  bool Theme::Load (const std::string& filepath)
  {
    using namespace rapidxml;
    namespace fs = boost::filesystem;

    bool ret = false;

    fs::path path(filepath);
    if (fs::is_directory(path)) {
      std::cerr << "Error: " << filepath << " is a directory" << std::endl;
      return ret;
    }

    if (fs::exists(path) && fs::is_regular_file(path)) {
      fs::file_status s = fs::status(path);
      if (!(s.permissions() & fs::owner_read)) {
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
        if (ui_node) {
          ParseUINode(ui_node);
        }
        ret = true;
      } else {
        std::cerr << "<Theme> should be the only root node in theme file: "
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
    if (fs::is_directory(path)) {
      std::cerr << "Error: " << filepath << " is a directory" << std::endl;
      return ret;
    }

    if (fs::exists(path) && fs::is_regular_file(path)) {
      std::cerr << "Warning: " << filepath << " exists, overwrite it"
          << std::endl;

      fs::file_status s = fs::status(path);
      if (!(s.permissions() & fs::owner_write)) {
        std::cerr << "Error: " << filepath << " is not writable" << std::endl;
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

    value = doc.allocate_string(default_font_);
    xml_attribute<>* attr = doc.allocate_attribute("default_font", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "%u", dpi_);
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("dpi", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "%hd", pixel_);
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("pixel", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "%g", shadow_fac_);
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("menu_shadow_fac", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "%hd", shadow_width_);
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("menu_shadow_width", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "#%02X%02X%02X%02X", xaxis_.uchar_red(),
             xaxis_.uchar_green(), xaxis_.uchar_blue(), xaxis_.uchar_alpha());
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("axis_x", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "#%02X%02X%02X%02X", yaxis_.uchar_red(),
             yaxis_.uchar_green(), yaxis_.uchar_blue(), yaxis_.uchar_alpha());
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("axis_y", value);
    ui_node->append_attribute(attr);

    snprintf(buf, 16, "#%02X%02X%02X%02X", zaxis_.uchar_red(),
             zaxis_.uchar_green(), zaxis_.uchar_blue(), zaxis_.uchar_alpha());
    value = doc.allocate_string(buf);
    attr = doc.allocate_attribute("axis_z", value);
    ui_node->append_attribute(attr);

    node->append_node(ui_node);

    xml_node<>* widget_color_node = AllocateThemeNode(doc,
                                                      "color_scheme_regular",
                                                      regular_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_tool", tool_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_text", text_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_push_button",
                                          push_button_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_radio_button",
                                          radio_button_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_option", option_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_toggle", toggle_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_number_field",
                                          number_field_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_number_slider",
                                          number_slider_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_menu", menu_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_pulldown",
                                          pulldown_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_menu_back",
                                          menu_back_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_menu_item",
                                          menu_item_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_tab", tab_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_tooltip",
                                          tooltip_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_box", box_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_scroll", scroll_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_progress",
                                          progress_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_list_item",
                                          list_item_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_dialog", dialog_);
    ui_node->append_node(widget_color_node);

    widget_color_node = AllocateThemeNode(doc, "color_scheme_node",
                                          node_);
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
  }

  void Theme::Reset ()
  {
    // Regular
    // _theme.color_scheme_regular	// use the default setting in struct constructor

    // Tool
    tool_.outline = 0x191919FF;
    tool_.item = 0x191919FF;
    tool_.inner = 0x999999FF;
    tool_.inner_sel = 0x646464FF;
    tool_.text = 0x000000FF;
    tool_.text_sel = 0xFFFFFFFF;
    tool_.shaded = true;
    tool_.shadetop = 15;
    tool_.shadedown = -15;

    // Radio Buttons
    radio_button_.outline = 0x000000FF;
    radio_button_.item = 0xFFFFFFFF;
    radio_button_.inner = 0x464646FF;
    radio_button_.inner_sel = 0x5680C2FF;
    radio_button_.text = 0xFFFFFFFF;
    radio_button_.text_sel = 0x000000FF;
    radio_button_.shaded = true;
    radio_button_.shadetop = 15;
    radio_button_.shadedown = -15;

    // Text
    text_.outline = 0x191919FF;
    text_.item = 0x5A5A5AFF;
    text_.inner = 0x999999FF;
    text_.inner_sel = 0x999999FF;
    text_.text = 0x000000FF;
    text_.text_sel = 0xFFFFFFFF;
    text_.shaded = true;
    text_.shadetop = 0;
    text_.shadedown = 25;

    // Option
    option_.outline = 0x000000FF;
    option_.item = 0xFFFFFFFF;
    option_.inner = 0x464646FF;
    option_.inner_sel = 0x464646FF;
    option_.text = 0x000000FF;
    option_.text_sel = 0xFFFFFFFF;
    option_.shaded = true;
    option_.shadetop = 15;
    option_.shadedown = -15;

    // PushButton, same as regular
    push_button_.outline = 0x191919FF;
    push_button_.item = 0x191919FF;
    push_button_.inner = 0x999999FF;
    push_button_.inner_sel = 0x646464FF;
    push_button_.text = 0x000000FF;
    push_button_.text_sel = 0xFFFFFFFF;
    push_button_.shaded = false;
    push_button_.shadetop = 0;
    push_button_.shadedown = 0;
    push_button_.alpha_check = 0;

    // Toggle
    toggle_.outline = 0x191919FF;
    toggle_.item = 0x191919FF;
    toggle_.inner = 0x999999FF;
    toggle_.inner_sel = 0x646464FF;
    toggle_.text = 0x000000FF;
    toggle_.text_sel = 0xFFFFFFFF;

    // Number Fields
    number_field_.outline = 0x191919FF;
    number_field_.item = 0xFAFAFAFF;
    number_field_.inner = 0xB4B4B4FF;
    number_field_.inner_sel = 0x999999FF;
    number_field_.text = 0x000000FF;
    number_field_.text_sel = 0xFFFFFFFF;
    number_field_.shaded = true;
    number_field_.shadetop = -20;
    number_field_.shadedown = 0;

    // Value Slider
    number_slider_.outline = 0x191919FF;
    number_slider_.item = 0x808080FF;
    number_slider_.inner = 0xB4B4B4FF;
    number_slider_.inner_sel = 0x999999FF;
    number_slider_.text = 0x000000FF;
    number_slider_.text_sel = 0xFFFFFFFF;
    number_slider_.shaded = true;
    number_slider_.shadetop = -20;
    number_slider_.shadedown = 0;

    // Box
    box_.outline = 0x191919FF;
    box_.item = 0x191919FF;
    box_.inner = 0x808080FF;
    box_.inner_sel = 0x646464FF;
    box_.text = 0x000000FF;
    box_.text_sel = 0xFFFFFFFF;

    // Menu
    menu_.outline = 0x000000FF;
    menu_.item = 0xFFFFFFFF;
    menu_.inner = 0x464646FF;
    menu_.inner_sel = 0x464646FF;
    menu_.text = 0xFFFFFFFF;
    menu_.text_sel = 0xCCCCCCFF;
    menu_.shaded = true;
    menu_.shadetop = 15;
    menu_.shadedown = -15;

    // Pulldown
    pulldown_.outline = 0x000000FF;
    pulldown_.item = 0xFFFFFFFF;
    pulldown_.inner = 0x3F3F3FFF;
    pulldown_.inner_sel = 0x5680C2FF;
    pulldown_.text = 0x000000FF;
    pulldown_.text_sel = 0x000000FF;

    // Menu Back
    menu_back_.outline = 0x000000FF;
    menu_back_.item = 0x646464FF;
    menu_back_.inner = 0x191919E6;
    menu_back_.inner_sel = 0x2D2D2DE6;
    menu_back_.text = 0xA0A0A0FF;
    menu_back_.text_sel = 0xFFFFFFFF;

    // Menu Item
    menu_item_.outline = 0x000000FF;
    menu_item_.item = 0xACACAC80;
    menu_item_.inner = 0x00000000;
    menu_item_.inner_sel = 0x5680C2FF;
    menu_item_.text = 0xFFFFFFFF;
    menu_item_.text_sel = 0x000000FF;
    menu_item_.shaded = true;
    menu_item_.shadetop = 38;
    menu_item_.shadedown = 0;

    // Tab, this influence TabButton, TabHeader
    tab_.outline = 0x000000FF;
    tab_.item = 0xFFFFFFFF;
    tab_.inner = 0x464646FF;
    tab_.inner_sel = 0x5680C2FF;
    tab_.text = 0xFFFFFFFF;
    tab_.text_sel = 0x000000FF;
    tab_.shaded = true;
    tab_.shadetop = 15;
    tab_.shadedown = -15;

    // Tooltip
    tooltip_.outline = 0x000000FF;
    tooltip_.item = 0x646464FF;
    tooltip_.inner = 0x191919E6;
    tooltip_.inner_sel = 0x2D2D2DE6;
    tooltip_.text = 0xA0A0A0FF;
    tooltip_.text_sel = 0xFFFFFFFF;

    // Scroll Bar
    scroll_.outline = 0x323232FF;
    scroll_.item = 0x808080FF;
    scroll_.inner = 0x505050B4;
    scroll_.inner_sel = 0x646464B4;
    scroll_.text = 0x000000FF;
    scroll_.text_sel = 0xFFFFFFFF;
    scroll_.shaded = true;
    scroll_.shadetop = 5 + 20;
    scroll_.shadedown = -5;

    // Progress Bar
    progress_.outline = 0x000000FF;
    progress_.item = 0x444444FF;
    progress_.inner = 0xBEBEBEFF;
    progress_.inner_sel = 0x646464B4;
    progress_.text = 0x000000FF;
    progress_.text_sel = 0xFFFFFFFF;

    // List Item
    list_item_.outline = 0x000000FF;
    list_item_.item = 0x000000FF;
    list_item_.inner = 0x00000000;
    list_item_.inner_sel = 0x5680C2FF;
    list_item_.text = 0x000000FF;
    list_item_.text_sel = 0x000000FF;

    // Dialog
    dialog_.outline = 0x000000FF;
    dialog_.item = 0x646464FF;
    dialog_.inner = 0x727272FF;
    dialog_.inner_sel = 0x2D2D2DE6;
    dialog_.text = 0xA0A0A0FF;
    dialog_.text_sel = 0xFFFFFFFF;

    // Decoration
    node_.outline = 0x000000FF;
    node_.item = 0x646464FF;
    node_.inner = 0x686868FF;
    node_.inner_sel = 0x2D2D2DE6;
    node_.text = 0xA0A0A0FF;
    node_.text_sel = 0xFFFFFFFF;
    node_.shaded = true;
    node_.shadetop = 15;
    node_.shadedown = 0;

#ifdef DEBUG
    strncpy(default_font_, "Source Han Sans CN", 256);
#else
    strncpy (default_font_, "Droid Sans", 256);
#endif

    dpi_ = 72;

    //_theme.panel.header = RGBAf();
    //_theme.panel.back = RGBAf();
    shadow_fac_ = 0.5f;
    shadow_width_ = 9;

    // TODO: check if retina in Mac OS
    pixel_ = 1;

    xaxis_ = 0xFF0000FF;
    yaxis_ = 0x00FF00FF;
    zaxis_ = 0x0000FFFF;
  }

  void Theme::ParseUINode (const rapidxml::xml_node<>* node)
  {
    unsigned int tmp = 0;

    for (rapidxml::xml_attribute<>* attrib = node->first_attribute();
        attrib != NULL; attrib = attrib->next_attribute()) {
      if (strcmp("default_font", attrib->name()) == 0) {

        strncpy(default_font_, attrib->value(), 256);

      } else if (strcmp("dpi", attrib->name()) == 0) {

        if (sscanf(attrib->value(), "%u", &tmp) == 1) {
          dpi_ = tmp;
        }

      } else if (strcmp("pixel", attrib->name()) == 0) {

        short v = 0;
        if (sscanf(attrib->value(), "%hd", &v) == 1) {
          pixel_ = v;
        }

      } else if (strcmp("menu_shadow_fac", attrib->name()) == 0) {

        float v = 0.f;
        if (sscanf(attrib->value(), "%f", &v) == 1) {
          shadow_fac_ = v;
        }

      } else if (strcmp("menu_shadow_width", attrib->name()) == 0) {

        short w = 0;

        if (sscanf(attrib->value(), "%hd", &w) == 1) {
          shadow_width_ = w;
        }

      } else if (strcmp("axis_x", attrib->name()) == 0) {

        if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
          xaxis_ = tmp;
        }

      } else if (strcmp("axis_y", attrib->name()) == 0) {

        if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
          yaxis_ = tmp;
        }

      } else if (strcmp("axis_z", attrib->name()) == 0) {

        if (sscanf(attrib->value(), "#%x", &tmp) == 1) {
          zaxis_ = tmp;
        }

      }
    }

    for (rapidxml::xml_node<>* sub = node->first_node(); sub != 0;
        sub = sub->next_sibling()) {
      ParseColorSchemeNode(sub);
    }
  }

  void Theme::ParseColorSchemeNode (const rapidxml::xml_node<>* node)
  {
    ColorScheme* p = 0;

    if (strcmp("color_scheme_regular", node->name()) == 0) {
      p = &regular_;
    } else if (strcmp("color_scheme_tool", node->name()) == 0) {
      p = &tool_;
    } else if (strcmp("color_scheme_text", node->name()) == 0) {
      p = &text_;
    } else if (strcmp("color_scheme_push_button", node->name()) == 0) {
      p = &push_button_;
    } else if (strcmp("color_scheme_radio_button", node->name()) == 0) {
      p = &radio_button_;
    } else if (strcmp("color_scheme_option", node->name()) == 0) {
      p = &option_;
    } else if (strcmp("color_scheme_toggle", node->name()) == 0) {
      p = &toggle_;
    } else if (strcmp("color_scheme_number_field", node->name()) == 0) {
      p = &number_field_;
    } else if (strcmp("color_scheme_number_slider", node->name()) == 0) {
      p = &number_slider_;
    } else if (strcmp("color_scheme_menu", node->name()) == 0) {
      p = &menu_;
    } else if (strcmp("color_scheme_pulldown", node->name()) == 0) {
      p = &pulldown_;
    } else if (strcmp("color_scheme_menu_back", node->name()) == 0) {
      p = &menu_back_;
    } else if (strcmp("color_scheme_menu_item", node->name()) == 0) {
      p = &menu_item_;
    } else if (strcmp("color_scheme_tab", node->name()) == 0) {
      p = &tab_;
    } else if (strcmp("color_scheme_tooltip", node->name()) == 0) {
      p = &tooltip_;
    } else if (strcmp("color_scheme_box", node->name()) == 0) {
      p = &box_;
    } else if (strcmp("color_scheme_scroll", node->name()) == 0) {
      p = &scroll_;
    } else if (strcmp("color_scheme_progress", node->name()) == 0) {
      p = &progress_;
    } else if (strcmp("color_scheme_list_item", node->name()) == 0) {
      p = &list_item_;
    } else if (strcmp("color_scheme_dialog", node->name()) == 0) {
      p = &dialog_;
    } else if (strcmp("color_scheme_node", node->name()) == 0) {
      p = &node_;
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

  rapidxml::xml_node<>* Theme::AllocateThemeNode (rapidxml::xml_document<>& doc,
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
