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

#include <gui/abstract-item-model.hpp>

namespace BlendInt {

  /**
   * @brief Abstract class for list model
   *
   * A list model stores data with the following order:
   *
   * O                    root node, the child node is (row 0, column 0)
   *  \
     *   O - O - O          row 0
   *   |
   *   O - O - O          row 1
   *   |
   *   O - O - O          row 2
   *   |
   *   .
   *   .
   *   ^   ^   ^
   *   |   |   |
   *   |   |   `------    column 2
   *   |   `----------    column 1
   *   `--------------    column 0
   */
  class AbstractListModel: public AbstractItemModel
  {
  public:

    AbstractListModel ();

    virtual ~AbstractListModel ();

    virtual bool InsertColumns (int column,
                                int count,
                                const ModelIndex& parent = ModelIndex());

    virtual bool RemoveColumns (int column,
                                int count,
                                const ModelIndex& parent = ModelIndex());

    /**
     * @brief Insert count rows into the model before the given row.
     * @param row The row before which will be inserted
     * @param count How many rows will be inserted
     * @param parent The parent ModelIndex
     * @return
     * 	- true if the rows were successfully inserted
     * 	- false if parent is invalid
     *
     * Insert count rows into the list model before the given row. Items in the new row will be
     * children of the item represented by the parent model index.
     *
     * If row is 0, the rows are prepended to any existing rows in the parent.
     * If row is greater than the current row count, the rows are appended to any existing rows in the parent
     * If parent has no child node, a single column with count rows is inserted
     */
    virtual bool InsertRows (int row, int count, const ModelIndex& parent =
                                 ModelIndex());

    /**
     * @brief Removes count rows starting with the given row under parent parent from the model
     * @param row
     * @param count
     * @param parent
     * @return
     */
    virtual bool RemoveRows (int row, int count, const ModelIndex& parent =
                                 ModelIndex());

    virtual ModelIndex GetRootIndex () const;

    virtual ModelIndex GetIndex (int row, int column, const ModelIndex& parent =
                                     ModelIndex()) const;

  protected:

    inline const ModelNode* root () const
    {
      return root_;
    }

  private:

    static void DestroyChildNode (ModelNode* node);

    static void DestroyRow (ModelNode * node);

    /**
     * @brief Clear and delete all child node from m_root->child
     */
    void ClearAllChildNodes ();

    ModelNode* root_;

  };

}
