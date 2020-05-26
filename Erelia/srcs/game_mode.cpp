#include "erelia.h"

Game_mode::Game_mode(Game_engine* p_engine, Board* p_board)
{
	_engine = p_engine;
	_board = p_board;
	_camera = new jgl::Camera(0, 45, g_application->size().x / g_application->size().y);
}