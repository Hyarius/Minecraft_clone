#ifndef ERELIE_EDITOR_INVENTORY_TAB_H
#define ERELIE_EDITOR_INVENTORY_TAB_H

#include "erelia_shortcut_bar.h"

extern size_t inventory_tab_size[];

class Editor_inventory_tab
{
protected:
	size_t _tab_index;
	size_t _nb_slot;
	jgl::Frame* _frame;
	jgl::Vscroll_bar* _scrollbar;
	std::vector<Item_slot*> _items_slot;
public:
	Editor_inventory_tab(size_t p_index, jgl::Widget* parent);
	void activate();
	void desactivate();
	void reset();
	void set_geometry(Vector2 p_anchor, Vector2 p_area);
	std::vector<Item_slot*>& item_slot() { return (_items_slot); }
	Item_slot* item_slot(size_t index) { if (index >= inventory_tab_size[_tab_index])return (nullptr); return (_items_slot[index]); }
};


#endif