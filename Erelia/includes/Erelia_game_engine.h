#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

#include "erelia_board.h"
#include "erelia_editor_mode.h"

class Game_engine : public jgl::Widget
{
private:
	Board* _board;

	size_t _active_mode;
	Game_mode *_modes[1];

public:
	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();

	Board* board() { return (_board); }

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
};

void create_scenery_list();
void delete_scenery_list();
void create_item_list();
void delete_item_list();

#endif