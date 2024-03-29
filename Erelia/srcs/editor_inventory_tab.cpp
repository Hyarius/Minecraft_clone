#include "erelia.h"

size_t inventory_tab_size[] = {
	60, 45, 30, 30, 30
};
Vector2 library_nb_element = Vector2(10, 6);
Vector2 button_nb_element = Vector2(2.0f, library_nb_element.y);

int block_tab_item_id[60] = {
		0,	1,	2,	3,	4,	5,	6,	7,	8,	9,
		10,	11,	12,	13,	14,	15,	16,	17,	18,	19,
		20,	21,	22,	23,	24,	25,	26,	27,	28,	29,
		30,	31,	32,	33,	34,	35,	36,	37,	38,	39,
		40,	41,	42,	43,	44,	45,	46,	47,	48,	49,
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1
};

int decor_tab_item_id[45] = {
		100,101,102,103,104,105,106,107,108,109,
		110,111,112,113,114,115,116,117,118,119,
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
		-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
		-1,	-1,	-1,	-1,	-1
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
	_scrollbar = new jgl::Vscroll_bar(_frame);
	_scrollbar->activate();
	_tab_index = p_index;
	_nb_slot = inventory_tab_size[_tab_index];
	_items_slot.resize(_nb_slot);
	for (size_t i = 0; i < _nb_slot; i++)
	{
		int item_id = inventory_tab_item_id[_tab_index][i];
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

void Editor_inventory_tab::reset()
{
	for (size_t i = 0; i < _nb_slot; i++)
	{
		Vector2 slot_pos = Vector2(
			5 + (Item_slot::size().x + 5) * (i % static_cast<size_t>(library_nb_element.x)),
			5 + (Item_slot::size().y + 5) * (i / static_cast<size_t>(library_nb_element.x))
		);
		_items_slot[i]->set_geometry(slot_pos, Item_slot::size());
	}
}

void Editor_inventory_tab::set_geometry(Vector2 p_anchor, Vector2 p_area)
{
	_frame->set_geometry(p_anchor, p_area - Vector2(20, 0));
	_scrollbar->set_geometry(Vector2(p_area.x - 20, 0.0f), Vector2(20.0f, p_area.y));
	reset();
}