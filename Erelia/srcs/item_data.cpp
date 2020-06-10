#include "erelia.h"

jgl::Vector2 Item_slot::_size = 80;

jgl::Sprite_sheet* icon_tile = nullptr;

std::vector<float> block_alpha_array = {
	1.0f, // 0 / 0
	1.0f, // 1 / 0
	1.0f, // 2 / 0
	1.0f, // 3 / 0
	1.0f, // 4 / 0
	1.0f, // 5 / 0
	1.0f, // 6 / 0
	1.0f, // 7 / 0
	1.0f, // 8 / 0
	1.0f, // 9 / 0
	1.0f, // 0 / 1
	0.5f, // 1 / 1
	0.5f, // 2 / 1
	0.5f, // 3 / 1
	1.0f, // 4 / 1
	1.0f, // 5 / 1
	1.0f, // 6 / 1
	1.0f, // 7 / 1
	1.0f, // 8 / 1
	1.0f, // 9 / 1
	1.0f, // 0 / 2
	1.0f, // 1 / 2
	1.0f, // 2 / 2
	1.0f, // 3 / 2
	1.0f, // 4 / 2
	1.0f, // 5 / 2
	1.0f, // 6 / 2
	1.0f, // 7 / 2
	1.0f, // 8 / 2
	1.0f, // 9 / 2
	1.0f, // 0 / 3
	1.0f, // 1 / 3
	1.0f, // 2 / 3
	1.0f, // 3 / 3
	0.5f, // 4 / 3
	0.5f, // 5 / 3
	0.5f, // 6 / 3
	1.0f, // 7 / 3
	1.0f, // 8 / 3
	1.0f, // 9 / 3
	1.0f, // 0 / 4
	1.0f, // 1 / 4
	1.0f, // 2 / 4
	1.0f, // 3 / 4
	1.0f, // 4 / 4
	1.0f, // 5 / 4
	1.0f, // 6 / 4
	1.0f, // 7 / 4
	1.0f, // 8 / 4
	1.0f, // 9 / 4
};

std::vector<Vector2> block_uv_delta = {
	{0, 0},
	{4, 0},
	{8, 0},
	{12, 0},
	{16, 0},
	{20, 0},
	{24, 0},
	{28, 0},
	{32, 0},
	{36, 0},
	{0, 6},
	{4, 6},
	{8, 6},
	{12, 6},
	{16, 6},
	{20, 6},
	{24, 6},
	{28, 6},
	{32, 6},
	{36, 6},
	{0, 12},
	{4, 12},
	{8, 12},
	{12, 12},
	{16, 12},
	{20, 12},
	{24, 12},
	{28, 12},
	{32, 12},
	{36, 12},
	{0, 18},
	{4, 18},
	{8, 18},
	{12, 18},
	{16, 18},
	{20, 18},
	{24, 18},
	{28, 18},
	{32, 18},
	{36, 18},
	{0, 24},
	{4, 24},
	{8, 24},
	{12, 24},
	{16, 24},
	{20, 24},
	{24, 24},
	{28, 24},
	{32, 24},
	{36, 24},
};

std::vector<Item*> item_list = {
	new Block_item("Orange tile", 0, Vector2(0, 0), 0),
	new Block_item("Grey tile", 1, Vector2(1, 0), 1),
	new Block_item("Red tile", 2, Vector2(2, 0), 2),
	new Block_item("Brown tile", 3, Vector2(3, 0), 3),
	new Block_item("Tiled stone", 4, Vector2(4, 0), 4),
	new Block_item("Dark tiled stone", 5, Vector2(5, 0), 5),
	new Block_item("Sand", 6, Vector2(6, 0), 6),
	new Block_item("Grass", 7, Vector2(7, 0), 7),
	new Block_item("Dirt", 8, Vector2(8, 0), 8),
	new Block_item("Cave dirt", 9, Vector2(9, 0), 9),
	new Block_item("Black tile", 10, Vector2(0, 1), 10),
	new Block_item("Glass", 11, Vector2(1, 1), 11),
	new Block_item("Blue glass", 12, Vector2(2, 1), 12),
	new Block_item("Grey glass", 13, Vector2(3, 1), 13),
	new Block_item("Blue tiled stone", 14, Vector2(4, 1), 14),
	new Block_item("Green tiled stone", 15, Vector2(5, 1), 15),
	new Block_item("Sand", 16, Vector2(6, 1), 16),
	new Block_item("Grass", 17, Vector2(7, 1), 17),
	new Block_item("Dirt", 18, Vector2(8, 1), 18),
	new Block_item("Cave dirt", 19, Vector2(9, 1), 19),
	new Block_item("Plank - Normal", 20, Vector2(0, 2), 20),
	new Block_item("Plank - Light", 21, Vector2(1, 2), 21),
	new Block_item("Plank - Dark", 22, Vector2(2, 2), 22),
	new Block_item("Plank - Purple", 23, Vector2(3, 2), 23),
	new Block_item("Pink tiled stone", 24, Vector2(4, 2), 24),
	new Block_item("Red tiled stone", 25, Vector2(5, 2), 25),
	new Block_item("Sand", 26, Vector2(6, 2), 26),
	new Block_item("Grass", 27, Vector2(7, 2), 27),
	new Block_item("Dirt", 28, Vector2(8, 2), 28),
	new Block_item("Cave dirt", 29, Vector2(9, 2), 29),
	new Block_item("Leaves - Normal", 30, Vector2(0, 3), 30),
	new Block_item("Leaves - Orange", 31, Vector2(1, 3), 31),
	new Block_item("Leaves - Dark", 32, Vector2(2, 3), 32),
	new Block_item("Leaves - Purple", 33, Vector2(3, 3), 33),
	new Block_item("Water", 34, Vector2(4, 3), 34),
	new Block_item("Water", 35, Vector2(5, 3), 35),
	new Block_item("Swamp water", 36, Vector2(6, 3), 36),
	new Block_item("Grass", 37, Vector2(7, 3), 37),
	new Block_item("Dirt", 38, Vector2(8, 3), 38),
	new Block_item("Cave dirt", 39, Vector2(9, 3), 39),
	new Block_item("Wood - Normal", 40, Vector2(0, 4), 40),
	new Block_item("Wood - Light", 41, Vector2(1, 4), 41),
	new Block_item("Wood - Dark", 42, Vector2(2, 4), 42),
	new Block_item("Wood - Purple", 43, Vector2(3, 4), 43),
	new Block_item("Lava - Walkable", 44, Vector2(4, 4), 44),
	new Block_item("Lava - Liquid", 45, Vector2(5, 4), 45),
	new Block_item("Lava - Solid", 46, Vector2(6, 4), 46),
	new Block_item("Stone", 47, Vector2(7, 4), 47),
	new Block_item("Stone", 48, Vector2(8, 4), 48),
	new Block_item("Stone", 49, Vector2(9, 4), 49),
};

Item* get_item(int index)
{
	if (index >= static_cast<int>(item_list.size()) || index < 0)
		return (nullptr);
	return (item_list[index]);
}