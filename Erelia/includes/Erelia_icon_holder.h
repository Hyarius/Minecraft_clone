#ifndef ERELIA_ICON_HOLDER_H
#define ERELIA_ICON_HOLDER_H

#include "jgl.h"
#include "erelia_item_slot.h"

class Icon_holder : public jgl::Widget
{
protected:
	jgl::Sprite_sheet* _tile;
	Item* _item;
public:
	Icon_holder(jgl::Sprite_sheet *p_tile, jgl::Widget* p_parent)
	{
		_tile = p_tile;
		_item = nullptr;
	}
	void set_item(Item* p_item) { _item = p_item; }
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area) {}
	void render()
	{
		if (_item != nullptr)
			_tile->draw_centred(_item->icon, g_mouse->pos, Item_slot::size() - 20);
	}
};

#endif