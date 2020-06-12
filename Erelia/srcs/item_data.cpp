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