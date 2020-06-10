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
	jgl::String name;
	int id;
	Vector2 icon;

	Item(jgl::String p_name = "unknow object", int p_id = -1, Vector2 p_icon = -1);

	virtual void use(jgl::Data data = nullptr) = 0;
	virtual jgl::String str() = 0;
};

struct Block_item : public Item
{
	int block_id;

	Block_item(jgl::String p_name, int p_id, Vector2 p_icon, int p_block_id);

	void use(jgl::Data data = nullptr);
	jgl::String str() { return ("Name : " + name + " / id : " + jgl::itoa(id)); }
};

Item* get_item(int index);

extern jgl::Sprite_sheet* icon_tile;
extern std::vector<Item*> item_list;

#endif