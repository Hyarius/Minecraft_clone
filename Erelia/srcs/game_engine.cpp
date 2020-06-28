#include "erelia.h"

Game_engine::Game_engine(jgl::Widget * p_parent) : jgl::Widget(p_parent)
{
	create_item_list();
	if (icon_tile == nullptr)
		icon_tile = new jgl::Sprite_sheet("ressources/texture/icon_tileset.png", Vector2(10, 13));
	_board = new Board();
	_active_mode = 0;
	_modes[0] = new Editor_mode(this, _board);
	_modes[0]->activate();
}

Game_engine::~Game_engine()
{
	delete_item_list();
	if (icon_tile != nullptr)
		delete icon_tile;
	delete _board;
	delete _modes[0];
}

void Game_engine::update()
{
	if (_modes[_active_mode] != nullptr && _modes[_active_mode]->is_active() == true)
		_modes[_active_mode]->update();
}	
bool Game_engine::handle_keyboard()
{
	if (_modes[_active_mode] != nullptr && _modes[_active_mode]->is_active() == true)
		return (_modes[_active_mode]->handle_keyboard());
	else
	{
		return (false);
	}
}	
bool Game_engine::handle_mouse()
{
	if (_modes[_active_mode] != nullptr && _modes[_active_mode]->is_active() == true)
		return (_modes[_active_mode]->handle_mouse());
	else
		return (false);
}	
void Game_engine::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);
	if (_modes[_active_mode] != nullptr && _modes[_active_mode]->is_active() == true)
		_modes[_active_mode]->render();
}	
void Game_engine::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{
	if (_modes[_active_mode] != nullptr)
		_modes[_active_mode]->set_geometry_imp(p_anchor, p_area);
}	