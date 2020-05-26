#include "erelia.h"

jgl::Sprite_sheet* icon_tile = nullptr;

Item* item_list[12] = {
	new Block_item(0, 0, 0),
	new Block_item(1, 1, 1),
	new Block_item(2, 2, 2),
	new Block_item(3, 3, 3),
	new Block_item(4, 4, 4),
	new Block_item(5, 5, 5),
	new Block_item(6, 6, 6),
	new Block_item(7, 7, 7),
	new Block_item(8, 8, 8),
	new Block_item(9, 9, 9),
	new Block_item(10, 10, 10),
	new Block_item(11, 11, 11)
};