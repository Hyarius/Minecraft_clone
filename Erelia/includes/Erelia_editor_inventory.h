#ifndef ERELIE_EDITOR_INVENTORY_H
#define ERELIE_EDITOR_INVENTORY_H

#include "erelia_shortcut_bar.h"
#include "erelia_editor_inventory_tab.h"
#include "erelia_icon_holder.h"

enum class Tab_type
{
	bloc = 0,
	decor = 1,
	count
};

class Editor_inventory : public jgl::Widget
{
protected:
	size_t _selected_tab_index;
	Item_slot* _clicked_slot;

	jgl::Contener* _contener;
	Icon_holder* _icon_holder;
	Shortcut_bar* _shortcut_bar;
	jgl::Frame* _inventory_frame;
	std::vector<Editor_inventory_tab*> _item_library;
	std::vector<jgl::Button*> _library_button;

public:
	Editor_inventory(jgl::Widget* parent);

	Shortcut_bar* shortcut_bar() { return (_shortcut_bar); }
	Item_slot* clicked_slot() { return (_clicked_slot); }
	Item_slot* selected_item() { return (_shortcut_bar->selected_item()); }
	std::vector<Editor_inventory_tab*>& item_library() { return (_item_library); }
	Editor_inventory_tab* item_library(size_t index) { if (index >= static_cast<size_t>(Tab_type::count)) return (nullptr); return (_item_library[index]); }

	void select_tab(size_t index);

	Item_slot* find_shortcut_slot();
	Item_slot* find_inventory_slot();
	Item_slot* find_slot() { Item_slot* tmp = find_shortcut_slot(); if (tmp == nullptr)tmp = find_inventory_slot(); return (tmp); }

	void disable() { _inventory_frame->desactivate(); _shortcut_bar->set_frozen(false); _item_library[_selected_tab_index]->reset(); }
	void enable() { _inventory_frame->activate(); _shortcut_bar->set_frozen(true); _item_library[_selected_tab_index]->reset(); }
	bool is_disable() { return (_inventory_frame->is_active()); }
	bool is_enable() { return (!_inventory_frame->is_active()); }

	bool is_shortcut_slot(Item_slot* slot);
	
	void set_shortcut_frozen(bool p_state) { _shortcut_bar->set_frozen(p_state); }
	bool is_shortcut_frozen() { return (_shortcut_bar->is_frozen()); }
	bool toggle();
	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
	void render();
};

#endif