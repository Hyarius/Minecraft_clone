#ifndef ERELIA_SHORTCUT_BAR_H
#define ERELIA_SHORTCUT_BAR_H

#include "jgl.h"
#include "erelia_item_slot.h"

class Shortcut_bar : public jgl::Widget
{
private:
	Item_slot* _selected_item = nullptr;
	jgl::Frame* _shortcut_frame = nullptr;
	Item_slot* _item_slots[9] = {
		nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr
	};

public:
	Shortcut_bar(jgl::Widget* p_parent = nullptr);
	Item_slot* selected_item() { return (_selected_item); }
	void select_itemslot(size_t index);
	void set_item(size_t index, Item* p_item);
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

#endif