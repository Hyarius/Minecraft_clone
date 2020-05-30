#include "erelia.h"

jgl::Vector2 Item_slot::_size = 80;

jgl::Sprite_sheet* icon_tile = nullptr;

jgl::String items_name[] = {
	"Grass",
	"Dirt",
	"Water",
	"Snow",
	"Forest grass",
	"Dirt",
	"Water",
	"Stone",
	"Haunted forest grass",
	"Sand",
	"Water",
	"Dark stone"
};

Vector2 items_icon[] = {
	{0, 0},
	{1, 0},
	{2, 0},
	{3, 0},
	{4, 0},
	{5, 0},
	{6, 0},
	{7, 0},
	{8, 0},
	{9, 0},
	{0, 1},
	{1, 1},
};

Item** item_list;

Item* get_item(int index)
{
	if (index >= static_cast<int>(Item_id::count) || index < 0)
		return (nullptr);
	return (item_list[index]);
}