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

#ifndef _BLENDINT_ABSTRACTEXTRAFORM_HPP_
#define _BLENDINT_ABSTRACTEXTRAFORM_HPP_

#include <BlendInt/AbstractRoundBox.hpp>

namespace BlendInt {

	enum ExpandableFormProperty {
		FormPosition = AbstrctFormPropertyLast + 1,
		FormPreferredSize,
		FormMinimalSize,
		FormMaximalSize,
		AbstractExtraFormPropertyLast = FormMaximalSize
	};

	/**
	 * @brief A Form which holds position and expandable along x or y axis
	 */
	class AbstractExtraForm: public AbstractForm
	{
	public:

		AbstractExtraForm();

		AbstractExtraForm(const AbstractExtraForm& orig);

		virtual ~AbstractExtraForm();

		AbstractExtraForm& operator = (const AbstractExtraForm& orig)
		{
			set_position(orig.position());
			resize(orig.size());
			m_expand_x = orig.expand_x();
			m_expand_y = orig.expand_y();
			m_preferred_size = orig.preferred_size();
			m_minimal_size = orig.minimal_size();
			m_maximal_size = orig.maximal_size();

			return *this;
		}

		const Point& position () const
		{
			return m_position;
		}

		const Size& preferred_size () const
		{
			return m_preferred_size;
		}

		const Size& minimal_size() const
		{
			return m_minimal_size;
		}

		const Size& maximal_size () const
		{
			return m_maximal_size;
		}

		/**
		 * @brief set the form's position
		 * @param x
		 * @param y
		 * @return true if new position is set, false if it's the same as the current position
		 */
		bool set_position (int x, int y);

		/**
		 * @brief set the form's position
		 * @param position
		 * @return true if new position is set, false if it's the same as the current position
		 */
		bool set_position (const Point& position);

		/**
		 * @brief reset the size of the form
		 * @param width
		 * @param height
		 * @return
		 *
		 * Hide the same function in base class
		 */
		bool resize (unsigned int width, unsigned int height);

		/**
		 * @brief reset the size of the form
		 * @param size
		 * @return
		 *
		 * Check if the given size is in [min, max], if is, call resize() in base class
		 */
		bool resize (const Size& size);

		/**
		 * @brief set the preferred size
		 * @param width
		 * @param height
		 * @return true if new preferred size is set, false if not
		 */
		bool set_preferred_size (unsigned int width, unsigned int height);

		/**
		 * @brief set the preferred size
		 * @param size
		 * @return true if new preferred size is set, false if not
		 */
		bool set_preferred_size (const Size& size);

		/**
		 * @brief set the minimal size
		 * @param width
		 * @param height
		 * @return true if the new minimal size is set, false if not
		 */
		bool set_minimal_size (unsigned int width, unsigned int height);

		/**
		 * @brief set the minimal size
		 * @param size
		 * @return true if the new minimal size is set, false if not
		 */
		bool set_minimal_size (const Size& size);

		/**
		 * @brief set the maximal size
		 * @param width
		 * @param height
		 * @return true if the new maximial size is set, false if not
		 */
		bool set_maximal_size (unsigned int width, unsigned int height);

		/**
		 * @brief set the maximal size
		 * @param size
		 * @return true if the new maximal size is set, false if not
		 */
		bool set_maximal_size (const Size& size);

		void set_expand_x (bool expand) {m_expand_x = expand;}

		bool expand_x () const {return m_expand_x;}

		void set_expand_y (bool expand) {m_expand_y = expand;}

		bool expand_y () const {return m_expand_y;}

		void set_expand (bool expand) {m_expand_x = expand; m_expand_y = expand;}

		bool contain (const Point& point);

		bool contain (int x, int y);

	private:

		bool m_expand_x;

		bool m_expand_y;

		Point m_position;

		Size m_preferred_size;

		Size m_minimal_size;

		Size m_maximal_size;
	};

}

#endif /* _BLENDINT_ABSTRACTEXTRAFORM_HPP_ */
