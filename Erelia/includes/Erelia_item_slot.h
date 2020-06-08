#ifndef ERELIA_ITEM_SLOT_H
#define ERELIA_ITEM_SLOT_H

#include "jgl.h"

#include "erelia_item.h"

class Item_slot : public jgl::Widget
{
protected:
	bool _state = false;
	static jgl::Vector2 _size;
	jgl::Vector2 _base_pos;
	jgl::Sprite_sheet* _icon_tile;
	Item* _item;

	jgl::w_box_component _box;
public:
	static jgl::Vector2 size() { return (_size); }

	Item_slot(jgl::Sprite_sheet* p_icon_tile, Item* p_item, jgl::Widget* p_parent = nullptr);
	void set_state(bool p_state) { _state = p_state; }
	void set_item(Item *p_item) { _item = p_item; }
	Item* item() { return (_item); }
	void render();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void select();
	void unselect();
	void move(Vector2 delta);
};

#endif