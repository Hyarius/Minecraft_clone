#include "erelia.h"

Item::Item(jgl::String p_name, int p_id, Vector2 p_icon)
{
	name = p_name;
	type = item_type::error;
	id = p_id;
	icon = p_icon;
}

Block_item::Block_item(jgl::String p_name, int p_id, Vector2 p_icon, int p_block_id) : Item(p_name, p_id, p_icon)
{
	type = item_type::block;
	block_id = p_block_id;
}

void Block_item::use(jgl::Data data)
{
	if (data.content.size() == 0)
		return;

	Board* board = data.acces<Board*>(0);
	Vector3 coord = *(data.acces<Vector3*>(1));

	board->place_block(coord, block_id);
}