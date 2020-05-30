#ifndef ERELIA_GAME_MODE_H
#define ERELIA_GAME_MODE_H

#include "jgl.h"
#include "erelia_board.h"

class Game_mode
{
protected:
	class Game_engine* _engine;
	jgl::Contener* _contener;

public:
	Game_mode(class Game_engine* p_engine);
	
	void set_active(bool p_state) { _contener->set_active(p_state); }
	void activate() { _contener->activate(); }
	void desactivate() { _contener->desactivate(); }
	bool is_active() { return (_contener->is_active()); }

	virtual void update() = 0;
	virtual bool handle_keyboard() = 0;
	virtual bool handle_mouse() = 0;
	virtual void render() = 0;
	virtual void set_geometry_imp(Vector2 p_anchor, Vector2 p_area) = 0;
};

#endif