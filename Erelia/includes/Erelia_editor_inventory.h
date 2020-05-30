#ifndef ERELIE_EDITOR_INVENTORY_H
#define ERELIE_EDITOR_INVENTORY_H

#include "erelia_shortcut_bar.h"
#include "erelia_editor_inventory_tab.h"

enum class Tab_type
{
	bloc = 0,
	decor = 1,
	count
};

class Editor_inventory
{
protected:
	size_t _selected_tab_index;

	Item_slot* _clicked_slot;
	Shortcut_bar* _shortcut_bar;
	jgl::Frame* _inventory_frame;
	std::vector<Editor_inventory_tab*> _item_library;
	std::vector<jgl::Button*> _library_button;

public:
	Editor_inventory(jgl::Widget* parent);
	Shortcut_bar* shortcut_bar() { return (_shortcut_bar); }
	std::vector<Editor_inventory_tab*>& item_library() { return (_item_library); }
	Editor_inventory_tab* item_library(size_t index) { if (index >= static_cast<size_t>(Tab_type::count)) return (nullptr); return (_item_library[index]); }
	void select_tab(size_t index);
	Item_slot* clicked_slot() { return (_clicked_slot); }
	Item_slot* selected_item() { return (_shortcut_bar->selected_item()); }
	Item_slot* find_shortcut_slot();
	Item_slot* find_inventory_slot();
	Item_slot* find_slot() { Item_slot* tmp = find_shortcut_slot(); if (tmp == nullptr)tmp = find_inventory_slot(); return (tmp); }
	bool is_shortcut_slot(Item_slot* slot);
	bool toggle();
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry(Vector2 p_anchor, Vector2 p_area);
	void render();
	void activate();
	void desactivate();
};

#endif