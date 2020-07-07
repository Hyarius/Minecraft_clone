#ifndef ERELIA_GAME_MODE_H
#define ERELIA_GAME_MODE_H

#include "jgl.h"
#include "erelia_world.h"

class Game_mode : public jgl::Widget
{
protected:
	class Game_engine* _engine;

public:
	Game_mode(class Game_engine* p_engine);

	virtual void update() = 0;
	virtual bool handle_keyboard() = 0;
	virtual bool handle_mouse() = 0;
	virtual void render() = 0;
	virtual void set_geometry_imp(Vector2 p_anchor, Vector2 p_area) = 0;
};

#endif