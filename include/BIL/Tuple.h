/*
 * Tuple
 *
 *  Created on: 2013年7月15日
 *      Author: zhanggyb
 */

#ifndef _BIL_TUPLE_
#define _BIL_TUPLE_

namespace BIL {

    template<typename T>
        union Tuple2
        {
            T data[2]; /**< All compnents at once */

            struct
            {
                T x;
                T y;
            } dimension;

        };

    template<typename T>
        union Tuple3
        {
            T data[3]; /**< All components at once */

            struct
            {
                T x; /**< Alias for first component */
                T y; /**< Alias for second component */
                T z; /**< Alias for third component */
            } dimension;

            struct
            {
                T r;
                T g;
                T b;
            } rgb;

            struct
            {
                T red;
                T green;
                T blue;
            } color;
        };

    template<typename T>
        union Tuple4
        {
            T data[4]; /**< All components at once */

            struct
            {
                T x; /**< Alias for first component */
                T y; /**< Alias for second component */
                T z; /**< Alias for third component */
                T w; /**< Alias for fourth component */
            } dimension;

            struct
            {
                T sx;
                T sy;
                T width;
                T height;
            } rect;

            struct
            {
                T r;
                T g;
                T b;
                T a;
            } rgba;

            struct
            {
                T red;
                T green;
                T blue;
                T alpha;
            } color;

        };

    typedef Tuple2<int> Tuple2i;
    typedef Tuple3<int> Tuple3i;
    typedef Tuple4<int> Tuple4i;

    typedef Tuple2<float> Tuple2f;
    typedef Tuple3<float> Tuple3f;
    typedef Tuple4<float> Tuple4f;

}       // namespace of BIL

#endif /* TUPLE_ */
