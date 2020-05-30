#include "erelia.h"

size_t inventory_tab_size[] = {
	30, 20, 10, 10, 10
};
Vector2 library_nb_element = Vector2(10, 6);
Vector2 button_nb_element = Vector2(2.0f, library_nb_element.y);

int block_tab_item_id[30] = {
		0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
		10,	11,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1
};

int decor_tab_item_id[20] = {
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1
};

int* inventory_tab_item_id[] =
{
	block_tab_item_id,
	decor_tab_item_id
};

Editor_inventory_tab::Editor_inventory_tab(size_t p_index, jgl::Widget* parent)
{
	_tab_index = p_index;
	_frame = new jgl::Frame(parent);
	_tab_index = p_index;
	_nb_slot = inventory_tab_size[_tab_index];
	_items_slot.resize(_nb_slot);
	for (size_t i = 0; i < _nb_slot; i++)
	{
		size_t item_id = inventory_tab_item_id[_tab_index][i];
		_items_slot[i] = new Item_slot(icon_tile, get_item(item_id), _frame);
		_items_slot[i]->activate();
	}
}

void Editor_inventory_tab::activate()
{
	_frame->activate();
}

void Editor_inventory_tab::desactivate()
{
	_frame->desactivate();
}

void Editor_inventory_tab::set_geometry(Vector2 p_anchor, Vector2 p_area)
{
	_frame->set_geometry(p_anchor, p_area);
	for (size_t i = 0; i < _nb_slot; i++)
	{
		Vector2 slot_pos = Vector2(
			5 + (Item_slot::size().x + 5) * (i % static_cast<size_t>(library_nb_element.x)),
			5 + (Item_slot::size().y + 5) * (i / static_cast<size_t>(library_nb_element.x))
		);
		_items_slot[i]->set_geometry(slot_pos, Item_slot::size());
	}
}