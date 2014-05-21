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

#ifndef _BLENDINT_GUI_ABSTRACTVECTORCONTAINER_HPP_
#define _BLENDINT_GUI_ABSTRACTVECTORCONTAINER_HPP_

#include <vector>
#include <boost/smart_ptr.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	typedef std::vector<AbstractWidget*> WidgetVector;

	class VectorIterator: public AbstractContainerIterator
	{
	public:

		VectorIterator (WidgetVector* vector)
		: AbstractContainerIterator(), m_vector_ptr(vector)
		{
			m_it = vector->begin();
		}

		VectorIterator (const VectorIterator& orig)
		: AbstractContainerIterator(),
		  m_vector_ptr(orig.m_vector_ptr),
		  m_it(orig.m_it)
		{
		}

		virtual ~VectorIterator ()
		{

		}

		VectorIterator& operator = (const VectorIterator& orig)
		{
			m_vector_ptr = orig.m_vector_ptr;
			m_it = orig.m_it;

			return *this;
		}

		virtual AbstractWidget* GetWidget () const
		{
			return *m_it;
		}

		virtual void GoToFirst ()
		{
			m_it = m_vector_ptr->begin();
		}

		virtual void GoNext ()
		{
			do {
				m_it++;
				if(m_it != m_vector_ptr->end() && (*m_it)) break;
			} while (m_it != m_vector_ptr->end());
		}

		virtual bool IsEnd ()
		{
			return m_it == m_vector_ptr->end();
		}

	private:

		WidgetVector* m_vector_ptr;
		WidgetVector::iterator m_it;
	};

	/**
	 * @brief A container holds sub widgets with std::vector
	 */
	class AbstractVectorContainer: public AbstractContainer
	{
	public:

		explicit AbstractVectorContainer (size_t size);

		virtual ~AbstractVectorContainer ();

	protected:

		bool SetSubWidget (size_t index, AbstractWidget* widget);

		virtual ResponseType FocusEvent (bool focus);

		virtual bool RemoveSubWidget (AbstractWidget* widget);

		virtual IteratorPtr CreateIterator (const DeviceEvent& event);

		void MoveSubWidgets (int offset_x, int offset_y);

		void ClearSubWidgets ();

		size_t sub_widget_size () const
		{
			return m_sub_widgets->size();
		}

		AbstractWidget* sub_widget (size_t index) const
		{
			return m_sub_widgets->at(index);
		}

		WidgetVector* sub_widgets () const
		{
			return m_sub_widgets.get();
		}

	private:

		void OnSubWidgetDestroyed (AbstractWidget* widget);

		boost::scoped_ptr<WidgetVector> m_sub_widgets;
	};

}


#endif /* _BLENDINT_GUI_ABSTRACTVECTORCONTAINER_HPP_ */
