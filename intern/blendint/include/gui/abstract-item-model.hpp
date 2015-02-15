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

#include <core/object.hpp>
#include <core/string.hpp>

#include <gui/abstract-form.hpp>

namespace BlendInt {

	class AbstractItemModel;
	class ModelIndex;

	extern inline bool operator == (const ModelIndex& src, const ModelIndex& dst);

	struct ModelNode
	{
		inline ModelNode ()
		: parent(0),
		  child(0),
		  up(0),
		  down(0),
		  left(0),
		  right(0)
		{
		}

		inline ~ModelNode ()
		{
		}

		ModelNode* parent;
		ModelNode* child;
		ModelNode* up;
		ModelNode* down;
		ModelNode* left;
		ModelNode* right;

		RefPtr<AbstractForm> data;
	};

	class ModelIndex
	{
	public:

		inline ModelIndex ()
		: node_(0)
		{}

		inline ModelIndex (const ModelIndex& orig)
		: node_(orig.node_)
		{}

		inline ~ModelIndex ()
		{}

		ModelIndex& operator = (const ModelIndex& orig);

		int GetRow () const;

		int GetColumn () const;

		RefPtr<AbstractForm> GetData () const;

		const AbstractForm* GetRawData () const;

		bool SetData (const RefPtr<AbstractForm>& data);

		ModelIndex GetRootIndex () const;

		ModelIndex GetParentIndex () const;

		ModelIndex GetChildIndex (int row = 0, int column = 0) const;

		ModelIndex GetLeftIndex () const;

		ModelIndex GetRightIndex () const;

		ModelIndex GetUpIndex () const;

		ModelIndex GetDownIndex () const;

		ModelIndex GetSibling (int row, int column) const;

		inline bool valid () const
		{
			return node_ != 0;
		}

		inline bool operator != (const ModelIndex& other) const
		{
			return node_ != other.node_;
		}

		inline bool operator == (const ModelIndex& other) const
		{
			return node_ == other.node_;
		}

	private:

		friend class AbstractItemModel;
		friend inline bool operator == (const ModelIndex& src, const ModelIndex& dst);

		ModelNode* node_;
	};

	inline bool operator == (const ModelIndex& src, const ModelIndex& dst)
	{
		return src.node_ == dst.node_;
	}

	/**
	 * @brief The abstract interface for item model classes.
	 *
	 * The AbstractItemModel class defines the standard interface that
	 * item models must use to be able to interoperate with other
	 * components in the model/view architecture. It is not supposed
	 * to be instantiated directly. Instead, you should subclass it to
	 * create new model.
	 *
	 * @note The MVC in BlendInt is implemented with reference to Qt's
	 * <a
	 * href="http://doc.qt.io/qt-5/model-view-programming.html">model/view
	 * framework</a>.
	 */
	class AbstractItemModel: public Object
	{

	public:

		AbstractItemModel ();

		virtual ~AbstractItemModel ();

		/**
		 * @brief Get the row count
		 */
		virtual int GetRowCount (const ModelIndex& parent = ModelIndex()) const = 0;

		/**
		 * @brief Get the column count
		 */
		virtual int GetColumnCount (const ModelIndex& parent = ModelIndex()) const = 0;

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

		virtual bool SetData (const ModelIndex& index, const RefPtr<AbstractForm>& data);

	protected:

		static inline void set_index_node (ModelIndex& index, ModelNode* node)
		{
			index.node_ = node;
		}

		static inline ModelNode* get_index_node (const ModelIndex& index)
		{
			return index.node_;
		}

	};

}
