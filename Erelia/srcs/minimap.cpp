#include "erelia.h"

Minimap::Minimap(World* p_world, Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_world = p_world;
	_player = p_player;
	_camera = new jgl::Camera(_player->pos() + jgl::Vector3(0, 10, 0), 45, g_application->size().x / g_application->size().y);
	_camera->look_at(_player->pos());
}

Minimap::~Minimap()
{
	if (_camera != nullptr)
		delete _camera;
}

void Minimap::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{

}

void Minimap::render()
{
	fill_rectangle(0, _viewport->area(), jgl::Color(60, 60, 60), _viewport);
	glClear(GL_DEPTH_BUFFER_BIT);
	Vector3 tmp = _player->pos() + Vector3::up();
	while (_world->voxels(tmp) != nullptr && _world->voxels(tmp)->type() <= AIR_BLOCK)
		tmp.y++;
	_world->render(_camera, _player, 20, _viewport);
	//static_cast<int>(tmp.y - 1)
	_player->body()->render(_camera, _viewport);
}