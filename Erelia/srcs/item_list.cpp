#include "Erelia.h"

jgl::String block_name_list[NB_BLOCK] =
{
	"Orange tile",
	"Grey tile",
	"Red tile",
	"Brown tile",
	"Tiled stone",
	"Dark tiled stone",
	"Sand",
	"Grass",
	"Dirt",
	"Cave dirt",
	"Black tile",
	"Glass",
	"Blue glass",
	"Grey glass",
	"Blue tiled stone",
	"Green tiled stone",
	"Sand",
	"Grass",
	"Dirt",
	"Cave dirt",
	"Plank - Normal",
	"Plank - Light",
	"Plank - Dark",
	"Plank - Purple",
	"Pink tiled stone",
	"Red tiled stone",
	"Sand",
	"Grass",
	"Dirt",
	"Cave dirt",
	"Leaves - Normal",
	"Leaves - Orange",
	"Leaves - Dark",
	"Leaves - Purple",
	"Water",
	"Water",
	"Swamp water",
	"Grass",
	"Dirt",
	"Cave dirt",
	"Wood - Normal",
	"Wood - Light",
	"Wood - Dark",
	"Wood - Purple",
	"Lava - Walkable",
	"Lava - Liquid",
	"Lava - Solid",
	"Stone",
	"Stone",
	"Stone",
};

int block_id_list[NB_BLOCK] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
};

jgl::Vector2 block_icon_list[NB_BLOCK] = {
	Vector2(0, 0),
	Vector2(1, 0),
	Vector2(2, 0),
	Vector2(3, 0),
	Vector2(4, 0),
	Vector2(5, 0),
	Vector2(6, 0),
	Vector2(7, 0),
	Vector2(8, 0),
	Vector2(9, 0),
	Vector2(0, 1),
	Vector2(1, 1),
	Vector2(2, 1),
	Vector2(3, 1),
	Vector2(4, 1),
	Vector2(5, 1),
	Vector2(6, 1),
	Vector2(7, 1),
	Vector2(8, 1),
	Vector2(9, 1),
	Vector2(0, 2),
	Vector2(1, 2),
	Vector2(2, 2),
	Vector2(3, 2),
	Vector2(4, 2),
	Vector2(5, 2),
	Vector2(6, 2),
	Vector2(7, 2),
	Vector2(8, 2),
	Vector2(9, 2),
	Vector2(0, 3),
	Vector2(1, 3),
	Vector2(2, 3),
	Vector2(3, 3),
	Vector2(4, 3),
	Vector2(5, 3),
	Vector2(6, 3),
	Vector2(7, 3),
	Vector2(8, 3),
	Vector2(9, 3),
	Vector2(0, 4),
	Vector2(1, 4),
	Vector2(2, 4),
	Vector2(3, 4),
	Vector2(4, 4),
	Vector2(5, 4),
	Vector2(6, 4),
	Vector2(7, 4),
	Vector2(8, 4),
	Vector2(9, 4),
};

Item* item_list[256];

extern jgl::String scenery_path_list[NB_SCENERY];
extern jgl::Vector3 scenery_delta_pos[NB_SCENERY];
extern jgl::Vector3 scenery_start_size[NB_SCENERY];

jgl::Vector2 scenery_icon_list[NB_SCENERY] = {
	Vector2(0, 5),
	Vector2(1, 5),
	Vector2(2, 5),
	Vector2(3, 5),
	Vector2(4, 5),
	Vector2(5, 5),
	Vector2(6, 5),
	Vector2(7, 5),
	Vector2(8, 5),
	Vector2(9, 5),
	Vector2(0, 6),
	Vector2(1, 6),
	Vector2(2, 6),
	Vector2(3, 6),
	Vector2(4, 6),
	Vector2(5, 6),
	Vector2(6, 6),
	Vector2(7, 6),
	Vector2(8, 6),
	Vector2(9, 6),
};

jgl::String scenery_name_list[NB_SCENERY] = {
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Forest",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
	"Grass - Wasteland",
};

void create_item_list()
{
	create_scenery_list();
	int id = 0;

	for (int i = 0; i < NB_BLOCK; i++)
	{
		item_list[id] = new Block_item(block_name_list[i], id, block_icon_list[i], id);
		id++;
	}
	for (int i = 0; i < NB_SCENERY; i++)
	{
		item_list[id] = new Scenery_item(scenery_name_list[i], id, scenery_icon_list[i], i);
		id++;
	}
}

void delete_item_list()
{
	delete_scenery_list();

	for (int i = 0; i < NB_BLOCK + NB_SCENERY; i++)
		delete item_list[i];
}

Item* get_item(int index)
{
	if (index >= NB_ITEM || index < 0)
		return (nullptr);
	return (item_list[index]);
}