// Copyright (c) 2010 Nickolas Pohilets
//
// This file is a part of the CppEvents library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef __CPP__EVENTS__CONNECTION__HPP
#define __CPP__EVENTS__CONNECTION__HPP

#include <Cpp/Events/AbstractConnection.hpp>

namespace Cpp {
	namespace Private {
		namespace Events {
			//------------------------------------------------------------------------------

			class Connection0: public AbstractConnection
			{
			protected:

				virtual ~Connection0 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate0<void> DelegateType;

				Connection0 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke () const
				{
					delegate_();
				}
			protected:
				DelegateType delegate_;

				Connection0 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0> class Connection1: public AbstractConnection
			{
			protected:

				virtual ~Connection1 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate1<Param0, void> DelegateType;

				Connection1 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0) const
				{
					delegate_(p0);
				}
			protected:
				DelegateType delegate_;

				Connection1 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1> class Connection2: public AbstractConnection
			{
			protected:

				virtual ~Connection2 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate2<Param0, Param1, void> DelegateType;

				Connection2 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1) const
				{
					delegate_(p0, p1);
				}
			protected:
				DelegateType delegate_;

				Connection2 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2> class Connection3: public AbstractConnection
			{
			protected:

				virtual ~Connection3 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate3<Param0, Param1, Param2, void> DelegateType;

				Connection3 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2) const
				{
					delegate_(p0, p1, p2);
				}
			protected:
				DelegateType delegate_;

				Connection3 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2, class Param3> class Connection4: public AbstractConnection
			{
			protected:

				virtual ~Connection4 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate4<Param0, Param1, Param2,
				        Param3, void> DelegateType;

				Connection4 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2, Param3 p3) const
				{
					delegate_(p0, p1, p2, p3);
				}
			protected:
				DelegateType delegate_;

				Connection4 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2, class Param3,
			        class Param4> class Connection5: public AbstractConnection
			{
			protected:

				virtual ~Connection5 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate5<Param0, Param1, Param2,
				        Param3, Param4, void> DelegateType;

				Connection5 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2, Param3 p3,
				        Param4 p4) const
				{
					delegate_(p0, p1, p2, p3, p4);
				}
			protected:
				DelegateType delegate_;

				Connection5 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2, class Param3,
			        class Param4, class Param5> class Connection6: public AbstractConnection
			{
			protected:

				virtual ~Connection6 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate6<Param0, Param1, Param2,
				        Param3, Param4, Param5, void> DelegateType;

				Connection6 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2, Param3 p3,
				        Param4 p4, Param5 p5) const
				{
					delegate_(p0, p1, p2, p3, p4, p5);
				}
			protected:
				DelegateType delegate_;

				Connection6 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2, class Param3,
			        class Param4, class Param5, class Param6> class Connection7: public AbstractConnection
			{
			protected:

				virtual ~Connection7 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate7<Param0, Param1, Param2,
				        Param3, Param4, Param5, Param6, void> DelegateType;

				Connection7 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2, Param3 p3,
				        Param4 p4, Param5 p5, Param6 p6) const
				{
					delegate_(p0, p1, p2, p3, p4, p5, p6);
				}
			protected:
				DelegateType delegate_;

				Connection7 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
			//------------------------------------------------------------------------------

			template<class Param0, class Param1, class Param2, class Param3,
			        class Param4, class Param5, class Param6, class Param7> class Connection8: public AbstractConnection
			{
			protected:

				virtual ~Connection8 ()
				{
				}
			public:
				typedef fastdelegate::FastDelegate8<Param0, Param1, Param2,
				        Param3, Param4, Param5, Param6, Param7, void> DelegateType;

				Connection8 (DelegateType const & deleg)
						: AbstractConnection(AbstractDelegate(deleg)), delegate_(
						        deleg)
				{
				}

				void invoke (Param0 p0, Param1 p1, Param2 p2, Param3 p3,
				        Param4 p4, Param5 p5, Param6 p6, Param7 p7) const
				{
					delegate_(p0, p1, p2, p3, p4, p5, p6, p7);
				}
			protected:
				DelegateType delegate_;

				Connection8 (AbstractDelegate const & memento)
						: AbstractConnection(memento), delegate_()
				{
				}
			};
		//------------------------------------------------------------------------------
		}//namespace Events
	} //namespace Private
} //namespace Cpp

#endif //__CPP__EVENTS__CONNECTION__HPP
