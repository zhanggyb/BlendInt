#ifndef _BLENDINT_WIDGET_SIM_HPP_
#define _BLENDINT_WIDGET_SIM_HPP_

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/RefPtr.hpp>

#include <set>

namespace BlendInt {

	class WidgetSim;

	typedef RefPtr<WidgetSim> WidgetSimPtr;
	
	class WidgetSim: public Object
	{
	public:

		WidgetSimPtr Create ();

		WidgetSim ();

		virtual ~WidgetSim ();

		void AddChild (const WidgetSimPtr& child);

		void print();

	private:

		template <typename T> friend class RefPtr;

		WidgetSim* m_parent;

		std::set<WidgetSimPtr > m_children;

	};

}

#endif
