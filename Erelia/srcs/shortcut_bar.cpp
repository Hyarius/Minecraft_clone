#include "erelia.h"

Shortcut_bar::Shortcut_bar(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_selected_item = nullptr;
	_shortcut_frame = new jgl::Frame(this);
	_shortcut_frame->activate();
	_item_slots.resize(9);
	for (size_t i = 0; i < 9; i++)
	{
		_item_slots[i] = new Item_slot(icon_tile, nullptr, _shortcut_frame);
		_item_slots[i]->activate();
	}

	select_itemslot(0);
}
void Shortcut_bar::select_itemslot(size_t index)
{
	if (index >= 9)
		return;

	if (_selected_item != nullptr)
		_selected_item->unselect();
	_selected_item = _item_slots[index];
	if (_selected_item != nullptr)
		_selected_item->select();
}

void Shortcut_bar::set_item(size_t index, Item* p_item)
{
	if (index >= 9)
		return;
	_item_slots[index]->set_item(p_item);
}

bool Shortcut_bar::handle_mouse()
{
	if (_disable == true)
		return false;
	
	static int index = 0;
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release)
	{
		for (int i = 0; i < 9; i++)
		{
			if (_item_slots[i]->is_pointed(jgl::get_mouse_pos()) == true)
			{
				select_itemslot(i);
				index = i;
				return (true);
			}
		}
	}
	if (g_mouse->wheel != 0 && jgl::get_key(jgl::key::left_control) != jgl::key_state::down)
	{
		index -= static_cast<int>(round(g_mouse->wheel / 2.0f));
		while (index < 0)
			index += 9;
		index = index % 9;
		select_itemslot(index);
		return (true);
	}
	return (false);
}

void Shortcut_bar::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_shortcut_frame->set_geometry(0, p_area);

	jgl::Vector2 size = Item_slot::size();
	jgl::Vector2 pos = (p_area.y - size.y) / 2;
	float delta = (p_area.x - size.x * 9 - pos.x * 2) / 8 + size.x;
	for (size_t i = 0; i < 9; i++)
	{
		_item_slots[i]->set_geometry(pos, size);
		pos.x += delta;
	}
}

void Shortcut_bar::render()
{

}