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

#ifndef _BLENDINT_GUI_ABSTRACTITEMMODEL_HPP_
#define _BLENDINT_GUI_ABSTRACTITEMMODEL_HPP_

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/Core/Types.hpp>

namespace BlendInt {

	class AbstractItemModel;
	class ModelIndex;

	extern bool operator == (const ModelIndex& src, const ModelIndex& dst);

	struct ModelNode
	{
		ModelNode ()
		: parent(0),
		  child(0),
		  up(0),
		  down(0),
		  left(0),
		  right(0)
		{
		}

		~ModelNode ()
		{
			// DBG_PRINT_MSG("Delete node: %s", ConvertFromString(data).c_str());
		}

		ModelNode* parent;
		ModelNode* child;
		ModelNode* up;
		ModelNode* down;
		ModelNode* left;
		ModelNode* right;
		String data;	// temporarily use String to store data
	};

	class ModelIndex
	{
	public:

		ModelIndex ();

		ModelIndex (const ModelIndex& orig);

		~ModelIndex ();

		ModelIndex& operator = (const ModelIndex& orig);

		int GetRow () const;

		int GetColumn () const;

		const String* GetData () const;	// Use String temporarily

		ModelIndex GetRootIndex () const;

		ModelIndex GetParentIndex () const;

		ModelIndex GetChildIndex (int row = 0, int column = 0) const;

		ModelIndex GetLeftIndex () const;

		ModelIndex GetRightIndex () const;

		ModelIndex GetUpIndex () const;

		ModelIndex GetDownIndex () const;

		ModelIndex GetSibling (int row, int column) const;

		bool IsValid () const;

		bool operator != (const ModelIndex& other) const;

		bool operator == (const ModelIndex& other) const;

	private:

		friend class AbstractItemModel;
		friend bool operator == (const ModelIndex& src, const ModelIndex& dst);

		ModelNode* node_;
	};

	class AbstractItemModel: public Object
	{

	public:

		AbstractItemModel ();

		virtual ~AbstractItemModel ();

		/**
		 * @brief Get the row number
		 */
		virtual int GetRows (const ModelIndex& parent = ModelIndex()) const = 0;

		/**
		 * @brief Get the column number
		 */
		virtual int GetColumns (const ModelIndex& parent = ModelIndex()) const = 0;

		virtual bool HasChild (const ModelIndex& parent = ModelIndex()) const;

		bool InsertColumn (int column, const ModelIndex& parent = ModelIndex ());

		virtual bool InsertColumns (int column, int count, const ModelIndex& parent = ModelIndex()) = 0;

		bool RemoveColumn (int column, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveColumns (int column, int count, const ModelIndex& parent = ModelIndex()) = 0;

		bool RemoveRow (int row, const ModelIndex& parent = ModelIndex());

		virtual bool RemoveRows (int row, int count, const ModelIndex& parent = ModelIndex()) = 0;

		bool InsertRow (int row, const ModelIndex& parent = ModelIndex());

		virtual bool InsertRows (int row, int count, const ModelIndex& parent = ModelIndex()) = 0;

		virtual ModelIndex GetRootIndex () const = 0;

		virtual ModelIndex GetIndex (int row, int column, const ModelIndex& parent = ModelIndex()) const = 0;

		virtual bool SetData (const ModelIndex& index, const String& data);	// temporarily use String

	protected:

		static void SetIndexNode (ModelIndex& index, ModelNode* node);

		static ModelNode* GetIndexNode (const ModelIndex& index);

	};

}

#endif /* _BLENDINT_GUI_ABSTRACTITEMMODEL_HPP_ */
