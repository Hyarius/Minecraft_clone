#include "erelia.h"

Editor_mode::Editor_mode(Game_engine* p_engine, Board* p_board) : Game_mode(p_engine)
{
	_board = p_board;
	_camera = new jgl::Camera(0, 45, g_application->size().x / g_application->size().y);
	_actual_tick = SDL_GetTicks();
	_timer = _actual_tick;
	_delta_time = 150;

	_to_place = 0;
	_voxel_source = -1;
	_voxel_target = -1;

	_camera_target = chunk_size / 2 * Vector3(1, 0, 1) + Vector3::up();
	_camera->place(chunk_size);
	_camera->look_at(_camera_target);
	_camera->set_light_direction(Vector3(-0.3f, -1.0f, -0.5f).normalize());
	_camera->set_light_position(Vector3(0, 100, 0));

	_editor_inventory = new Editor_inventory(_contener);

	_debug_screen = new Debug_screen(_contener);
	_debug_screen->set_text_size(18);
	_debug_screen->add_line("FPS : " + jgl::itoa(print_fps));
	_debug_screen->add_line("Camera pos : " + _camera->pos().str());
	_debug_screen->add_line("Camera direction : " + _camera->direction().str());
	_debug_screen->add_line("Camera forward : " + _camera->forward().str());
	_debug_screen->add_line("Camera right : " + _camera->right().str());
	_debug_screen->add_line("Camera up : " + _camera->up().str());
	_debug_screen->activate();
	_save_button = new jgl::Button(nullptr, nullptr, _contener);
	_save_button->set_text("Save");
	_save_button->activate();
	_load_button = new jgl::Button(nullptr, nullptr, _contener);
	_load_button->set_text("Load");
	_load_button->activate();
}

bool Editor_mode::voxel_raycast(Vector3 pos, Vector3 direction, Vector3* voxel_source, Vector3* voxel_target)
{
	bool found = false;
	Vector3 delta = direction / 100.0f;
	Vector3 actual = pos;
	Vector3 old = -1;

	while (found == false && actual.y >= 0.0f && pos.distance(actual) <= 100.0f)
	{
		actual += delta;
		if (actual.floor() != old)
		{
			old = actual.floor();
			Voxel* tmp = _board->voxels(actual);
			if (tmp != nullptr && tmp->type() >= 0)
				found = true;
		}
	}

	if (found == true)
	{
		*voxel_source = actual.floor();
		*voxel_target = (actual - delta).floor();
		return (true);
	}
	else
	{
		actual -= delta;
		if (_board->voxels(actual) != nullptr)
		{
			*voxel_source = actual.floor();
			*voxel_target = (actual - delta).floor();
			return (true);
		}
		else
		{
			*voxel_source = -1;
			*voxel_target = -1;
			return (false);
		}
	}
}

void Editor_mode::move_camera(Vector3 delta)
{
	_camera->move(delta);
	_camera_target += delta;
	_debug_screen->set_text(1, "Camera pos : " + _camera->pos().str());
}

void Editor_mode::rotate_camera(int yaw, int pitch)
{
	if (_camera->pitch() >= -88.0f + pitch && pitch > 0 ||
		_camera->pitch() <= -0.0f + pitch && pitch < 0)
		_camera->rotate_around_point(_camera_target, -static_cast<float>(pitch), _camera->right());
	_camera->rotate_around_point(_camera_target, -static_cast<float>(yaw), Vector3::up());
	_camera->look_at(_camera_target);
	_debug_screen->set_text(1, "Camera pos : " + _camera->pos().str());
	_debug_screen->set_text(2, "Camera direction : " + _camera->direction().str());
	_debug_screen->set_text(3, "Camera forward : " + _camera->forward().str());
	_debug_screen->set_text(4, "Camera right : " + _camera->right().str());
	_debug_screen->set_text(5, "Camera up : " + _camera->up().str());
}

void Editor_mode::update()
{
	_debug_screen->set_text(0, "FPS : " + jgl::itoa(print_fps));

	_actual_tick = g_application->time();
	_board->update();
	_editor_inventory->render();
}
bool Editor_mode::handle_keyboard()
{
	if (_editor_inventory->handle_keyboard())
		return (true);
	if (jgl::get_key(jgl::key::escape) == jgl::key_state::release)
	{
		if (_editor_inventory->toggle() == true)
			_editor_inventory->desactivate();
		else
			g_application->quit();
	}
	if (jgl::get_key(jgl::key::F3) == jgl::key_state::release)
		_debug_screen->set_active(!_debug_screen->is_active());

	if (_editor_inventory->toggle() == true)
		return false;

	if (jgl::get_key(jgl::key::w) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->forward() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::s) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->forward() * Vector3(1, 0, 1)).normalize() * 0.3f;
		if (delta == 0)
			delta = (_camera->up() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::a) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->right() * Vector3(-1, 0, -1)).normalize() * 0.3f;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::d) == jgl::key_state::down)
	{
		Vector3 delta = (_camera->right() * Vector3(1, 0, 1)).normalize() * 0.3f;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::space) == jgl::key_state::down)
	{
		Vector3 delta = Vector3(0, 1, 0) * 0.3f;
		move_camera(delta);
	}
	if (jgl::get_key(jgl::key::left_shift) == jgl::key_state::down && _camera_target.y >= 0.3f)
	{
		Vector3 delta = Vector3(0, 1, 0) * -0.3f;
		move_camera(delta);
	}
	return (false);
}
bool Editor_mode::handle_mouse()
{
	if (_editor_inventory->handle_mouse() == true)
		return (true);
	if (_editor_inventory->toggle() == true || _editor_inventory->shortcut_bar()->is_pointed() == true)
		return false;

	if (jgl::get_button(jgl::mouse_button::middle) == jgl::mouse_state::down)
	{
		rotate_camera(static_cast<int>(g_mouse->rel_pos.x), static_cast<int>(g_mouse->rel_pos.y));
	}
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::down && _actual_tick >= _timer && 
		_editor_inventory->selected_item() != nullptr && _editor_inventory->selected_item()->item() != nullptr)
	{
		if (jgl::get_key(jgl::key::left_control) != jgl::key_state::down)
		{
			if (_board->chunks().size() == 0)
			{
				_voxel_target = 0;
				jgl::Data param = jgl::Data(2, _board, &_voxel_target);
				_editor_inventory->selected_item()->item()->use(param);
			}
			else if (voxel_raycast(_camera->pos(), _camera->mouse_direction(), &_voxel_source, &_voxel_target) == true)
			{
				jgl::Data param = jgl::Data(2, _board, &_voxel_target);
				_editor_inventory->selected_item()->item()->use(param);
				_timer = _actual_tick + _delta_time;
			}
		}
		else
		{
			if (voxel_raycast(_camera->pos(), _camera->mouse_direction(), &_voxel_source, &_voxel_target) == true)
			{
				Vector3 chunk_pos = _board->chunk_pos(_voxel_source);
				_board->remove_chunk(chunk_pos);
				_timer = _actual_tick + _delta_time;
			}
		}
	}
	if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::down && _actual_tick >= _timer)
	{
		if (voxel_raycast(_camera->pos(), _camera->mouse_direction(), &_voxel_source, &_voxel_target) == true)
		{
			voxel_raycast(_camera->pos(), _camera->mouse_direction(), &_voxel_source, &_voxel_target);
			_board->remove_block(_voxel_source);
			_timer = _actual_tick + _delta_time;
		}
	}
	if (g_mouse->wheel != 0)
	{
		if (jgl::get_key(jgl::key::left_control) == jgl::key_state::down)
			_camera->move(_camera->forward() * -g_mouse->wheel);
	}

	return (false);
}
void Editor_mode::render()
{
	_engine->viewport()->use();
	_board->render(_camera);
}
void Editor_mode::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{
	_editor_inventory->set_geometry(p_anchor, p_area);
	Vector2 button_size = Vector2(100, 30);
	Vector2 button_pos = 0;
	_save_button->set_geometry(button_pos, button_size);
	button_pos.x += button_size.x + 2;
	_load_button->set_geometry(button_pos, button_size);
	_debug_screen->set_geometry(p_anchor + Vector2(0, 40), p_area);
}