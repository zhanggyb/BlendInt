/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_VECTOR_HPP_
#define _BIL_VECTOR_HPP_

#include <string.h>
#include <assert.h>

namespace BIL {

	template <typename T, int N>
	class Vector
	{
	public:
		
		Vector ()
		{
			memset (data_, 0, sizeof(T) * N);
		}

		Vector (const T val[N])
		{
			for (int i = 0; i < N; i++)
			{
				data_[i] = val[i];
			}
		}

		const T& operator [] (int index) const
		{
			if (index >= N) return 0;

			return data_[index];
		}

		T& operator [] (int index)
		{
			assert (index < N);
			
			return data_[index];
		}

		Vector& operator = (const T val[N])
		{
			for (int i = 0; i < N; i++)
			{
				data_[i] = val[i];
			}
			return *this;
		}

		Vector& operator = (const Vector& orig)
		{
			memcpy (data_, orig.data_, N * sizeof(T));
			return *this;
		}

	private:
		
		T data_[N];
	};

}

#endif	// _BIL_VECTOR_HPP_
