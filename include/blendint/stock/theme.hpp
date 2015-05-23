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

#pragma once

#include <blendint/core/color.hpp>

namespace BlendInt {

struct ColorScheme
{
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

struct ThemeData
{
  /* Interface Elements (buttons, menus, icons) */
  ColorScheme regular;
  ColorScheme tool;
  ColorScheme text;
  ColorScheme push_button;
  ColorScheme radio_button;
  ColorScheme option;
  ColorScheme toggle;
  ColorScheme number_field;
  ColorScheme number_slider;
  ColorScheme menu;
  ColorScheme pulldown;
  ColorScheme menu_back;
  ColorScheme menu_item;
  ColorScheme tab;
  ColorScheme tooltip;
  ColorScheme box;
  ColorScheme scroll;
  ColorScheme progress;
  ColorScheme list_item;
  ColorScheme dialog;
  ColorScheme node;

  /** Default font */
  char default_font[256];

  /** Font DPI */
  unsigned int dpi;

  /** How many pixels for 1 border */
  float pixel;

  /* fac: 0 - 1 for blend factor, width in pixels */
  // NOT USED
  float shadow_fac;

  short shadow_width;

  //short pad;

  //char iconfile[256]; // FILE_MAXFILE length
  //float icon_alpha;

  /* Axis Colors */
  Color xaxis, yaxis, zaxis;
};

class Theme
{
public:

  static Theme* kTheme;

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

  inline const ColorScheme& box () const
  {
    return data_.box;
  }

  inline const ColorScheme& list_item () const
  {
    return data_.list_item;
  }

  inline const ColorScheme& menu () const
  {
    return data_.menu;
  }

  inline const ColorScheme& menu_back () const
  {
    return data_.menu_back;
  }

  inline const ColorScheme& menu_item () const
  {
    return data_.menu_item;
  }

  inline const ColorScheme& number_field () const
  {
    return data_.number_field;
  }

  inline const ColorScheme& number_slider () const
  {
    return data_.number_slider;
  }

  inline const ColorScheme& option () const
  {
    return data_.option;
  }

  inline const ColorScheme& progress () const
  {
    return data_.progress;
  }

  inline const ColorScheme& pulldown () const
  {
    return data_.pulldown;
  }

  inline const ColorScheme& push_button () const
  {
    return data_.push_button;
  }

  inline const ColorScheme& radio_button () const
  {
    return data_.radio_button;
  }

  inline const ColorScheme& regular () const
  {
    return data_.regular;
  }

  inline const ColorScheme& scroll () const
  {
    return data_.scroll;
  }

  inline const ColorScheme& tab () const
  {
    return data_.tab;
  }

  inline const ColorScheme& text () const
  {
    return data_.text;
  }

  inline const ColorScheme& toggle () const
  {
    return data_.toggle;
  }

  inline const ColorScheme& tool () const
  {
    return data_.tool;
  }

  inline const ColorScheme& tooltip () const
  {
    return data_.tooltip;
  }

  inline const ColorScheme& dialog () const
  {
    return data_.dialog;
  }

  inline const ColorScheme& node () const
  {
    return data_.node;
  }

  inline const char* default_font () const
  {
    return data_.default_font;
  }

  inline unsigned int dpi () const
  {
    return data_.dpi;
  }

  inline float pixel () const
  {
    return data_.pixel;
  }

  inline float menu_shadow_fac1 () const
  {
    return data_.shadow_fac;
  }

  inline short menu_shadow_width1 () const
  {
    return data_.shadow_width;
  }

  inline float shadow_fac () const
  {
    return data_.shadow_fac;
  }

  inline short shadow_width () const
  {
    return data_.shadow_width;
  }

private:

  friend class AbstractWindow;

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

  ThemeData data_;
};

inline Theme* const theme ()
{
  return Theme::kTheme;
}

inline float pixel_size (float a)
{
  return a * Theme::kTheme->pixel();
}

inline float pixel_size (int a)
{
  return a * Theme::kTheme->pixel();
}

inline float pixel_size (unsigned int a)
{
  return a * Theme::kTheme->pixel();
}

} /* namespace BlendInt */
