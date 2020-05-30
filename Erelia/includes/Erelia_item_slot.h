#ifndef ERELIA_ITEM_SLOT_H
#define ERELIA_ITEM_SLOT_H

#include "jgl.h"

#include "erelia_item.h"

class Item_slot : public jgl::Widget
{
protected:
	jgl::Sprite_sheet* _icon_tile;
	Item* _item;

	jgl::w_box_component _box;
public:
	Item_slot(jgl::Sprite_sheet *p_icon_tile, Item* p_item, jgl::Widget *p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_icon_tile = p_icon_tile;
		_item = p_item;
		_box = jgl::w_box_component(this);
		_box.set_back(jgl::Color(90, 90, 90));
		_box.set_front(jgl::Color(120, 120, 120));
		_box.set_border(5);
	}
	void set_item(Item *p_item)
	{
		_item = p_item;
	}
	Item* item()
	{
		return (_item);
	}
	void render()
	{
		_box.render(_viewport);
		if (_item != nullptr && _item->id != -1 && _icon_tile != nullptr)
			_icon_tile->draw(_item->icon, _box.anchor() + 10, _box.area() - 20);
	}
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
	{
		_box.set_anchor(p_anchor);
		_box.set_area(p_area);
	}
	void select()
	{
		_box.set_back(jgl::Color(171, 163, 9));
		_box.set_front(jgl::Color(230, 221, 62));
		_box.set_border(10);
	}
	void unselect()
	{
		_box.set_back(jgl::Color(90, 90, 90));
		_box.set_front(jgl::Color(120, 120, 120));
		_box.set_border(5);
	}
};

#endif