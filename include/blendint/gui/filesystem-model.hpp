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

#include <blendint/gui/abstract-item-model.hpp>

namespace BlendInt {

/**
 * @brief A data model based on boost::filesystem for the local file system
 *
 * This data model contains 6 columns by default:
 * <Name> - <Type> - <Owner> - <Group> - <Permissions> - <Last write time>
 *
 * The default constructor does nothing, use Load() to load and store a file
 * list in a path.
 */
class FileSystemModel: public AbstractItemModel
{
public:

  FileSystemModel ();

  virtual ~FileSystemModel ();

  /**
   * @brief List files in a path
   * @param pathname The path name
   * @return
   * 	- true success
   * 	- false failure
   */
  bool Load (const std::string& pathname);

  void Clear ();

  virtual int GetRowCount (const ModelIndex& superview = ModelIndex()) const
      override;

  virtual int GetPreferredColumnWidth (int index, const ModelIndex& parent =
                                           ModelIndex()) const override;

  virtual int GetColumnCount (const ModelIndex& superview = ModelIndex()) const;

  virtual int GetPreferredRowHeight (int index, const ModelIndex& parent =
                                         ModelIndex()) const override;

  virtual bool InsertColumns (int column,
                              int count,
                              const ModelIndex& superview = ModelIndex());

  virtual bool RemoveColumns (int column,
                              int count,
                              const ModelIndex& superview = ModelIndex());

  virtual bool InsertRows (int row, int count, const ModelIndex& superview =
                               ModelIndex());

  virtual bool RemoveRows (int row, int count, const ModelIndex& superview =
                               ModelIndex());

  virtual ModelIndex GetRootIndex () const;

  virtual ModelIndex GetIndex (int row,
                               int column,
                               const ModelIndex& superview =
                                   ModelIndex()) const;

#ifdef DEBUG

  void Print ();

  void PrintRow (ModelNode* first);

#endif	// DEBUG

protected:

  const ModelNode* root () const
  {
    return root_;
  }

private:

  void InsertColumns (int column, int count, ModelNode* left);

  void DestroyColumnsInRow (int column, int count, ModelNode* node);

  static void DestroyRow (ModelNode* node);

  static void DestroyColumn (ModelNode* node);

  int rows_;

  int columns_;

  ModelNode* root_;

  static const int DefaultColumns = 5;

};

}
