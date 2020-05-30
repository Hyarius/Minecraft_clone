#ifndef ERELIA_SHORTCUT_BAR_H
#define ERELIA_SHORTCUT_BAR_H

#include "jgl.h"
#include "erelia_item_slot.h"

class Shortcut_bar : public jgl::Widget
{
private:
	bool _disable = false;
	Item_slot* _selected_item = nullptr;
	jgl::Frame* _shortcut_frame = nullptr;
	std::vector<Item_slot*> _item_slots;

public:
	Shortcut_bar(jgl::Widget* p_parent = nullptr);
	Item_slot* selected_item() { return (_selected_item); }
	std::vector<Item_slot*>& item_slot() { return (_item_slots); }
	Item_slot* item_slot(size_t index) { if (index >= 9)return (nullptr); return (_item_slots[index]); }
	void select_itemslot(size_t index);
	void set_item(size_t index, Item* p_item);
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
	void enable() { _disable = false; if (_selected_item != nullptr)_selected_item->select(); }
	void disable() { _disable = true; if (_selected_item != nullptr)_selected_item->unselect();	}
};

#endif