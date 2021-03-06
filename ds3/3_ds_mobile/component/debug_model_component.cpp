//
// The MIT License(MIT)
//
// Copyright(c) 2014 Demonsaw LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cassert>

#include "component/debug_model_component.h"
#include "component/error_component.h"
#include "component/callback/callback_action.h"
#include "component/callback/callback_type.h"

#include "controller/application_controller.h"
#include "controller/debug_controller.h"
#include "thread/thread_info.h"

namespace eja
{
	// Constructor
	debug_model_component::debug_model_component(QObject* parent /*= nullptr*/) : model_component(parent)
	{
		// Callback
		add_callback(callback_action::add, [&](const entity::ptr entity) { add(entity); });
	}

	// Interface
	void debug_model_component::init()
	{
		component::init();

		set_type(callback_type::error);
	}

	void debug_model_component::on_add(const entity::ptr entity)
	{
		assert(thread_info::main());

		const auto owner = get_entity();
		const auto app = application_controller::get();

		if ((owner == app->get_entity()) /*&& !app->suspended()*/)
		{
			// Active
			const auto control = app->get_debug_controller();
			control->add(entity);
		}
		else
		{
			// Inactive
			const auto parent = entity->get_parent();
			const auto error_list = parent->get<error_list_component>();
			error_list->push_back(entity);
		}
	}
}
