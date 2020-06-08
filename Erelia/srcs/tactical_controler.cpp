#include "erelia.h"

Tactical_controller::Tactical_controller(Board* p_board, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board = p_board;
	_camera = new jgl::Camera(0, 45, g_application->size().x / g_application->size().y);

	_camera_target = chunk_size / 2 * Vector3(1, 0, 1) + Vector3::up();
	_camera->place(chunk_size);
	_camera->look_at(_camera_target);
	_camera->set_light_direction(Vector3(-0.3f, -1.0f, -0.5f).normalize());
	_camera->set_light_position(Vector3(0, 100, 0));
}

void Tactical_controller::move_camera(Vector3 delta)
{
	_camera->move(delta);
	_camera_target += delta;
}

void Tactical_controller::rotate_camera(int yaw, int pitch)
{
	if (_camera->pitch() >= -88.0f + pitch && pitch > 0 ||
		_camera->pitch() <= -0.0f + pitch && pitch < 0)
		_camera->rotate_around_point(_camera_target, -static_cast<float>(pitch), _camera->right());
	_camera->rotate_around_point(_camera_target, -static_cast<float>(yaw), Vector3::up());
	_camera->look_at(_camera_target);
}

void Tactical_controller::update()
{
	_board->update();
}

bool Tactical_controller::handle_keyboard()
{
	bool result = false;
	if (jgl::get_key(jgl::key::w) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->forward() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
		result = true;
	}
	if (jgl::get_key(jgl::key::s) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->forward() * Vector3(1, 0, 1)).normalize() * 0.3f;
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
		result = true;
	}
	if (jgl::get_key(jgl::key::a) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->right() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
		result = true;
	}
	if (jgl::get_key(jgl::key::d) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->right() * Vector3(1, 0, 1)).normalize() * 0.3f;
		move_camera(delta);
		result = true;
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::down)
	{
		Vector3 delta = Vector3(0, 1, 0) * 0.3f;
		move_camera(delta);
		result = true;
	}
	if (jgl::get_key(jgl::key::left_shift) == jgl::key_state::down && _camera_target.y >= 0.3f)
	{
		Vector3 delta = Vector3(0, 1, 0) * -0.3f;
		move_camera(delta);
		result = true;
	}
	return (result);
}
bool Tactical_controller::handle_mouse()
{
	if (jgl::get_button(jgl::mouse_button::middle) == jgl::mouse_state::down)
	{
		rotate_camera(static_cast<int>(g_mouse->rel_pos.x), static_cast<int>(g_mouse->rel_pos.y));
		return (true);
	}
	if (g_mouse->wheel != 0 && jgl::get_key(jgl::key::left_control) == jgl::key_state::down)
	{
		_camera->move(_camera->forward() * -g_mouse->wheel);
		return (true);
	}
	return (false);
}
void Tactical_controller::render()
{
	//std::cout << "Here" << std::endl;
	_board->render(_camera);
}
void Tactical_controller::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{

}