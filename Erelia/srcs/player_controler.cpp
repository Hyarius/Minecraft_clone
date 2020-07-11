#include "erelia.h"

Player_controller::Player_controller(World* p_world, Player* p_player, jgl::Camera* p_minimap_cam, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_world = p_world;
	_minimap_cam = p_minimap_cam;
	_player = p_player;
	_camera = new jgl::Camera(0, 45, g_application->size().x / g_application->size().y);

	_camera->place(p_player->pos() + Vector3(0, 1, 0));
	_camera_target = _camera->pos() + Vector3(1, 0, 0);
	_camera->look_at(_camera_target);
	_camera->set_light_direction(Vector3(-0.3f, -1.0f, -0.5f).normalize());
	_camera->set_light_position(Vector3(0, 100, 0));
	g_mouse->place(g_application->size() / 2);
	g_mouse->actualize();
}

Player_controller::~Player_controller()
{
	if (_camera != nullptr)
		delete _camera;
}

void Player_controller::move_camera(Vector3 delta)
{
	_camera->move(delta);
	_camera_target += delta;
	_player->body()->move(delta);
	_minimap_cam->move(delta);
}

void Player_controller::rotate_camera(float yaw, float pitch)
{
	if (_camera->pitch() >= -87.0f - pitch && pitch > 0 ||
		_camera->pitch() <= 87.0f - pitch && pitch < 0)
	{
		_camera_target = jgl::rotate_point(_camera_target, _camera->pos(), _camera->right(), pitch);
	}
	_camera_target = jgl::rotate_point(_camera_target, _camera->pos(), Vector3::up(), yaw);
	_player->body()->rotate(Vector3(0.0f, -yaw, 0.0f));
	_camera->look_at(_camera_target);
}

void Player_controller::update()
{
	_world->update();
}

bool Player_controller::handle_keyboard()
{
	Vector3 delta;

	if (jgl::get_key(jgl::key::w) == jgl::key_state::down)
	{
		delta = (_camera->forward() * Vector3(-1, 0, -1)).normalize() * _player->move_speed();
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * _player->move_speed();
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::s) == jgl::key_state::down)
	{
		delta = (_camera->forward() * Vector3(1, 0, 1)).normalize() * _player->move_speed();
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * _player->move_speed();
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::a) == jgl::key_state::down)
	{
		delta = (_camera->right() * Vector3(-1, 0, -1)).normalize() * _player->move_speed();
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::d) == jgl::key_state::down)
	{
		delta = (_camera->right() * Vector3(1, 0, 1)).normalize() * _player->move_speed();
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::down)
	{
		if (_player->fly_mode() == true)
			delta = Vector3(0, 1, 0) * _player->move_speed();
		else
			delta = 0;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::left_shift) == jgl::key_state::down)
	{
		if (_player->fly_mode() == true)
			delta = Vector3(0, -1, 0) * _player->move_speed();
		else
			delta = 0;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::F1) == jgl::key_state::release)
	{
		_player->set_fly_mode(!_player->fly_mode());
	}
	return (false);
}

bool Player_controller::handle_mouse()
{
	rotate_camera(-g_mouse->rel_pos.x * _player->rot_speed(), -g_mouse->rel_pos.y * _player->rot_speed());
	g_mouse->place(_viewport->area() / 2 + anchor());
	return (false);
}

void Player_controller::render()
{
	_world->render(_camera, _player, _viewport);
	_player->body()->render(_camera, _viewport);
}

void Player_controller::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{

}