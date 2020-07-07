#ifndef ERELIA_ITEM_H
#define ERELIA_ITEM_H

#include "jgl.h"

#include "erelia_scenery.h"

enum class item_type
{
	error = -1,
	item = 0,
	block = 1,
	scenery = 2
};

struct Item
{
	item_type type;
	jgl::String name;
	int id;
	Vector2 icon;

	Item(jgl::String p_name = "unknow object", int p_id = -1, Vector2 p_icon = -1);

	virtual jgl::String str() { return ("Name : " + name + " / id : " + jgl::itoa(id)); }

	virtual void use(jgl::Data data = nullptr) = 0;
};

struct Block_item : public Item
{
	int block_id;

	Block_item(jgl::String p_name, int p_id, Vector2 p_icon, int p_block_id);

	void use(jgl::Data data = nullptr);
};

struct Scenery_item : public Item
{
	int scenery_id;

	Scenery_item(jgl::String p_name, int p_id, Vector2 p_icon, int p_scenery_id);
	void use(jgl::Data data = nullptr);
};

Item* get_item(int index);

extern jgl::Sprite_sheet* icon_tile;

#define NB_ITEM 256
#define START_BLOCK 0
#define NB_BLOCK 50
#define START_SCENERY 100
#define NB_SCENERY 20

extern Scenery* scenery_list[NB_SCENERY];

extern Item* item_list[NB_ITEM];

#endif