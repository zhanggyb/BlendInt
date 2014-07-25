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

#ifndef _BLENDINT_GUI_ABSTRACTLISTMODEL_HPP_
#define _BLENDINT_GUI_ABSTRACTLISTMODEL_HPP_

#include <BlendInt/Gui/AbstractItemModel.hpp>

namespace BlendInt {

	class AbstractListModel: public AbstractItemModel
	{
	public:

		AbstractListModel ();

		virtual ~AbstractListModel ();

		//virtual int GetRows (const ModelIndex& parent = ModelIndex()) const;

		//virtual int GetColumns (const ModelIndex& parent = ModelIndex()) const;

		virtual bool InsertColumns (int column, int count, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveColumns (int column, int count, const ModelIndex& parent = ModelIndex());

		virtual bool InsertRows (int row, int count, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveRows (int row, int count, const ModelIndex& parent = ModelIndex());

		virtual ModelIndex GetRootIndex () const;

		virtual ModelIndex GetIndex (int row, int column, const ModelIndex& parent = ModelIndex()) const;

	protected:

		const ModelNode* root () const
		{
			return m_root;
		}

	private:

		static void DestroyChildNode (ModelNode* node);

		static void DestroyRow (ModelNode * node);

		static void DestroyColumn (ModelNode* node);

		/**
		 * @brief Clear and delete all child node from m_root->child
		 */
		void ClearAllChildNodes ();

		ModelNode* m_root;

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTLISTMODEL_HPP_ */
