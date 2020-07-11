#ifndef ERELIA_GAME_ENGINE_H
#define ERELIA_GAME_ENGINE_H

#include "erelia_player.h"
#include "erelia_world.h"
#include "erelia_editor_mode.h"

class Game_engine : public jgl::Widget
{
private:
	World* _board = nullptr;
	Player* _player = nullptr;

	size_t _active_mode = 0;
	Game_mode* _modes[1] = {nullptr};

	jgl::Engine _engine;

public:
	Game_engine(jgl::Widget* p_parent = nullptr);
	~Game_engine();

	World* world() { return (_board); }

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
};

extern Game_engine* g_engine;

void create_scenery(size_t index);
void create_scenery_list();
void delete_scenery_list();
void create_item_list();
void delete_item_list();

#endif