#ifndef ERELIA_ITEM_H
#define ERELIA_ITEM_H

#include "jgl.h"

enum class item_type
{
	error = -1,
	item = 0,
	block = 1
};

struct Item
{
	item_type type;
	int id;
	int icon;

	Item(int p_id = -1, int p_icon = -1);

	virtual void use(jgl::Data data = nullptr) = 0;
};

struct Block_item : public Item
{
	int block_id;

	Block_item(int p_id, int p_icon, int p_block_id);

	void use(jgl::Data data = nullptr);
};

extern jgl::Sprite_sheet* icon_tile;
extern Item* item_list[12];

#endif