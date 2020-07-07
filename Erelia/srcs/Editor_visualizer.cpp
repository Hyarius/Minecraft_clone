#include "erelia.h"

Editor_visualizer::Editor_visualizer(Player_controller* p_controller, jgl::Widget* p_parent) : jgl::Widget(p_parent), _controller(p_controller)
{
	_pointer = jgl::primitive_cube(0, 0, 1.001f, jgl::Color(255, 255, 255));
	_pointer->set_transparency(0.5f);
	_first_voxel = 0;
	_second_voxel = 0;
}

void Editor_visualizer::set_voxels(Vector3 p_first, Vector3 p_second)
{
	if (p_first != _first_voxel || p_second != _second_voxel)
	{
		_first_voxel = Vector3(min(p_first.x, p_second.x), min(p_first.y, p_second.y), min(p_first.z, p_second.z));
		_second_voxel = Vector3(max(p_first.x, p_second.x), max(p_first.y, p_second.y), max(p_first.z, p_second.z));
		recalculate();
	}
}
void Editor_visualizer::recalculate()
{
	_pointer->resize((_second_voxel - _first_voxel) + 1);
}

void Editor_visualizer::set_geometry_imp(Vector2 p_anchor, Vector2 p_area) {}
void Editor_visualizer::render()
{
	if (jgl::get_button(jgl::mouse_button::left) != jgl::mouse_state::up || jgl::get_button(jgl::mouse_button::right) != jgl::mouse_state::up)
	{
		_pointer->render_differed(_controller->camera(), _first_voxel + _pointer->size() / 2);
	}
}