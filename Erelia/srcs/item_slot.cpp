#include "erelia.h"

Item_slot::Item_slot(jgl::Sprite_sheet *p_icon_tile, Item* p_item, jgl::Widget *p_parent) : jgl::Widget(p_parent)
{
	_icon_tile = p_icon_tile;
	_item = p_item;
	_base_pos = -1;
	_box = jgl::w_box_component(this);
	_box.set_back(jgl::Color(90, 90, 90));
	_box.set_front(jgl::Color(120, 120, 120));
	_box.set_border(5);
}

void Item_slot::render()
{
	_box.render(_viewport);
	if (_item != nullptr && _item->id != -1 && _icon_tile != nullptr)
		_icon_tile->draw(_item->icon, _box.anchor() + 10, _box.area() - 20, 1.0f, _viewport);
}

void Item_slot::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_viewport->resize(p_anchor, p_area);
	_box.set_anchor(p_anchor);
	_box.set_area(p_area);
}

void Item_slot::select()
{
	if (_state == true)
		return;
	_box.set_anchor(_box.anchor() - 6);
	_box.set_area(_box.area() + 12);
	_box.set_back(jgl::Color(171, 163, 9));
	_box.set_front(jgl::Color(230, 221, 62));
	_box.set_border(10);
	_state = true;
}

void Item_slot::unselect()
{
	if (_state == false)
		return;
	_box.set_anchor(_box.anchor() + 6);
	_box.set_area(area());
	_box.set_back(jgl::Color(90, 90, 90));
	_box.set_front(jgl::Color(120, 120, 120));
	_box.set_border(5);
	_state = false;
}

void Item_slot::move(Vector2 delta)
{
	_box.set_anchor(_box.anchor() + delta);
	_viewport->set_anchor(anchor() + delta);
}