#include "erelia.h"

Game_engine::Game_engine(jgl::Widget * p_parent) : jgl::Widget(p_parent)
{
	create_item_list();
	if (icon_tile == nullptr)
		icon_tile = new jgl::Sprite_sheet("ressources/texture/icon_tileset.png", Vector2(10, 13));
	_board = new World();
	_player = new Player(chunk_size / 2 + jgl::Vector3::up() * 2);
	_active_mode = 0;
	_modes[0] = new Editor_mode(this, _board, _player);
	_modes[0]->activate();
}

Game_engine::~Game_engine()
{
	delete_item_list();
	if (icon_tile != nullptr)
		delete icon_tile;
	if (_board != nullptr)
		delete _board;
	if (_player != nullptr)
		delete _player;
}

void Game_engine::update()
{
}	
bool Game_engine::handle_keyboard()
{
	return (false);
}	
bool Game_engine::handle_mouse()
{
	return (false);
}	
void Game_engine::render()
{

}	
void Game_engine::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{
	if (_modes[_active_mode] != nullptr)
		_modes[_active_mode]->set_geometry_imp(p_anchor, p_area);
}	