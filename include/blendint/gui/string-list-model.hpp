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

#pragma once

#include <blendint/gui/abstract-list-model.hpp>

namespace BlendInt {

class StringListModel: public AbstractListModel
{
public:

  StringListModel ();

  virtual ~StringListModel ();

  void AddString (const String& string);

  void InsertString (int row, const String& string);

  virtual int GetRowCount (const ModelIndex& parent = ModelIndex()) const;

  virtual int GetColumnCount (const ModelIndex& parent = ModelIndex()) const;

  virtual int GetPreferredColumnWidth (int index, const ModelIndex& parent =
                                           ModelIndex()) const override;

  virtual int GetPreferredRowHeight (int index, const ModelIndex& parent =
                                         ModelIndex()) const override;

#ifdef DEBUG

  void Print ();

#endif

protected:

  virtual bool InsertColumns (int column, int count, const ModelIndex& parent =
                                  ModelIndex());

  virtual bool RemoveColumns (int column, int count, const ModelIndex& parent =
                                  ModelIndex());

  virtual bool InsertRows (int row, int count, const ModelIndex& parent =
                               ModelIndex());

  virtual bool RemoveRows (int row, int count, const ModelIndex& parent =
                               ModelIndex());

private:

  int rows_;

};

}
