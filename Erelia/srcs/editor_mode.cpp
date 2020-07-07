#include "erelia.h"

void save_map(jgl::Data param)
{
	Editor_mode* editor = param.acces<Editor_mode *>(0);

	if (editor != nullptr)
	{
		editor->set_frozen(true);
		editor->echap_menu_frame()->desactivate();
		editor->editor_contener()->desactivate();
		editor->saver_menu()->activate();
		editor->saver_menu()->actualize_files();
	}
}

void load_map(jgl::Data param)
{
	Editor_mode* editor = param.acces<Editor_mode*>(0);

	if (editor != nullptr)
	{
		editor->set_frozen(true);
		editor->echap_menu_frame()->desactivate();
		editor->editor_contener()->desactivate();
		editor->loader_menu()->activate();
		editor->loader_menu()->actualize_files();
	}
}

void setting_edit(jgl::Data param)
{

}

void quit_editor(jgl::Data param)
{
	g_application->quit();
}

void save_quit_editor(jgl::Data param)
{
	save_map(param);
	quit_editor(param);
}

Editor_mode::Editor_mode(Game_engine* p_engine, World* p_world, Player *p_player) : Game_mode(p_engine)
{
	_player = p_player;
	_edited = false;
	_engine = p_engine;
	_world = p_world;
	_voxel_source = -1;
	_voxel_target = -1;
	_actual_tick = g_time;
	_timer = _actual_tick;
	_delta_time = 150;
	create_editor_panel();
	create_saver_panel();
	create_loader_panel();
	_visualizer = new Editor_visualizer(_controller, _controller);
	_visualizer->activate();
	_visualizer->send_front();
}

void Editor_mode::create_editor_panel()
{
	_editor_contener = new jgl::Contener(this);
	_editor_contener->activate();

	_minimap = new Minimap(_world, _player, _editor_contener);
	_minimap->activate();

	_controller = new Player_controller(_world, _player, _minimap->camera(), _editor_contener);
	_controller->activate();

	_editor_inventory = new Editor_inventory(_controller, _editor_contener);
	_editor_inventory->activate();

	_debug_screen = new Debug_screen(_editor_contener);
	_debug_screen->set_text_size(18);
	_debug_screen->add_line("FPS : " + jgl::itoa(print_fps));
	_debug_screen->add_line("Camera pos : " + _controller->camera()->pos().str());
	_debug_screen->add_line("Camera direction : " + _controller->camera()->direction().str());
	_debug_screen->add_line("Camera forward : " + _controller->camera()->forward().str());
	_debug_screen->add_line("Camera right : " + _controller->camera()->right().str());
	_debug_screen->add_line("Camera up : " + _controller->camera()->up().str());
	_debug_screen->activate();

	create_echap_panel();

	_controller->send_back();
	_minimap->send_front();
	_minimap->lower();
	_editor_inventory->send_front();
}

void Editor_mode::create_echap_panel()
{
	_echap_menu_frame = new jgl::Frame(this);
	_echap_menu_frame->send_front();

	_save_button = new jgl::Button(save_map, this, _echap_menu_frame);
	_save_button->set_text("Save the level");
	_save_button->activate();

	_load_button = new jgl::Button(load_map, this, _echap_menu_frame);
	_load_button->set_text("Load a level");
	_load_button->activate();

	_settings_button = new jgl::Button(setting_edit, nullptr, _echap_menu_frame);
	_settings_button->set_text("Settings");
	_settings_button->activate();

	_save_quit_button = new jgl::Button(save_quit_editor, this, _echap_menu_frame);
	_save_quit_button->set_text("Save the level and quit");
	_save_quit_button->activate();

	_quit_button = new jgl::Button(quit_editor, nullptr, _echap_menu_frame);
	_quit_button->set_text("Quit");
	_quit_button->activate();
}

bool Editor_mode::voxel_raycast(Vector3 pos, Vector3 direction, Vector3* voxel_source, Vector3* voxel_target)
{
	bool found = false;
	Vector3 delta = direction / 100.0f;
	Vector3 actual = pos;
	Vector3 old = -1;

	if (_world == nullptr)
		jgl::error_exit(1, "Error in voxel raycasting : world nullptr");

	while (found == false && actual.y >= 0.0f && pos.distance(actual) <= 100.0f)
	{
		actual += delta;
		if (actual.floor() != old)
		{
			old = actual.floor();
			Voxel* tmp = _world->voxels(actual);
			if (tmp != nullptr && tmp->type() != AIR_BLOCK)
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
		if (actual.y <= 0.0f)
		{
			actual -= delta;
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


void Editor_mode::use_item_left(Vector3 A, Vector3 B, Item* item)
{
	std::vector<jgl::Vector3> chunk_pos_list;

	Vector3 start = Vector3(min(A.x, B.x), min(A.y, B.y), min(A.z, B.z));
	Vector3 end = Vector3(max(A.x, B.x), max(A.y, B.y), max(A.z, B.z));

	for (int i = static_cast<int>(start.x); i <= static_cast<int>(end.x); i++)
		for (int j = static_cast<int>(start.y); j <= static_cast<int>(end.y); j++)
			for (int k = static_cast<int>(start.z); k <= static_cast<int>(end.z); k++)
			{
				Vector3 voxel_pos = Vector3(i, j, k);
				Vector3 chunk_pos = _world->chunk_pos(voxel_pos);
				if (item->type == item_type::block)
				{
					if (std::find(chunk_pos_list.begin(), chunk_pos_list.end(), chunk_pos) == chunk_pos_list.end())
						chunk_pos_list.push_back(chunk_pos);
				}
				if (item->type != item_type::scenery || _world->voxels(voxel_pos)->type() != AIR_BLOCK)
					item->use(jgl::Data(2, _world, &voxel_pos));
			}

	if (item->type == item_type::block)
	{
		for (size_t i = 0; i < chunk_pos_list.size(); i++)
			_world->baking_chunk(chunk_pos_list[i]);
	}
}

void Editor_mode::use_item_right(Vector3 A, Vector3 B, Item* item)
{
	std::vector<jgl::Vector3> chunk_pos_list;

	Vector3 start = Vector3(min(A.x, B.x), min(A.y, B.y), min(A.z, B.z));
	Vector3 end = Vector3(max(A.x, B.x), max(A.y, B.y), max(A.z, B.z));

	for (int i = static_cast<int>(start.x); i <= static_cast<int>(end.x); i++)
		for (int j = static_cast<int>(start.y); j <= static_cast<int>(end.y); j++)
			for (int k = static_cast<int>(start.z); k <= static_cast<int>(end.z); k++)
			{
				Vector3 voxel_pos = Vector3(i, j, k);
				Voxel* tmp_voxel = _world->voxels(voxel_pos);
				if (tmp_voxel != nullptr)
				{
					Vector3 chunk_pos = _world->chunk_pos(voxel_pos);
					if (std::find(chunk_pos_list.begin(), chunk_pos_list.end(), chunk_pos) == chunk_pos_list.end())
						chunk_pos_list.push_back(chunk_pos);
					if (_world->voxels(voxel_pos)->type() > AIR_BLOCK)
						_world->set_block(voxel_pos, AIR_BLOCK);
					while (_world->voxels(voxel_pos)->type() == SCENERY_BLOCK)
						(voxel_pos.y)--;
					_world->place_scenery(voxel_pos, nullptr);
				}
			}

	for (size_t i = 0; i < chunk_pos_list.size(); i++)
		_world->baking_chunk(chunk_pos_list[i]);
}

void Editor_mode::update()
{
	static Vector3 old_value[5] = {
		-1,
		-1,
		-1,
		-1,
		-1
	};
	_actual_tick = g_time;
	if (_edited == true && _timer < _actual_tick)
		_edited = false;
	_debug_screen->set_text(0, "FPS : " + jgl::itoa(print_fps));
	if (_controller->camera()->pos() != old_value[0])
	{
		_debug_screen->set_text(1, "Camera pos : " + _controller->camera()->pos().str());
		old_value[0] = _controller->camera()->pos();
	}
	if (_controller->camera()->direction() != old_value[1])
	{
		_debug_screen->set_text(2, "Camera direction : " + _controller->camera()->direction().str());
		old_value[1] = _controller->camera()->direction();
	}
	if (_controller->camera()->forward() != old_value[2])
	{
		_debug_screen->set_text(3, "Camera forward : " + _controller->camera()->forward().str());
		old_value[2] = _controller->camera()->forward();
	}
	if (_controller->camera()->right() != old_value[3])
	{
		_debug_screen->set_text(4, "Camera right : " + _controller->camera()->right().str());
		old_value[3] = _controller->camera()->right();
	}
	if (_controller->camera()->up() != old_value[4])
	{
		_debug_screen->set_text(5, "Camera up : " + _controller->camera()->up().str());
		old_value[4] = _controller->camera()->up();
	}
}

bool Editor_mode::handle_keyboard()
{
	if (_dodge == true)
	{
		_dodge = false;
		return (false);
	}
	if (_edited == true)
		return (false);
	if (_editor_inventory->toggle() == true)
		return (false);

	if (jgl::get_key(jgl::key::escape) == jgl::key_state::release)
	{
		_echap_menu_frame->set_active(!_echap_menu_frame->is_active());
		_editor_inventory->set_shortcut_frozen(_echap_menu_frame->is_active());
		_controller->set_frozen(_echap_menu_frame->is_active());
		g_mouse->place(g_application->size() / 2);
		g_mouse->actualize();
		return (true);
	}
	else if (jgl::get_key(jgl::key::F3) == jgl::key_state::release)
	{
		_debug_screen->set_active(!_debug_screen->is_active());
		return (true);
	}

	return (false);
}

bool Editor_mode::handle_mouse()
{
	Item* item = (_editor_inventory->selected_item() != nullptr ? _editor_inventory->selected_item()->item() : nullptr);

	if (_dodge == true)
	{
		_dodge = false;
		return (false);
	}
	if (_edited == true)
		return (false);
	if (_editor_inventory->is_frozen() == true)
		return (false);
	if (_editor_inventory->toggle() == true || _editor_inventory->shortcut_bar()->is_pointed() == true)
		return (false);
	if (_echap_menu_frame->is_active() == true)
		return (false);

	bool raycast_state = false;
	if (jgl::get_button(jgl::mouse_button::left) != jgl::mouse_state::up ||
		jgl::get_button(jgl::mouse_button::right) != jgl::mouse_state::up ||
		jgl::get_button(jgl::mouse_button::center) != jgl::mouse_state::up)
		raycast_state = voxel_raycast(_controller->camera()->pos(), _controller->camera()->mouse_direction(_controller->viewport()), &_voxel_source, &_voxel_target);
	if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::pressed && item != nullptr)
	{
		_visualizer->activate();
		if (item->type == item_type::block)
		{
			_first_voxel = _voxel_target;
			_second_voxel = _voxel_target;
		}
		else
		{
			_first_voxel = _voxel_source;
			_second_voxel = _voxel_source;
		}
		_visualizer->set_voxels(_first_voxel, _second_voxel);
		return (true);
		
	}
	else if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::down && item != nullptr)
	{
		_visualizer->activate();
		if (item->type == item_type::block)
			_second_voxel = _voxel_target;
		else
			_second_voxel = _voxel_source;
		_visualizer->set_voxels(_first_voxel, _second_voxel);
		return (true);
		
	}
	else if (jgl::get_button(jgl::mouse_button::left) == jgl::mouse_state::release && item != nullptr)
	{
		_visualizer->activate();
		if (_editor_inventory->selected_item() != nullptr)
		{
			if (item != nullptr)
				use_item_left(_first_voxel, _second_voxel, item);
		}
		_first_voxel = 0;
		_second_voxel = 0;
		return (true);
	}
	else if (jgl::get_button(jgl::mouse_button::center) == jgl::mouse_state::release)
	{
		_visualizer->activate();
		if (_world->voxels(_voxel_source) != nullptr &&
			_world->voxels(_voxel_source)->type() > AIR_BLOCK)
		{
			_editor_inventory->shortcut_bar()->selected_item()->set_item(item_list[_world->voxels(_voxel_source)->type()]);
		}
		return (true);
	}
	else if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::pressed)
	{
		_visualizer->activate();
		_first_voxel = _voxel_source;
		_second_voxel = _voxel_source;
		_visualizer->set_voxels(_first_voxel, _second_voxel);
		return (true);
		
	}
	else if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::down)
	{
		_visualizer->activate();
		_second_voxel = _voxel_source;
		_visualizer->set_voxels(_first_voxel, _second_voxel);
		return (true);
		
	}
	else if (jgl::get_button(jgl::mouse_button::right) == jgl::mouse_state::release)
	{
		_visualizer->activate();
		if (item != nullptr)
			use_item_right(_first_voxel, _second_voxel, item);
		else
			use_item_right(_first_voxel, _second_voxel, nullptr);
		_first_voxel = 0;
		_second_voxel = 0;
		return (true);
	}
	else
	{
		_visualizer->desactivate();
	}

	return (false);
}

void Editor_mode::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Editor_mode::set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
{
	_controller->set_geometry(0, p_area);
	_minimap->set_geometry(Vector2(p_area.x - p_area.x / 4, 0.0f), p_area / 4);
	_visualizer->set_geometry(0, p_area);
	_editor_inventory->set_geometry(0, p_area);
	_echap_menu_frame->set_geometry(p_area / 4 - 10, p_area / 2 + 20);
	Vector2 size = Vector2(_echap_menu_frame->area().x - 20, 40.0f);
	Vector2 pos = 10;
	_save_button->set_geometry(pos, size);
	pos.y += size.y + 10;
	_load_button->set_geometry(pos, size);
	pos.y += size.y + 10;
	_settings_button->set_geometry(pos, size);
	pos.y = _echap_menu_frame->area().y - (10 + size.y) * 2;
	_save_quit_button->set_geometry(pos, size);
	pos.y += size.y + 10;
	_quit_button->set_geometry(pos, size);
	_debug_screen->set_geometry(0, p_area);

	_saver_menu->set_geometry(0, p_area);
	_loader_menu->set_geometry(0, p_area);
}