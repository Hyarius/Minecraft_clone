#include "erelia.h"

Game_mode::Game_mode(Game_engine* p_engine)
{
	_engine = p_engine;
	_contener = new jgl::Contener(p_engine);
	_contener->activate();
}