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

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt
{

	ContextLayerExt::ContextLayerExt ()
			: refresh(true), widgets(0), buffer(0)
	{
	}

	ContextLayerExt::~ContextLayerExt ()
	{
		if (buffer) {
			DBG_PRINT_MSG("%s", "Delete texture buffer in context layer");
			delete buffer;
		}

		if (widgets) {
			DBG_PRINT_MSG("%s", "Delete widget set in context layer");
			assert(widgets->size() == 0);
			widgets->clear();
			delete widgets;
		}
	}

	Context::Context ()
	: AbstractContainerExt()
	{
	}

	Context::~Context ()
	{
	}

	bool Context::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			return true;
		} else {

			return false;
		}
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	bool Context::AddSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget)
			return false;

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
				        widget->name().c_str(),
				        widget->container()->name().c_str());
				return true;
			} else {
				// Set widget's container to 0
				AbstractContainerExt::RemoveSubWidget(widget->container(),
				        widget);
			}
		}

		map<int, ContextLayerExt>::iterator layer_iter;
		layer_iter = m_layers.find(widget->z());
		if (layer_iter != m_layers.end()) {
			layer_iter->second.widgets->insert(widget);
		} else {
			std::set<AbstractWidgetExt*>* new_widget_set_p = new std::set<AbstractWidgetExt*>;
			new_widget_set_p->insert(widget);
			m_layers[widget->z()].widgets = new_widget_set_p;

			// Refresh this layer in the render loop
			m_layers[widget->z()].refresh = true;
			refresh_once = true;
		}

		m_index[widget] = widget->z();

		events()->connect(widget->destroyed(), this, &Context::OnSubWidgetDestroyed);

		return true;
	}

	bool Context::RemoveSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
				&Context::OnSubWidgetDestroyed);

		if (AbstractWidgetExt::focused_widget == widget) {
			widget->m_flag.reset(AbstractWidgetExt::WidgetFlagFocusExt);
			AbstractWidgetExt::focused_widget = 0;
		}

		map<AbstractWidgetExt*, int>::iterator index_iter;

		index_iter = m_index.find(widget);

		if (index_iter != m_index.end()) {

			int z = index_iter->second;

			std::set<AbstractWidgetExt*>* widget_set_p = m_layers[z].widgets;
			std::set<AbstractWidgetExt*>::iterator widget_iter = widget_set_p->find(widget);
			if (widget_iter != widget_set_p->end()) {
				widget_set_p->erase(widget_iter);
			} else {
				DBG_PRINT_MSG("Error: object %s is not recorded in set",
						widget->name().c_str());
			}

			if (widget_set_p->empty()) {
				DBG_PRINT_MSG("layer %d is empty, delete it", z);
				//delete widget_set_p; widget_set_p = 0;
				delete m_layers[z].widgets;
				m_layers[z].widgets = 0;

				if(m_layers[z].buffer) {
					m_layers[z].buffer->Clear();
					delete m_layers[z].buffer;
					m_layers[z].buffer = 0;
				}
				m_layers.erase(z);
			}

			m_index.erase(widget);

		} else {
			DBG_PRINT_MSG("Error: object %s is not recorded in map",
					widget->name().c_str());
			return false;
		}

		return true;
	}

	AbstractWidgetIterator* Context::First (const DeviceEvent& event)
	{
		// A Context object only interacts with Interface
		return 0;
	}

	bool Context::End (const DeviceEvent& event, AbstractWidgetIterator* iter)
	{
		// A Context object only interacts with Interface
		return false;
	}

	int Context::GetMaxLayer () const
	{
		map<int, ContextLayerExt>::const_reverse_iterator rit = m_layers.rbegin();

		return rit->first;
	}

	void Context::RefreshLayer (int layer)
	{
		map<int, ContextLayerExt>::iterator layer_iter;

		layer_iter = m_layers.find(layer);

		if (layer_iter != m_layers.end()) {
			m_layers[layer].refresh = true;
			refresh_once = true;
		}
	}

	void Context::OnSubWidgetDestroyed (AbstractWidgetExt* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the context manager", widget->name().c_str());

		RemoveSubWidget(widget);

		//widget->m_flag.reset(AbstractWidget::WidgetFlagInContextManager);
		//widget->destroyed().disconnectOne(this, &Context::OnSubWidgetDestroyed);
	}

}
