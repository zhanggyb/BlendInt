#include "WidgetSim.hpp"

#include <iostream>

namespace BlendInt {
	
	WidgetSimPtr WidgetSim::Create ()
	{
		WidgetSimPtr ret(new WidgetSim);

		return ret;
	}

	WidgetSim::WidgetSim()
	: Object(),
	  m_parent(0)
	{

	}

	WidgetSim::~WidgetSim()
	{

	}

	void WidgetSim::AddChild (const WidgetSimPtr& child)
	{
		m_children.insert(child);
	}

	void WidgetSim::print ()
	{
		for(std::set<WidgetSimPtr>::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			std::cout << "count: " << (*it)->reference_count() << std::endl;
		}
	}

}
