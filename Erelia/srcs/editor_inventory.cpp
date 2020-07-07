#include "erelia.h"


jgl::String inventory_tab_text[] = {
	"Blocs",
	"Decor"
};
extern Vector2 library_nb_element;
extern Vector2 button_nb_element;

void editor_inventory_active_frame(jgl::Data param)
{
	Editor_inventory* editor = param.acces<Editor_inventory*>(0);
	Editor_inventory_tab* target = param.acces<Editor_inventory_tab*>(1);
	size_t max_size = editor->item_library().size();

	for (size_t i = 0; i < static_cast<size_t>(Tab_type::count); i++)
	{
		if (target == editor->item_library(i))
		{
			editor->select_tab(i);
			return ;
		}
	}
}

void Editor_inventory::select_tab(size_t index)
{
	if (index >= _item_library.size())
		return;
	_item_library[_selected_tab_index]->desactivate();
	_selected_tab_index = index;
	_item_library[index]->activate();
	_item_library[index]->reset();
}

Editor_inventory::Editor_inventory(Player_controller* controller, jgl::Widget* parent) : jgl::Widget(parent)
{
	_controller = controller;
	_selected_tab_index = 0;
	_clicked_slot = nullptr;
	_shortcut_bar = new Shortcut_bar(this);
	for (size_t i = 0; i < 9; i++)
	{
		_shortcut_bar->set_item(i, item_list[i]);
	}
	_shortcut_bar->activate();

	_inventory_frame = new jgl::Frame(this);
	_item_library.resize(static_cast<size_t>(Tab_type::count));
	_library_button.resize(static_cast<size_t>(Tab_type::count));
	for (size_t i = 0; i < _item_library.size(); i++)
	{
		_item_library[i] = new Editor_inventory_tab(i, _inventory_frame);
		_library_button[i] = new jgl::Button(editor_inventory_active_frame, jgl::Data(2, this, _item_library[i]), _inventory_frame);
		_library_button[i]->set_text(inventory_tab_text[i]);
		_library_button[i]->activate();
	}
	select_tab(0);
	
	_icon_holder = new Icon_holder(icon_tile, this);
	_icon_holder->send_front();
	_icon_holder->activate();
}

Editor_inventory::~Editor_inventory()
{
	for (size_t i = 0; i < _item_library.size(); i++)
	{
		if (_item_library[i] != nullptr)
			delete _item_library[i];
	}
}

Item_slot* Editor_inventory::find_shortcut_slot()
{
	Item_slot* tmp;

	for (size_t i = 0; i < _shortcut_bar->item_slot().size(); i++)
	{
		tmp = _shortcut_bar->item_slot(i);
		if (tmp->is_pointed() == true)
			return (tmp);
	}
	return (nullptr);
}

void Editor_inventory::disable()
{
	g_mouse->place(g_application->size() / 2);
	g_mouse->actualize();
	_controller->set_frozen(false);
	_inventory_frame->desactivate();
	_shortcut_bar->set_frozen(false);
	_item_library[_selected_tab_index]->reset();
}

void Editor_inventory::enable()
{
	_controller->set_frozen(true);
	_inventory_frame->activate();
	_shortcut_bar->set_frozen(true);
	_item_library[_selected_tab_index]->reset();
}

Item_slot* Editor_inventory::find_inventory_slot()
{
	Item_slot* tmp;

	for (size_t i = 0; i < _item_library[_selected_tab_index]->item_slot().size(); i++)
	{
		tmp = _item_library[_selected_tab_index]->item_slot(i);
		if (tmp->is_pointed() == true)
			return (tmp);
	}
	return (nullptr);
}

bool Editor_inventory::toggle()
{
	return (_inventory_frame->is_active());
}

bool Editor_inventory::handle_keyboard()
{
	if (jgl::get_key(jgl::key::e) == jgl::key_state::release)
	{
		if (toggle() == false)
		{
			enable();
		}
		else
		{
			disable();
		}
		return (true);
	}
	if (jgl::get_key(jgl::key::escape) == jgl::key_state::release)
	{
		if (toggle() == true)
		{
			disable();
			return (true);
		}
	}
	return (false);
}

bool Editor_inventory::is_shortcut_slot(Item_slot *slot)
{
	for (size_t i = 0; i < _shortcut_bar->item_slot().size(); i++)
		if (slot == _shortcut_bar->item_slot(i))
			return (true);
	return (false);
}

bool Editor_inventory::handle_mouse()
{
	if (toggle() == false)
		return (false);

	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::pressed)
	{
		_clicked_slot = find_slot();
		if (_clicked_slot != nullptr)
			_icon_holder->set_item(_clicked_slot->item());
	}
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		Item_slot* tmp = find_shortcut_slot();
		Item* clicked_item = (_clicked_slot == nullptr ? nullptr : _clicked_slot->item());
		Item* tmp_item = (tmp == nullptr ? nullptr : tmp->item());
		if (is_shortcut_slot(_clicked_slot) == true)
			_clicked_slot->set_item(tmp_item);
		if (tmp != nullptr)
			tmp->set_item(clicked_item);
		_clicked_slot = nullptr;
		_icon_holder->set_item(nullptr);
	}
	return (false);
}

void Editor_inventory::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{
	Item_slot::set_size(p_area.x / 11 > 80 ? 80 : p_area.x / 11);
	Vector2 size = Vector2(Item_slot::size().x * 9 + 60, Item_slot::size().y + 20);
	Vector2 pos = Vector2((p_area.x - size.x) / 2, p_area.y - size.y * 1.2f);
	_shortcut_bar->set_geometry(pos, size);

	Vector2 nb_element = Vector2(library_nb_element.x + button_nb_element.x, library_nb_element.y);
	size = Vector2(
		Item_slot::size().x * (nb_element.x) + 5 * (nb_element.x - 1) + 30,
		Item_slot::size().y * nb_element.y - 5 * (nb_element.y - 1) + 20
	);
	pos.x = (p_area.x - size.x) / 2;
	pos.y = (pos.y - size.y) / 2;

	_inventory_frame->set_geometry(pos, size + Vector2(20, 0));

	Vector2 library_size = Vector2(
		Item_slot::size().x * (library_nb_element.x) + 5 * (library_nb_element.x - 1) + 30,
		Item_slot::size().y * library_nb_element.y - 5 * (library_nb_element.y - 1)
	);
	Vector2 button_size = Item_slot::size();
	Vector2 button_pos;
	for (size_t i = 0; i < static_cast<size_t>(Tab_type::count); i++)
	{
		_item_library[i]->set_geometry(10, library_size);
		button_pos = Vector2(
			10 + library_size.x + 10 + (button_size.x + 5) * (i % static_cast<size_t>(button_nb_element.x)),
			10 + (button_size.y + 5) * (i / static_cast<size_t>(button_nb_element.x))
		);
		_library_button[i]->set_geometry(button_pos, button_size);
	}

	_icon_holder->set_geometry(0, p_area);

}

void Editor_inventory::update()
{
}

void Editor_inventory::render()
{

}
