#include "erelia.h"

Editor_visualizer::Editor_visualizer(Tactical_controller* p_controller, jgl::Widget* p_parent) : jgl::Widget(p_parent), _controller(p_controller)
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
		_pos_vector.clear();
		_first_voxel = p_first;
		_second_voxel = p_second;
		Vector3 start = Vector3(min(_first_voxel.x, _second_voxel.x), min(_first_voxel.y, _second_voxel.y), min(_first_voxel.z, _second_voxel.z));
		Vector3 end = Vector3(max(_first_voxel.x, _second_voxel.x), max(_first_voxel.y, _second_voxel.y), max(_first_voxel.z, _second_voxel.z));

		bool tmp_search = _controller->board()->voxels(_first_voxel) == nullptr || _controller->board()->voxels(_first_voxel)->type() == -1;
		for (int i = static_cast<int>(start.x); i <= static_cast<int>(end.x); i++)
			for (int j = static_cast<int>(start.y); j <= static_cast<int>(end.y); j++)
				for (int k = static_cast<int>(start.z); k <= static_cast<int>(end.z); k++)
				{
					Vector3 tmp = Vector3(i, j, k);
					if ((tmp_search == true && (_controller->board()->voxels(tmp) == nullptr || _controller->board()->voxels(tmp)->type() == -1)) ||
						(tmp_search == false && _controller->board()->voxels(tmp) != nullptr && _controller->board()->voxels(tmp)->type() != -1))
						_pos_vector.push_back(Vector3(i, j, k));
				}
		recalculate();
	}
}
void Editor_visualizer::recalculate()
{
	for (size_t i = 1; i < _pos_vector.size(); i++)
	{
		if (_pos_vector[i - 1].distance(_controller->camera()->pos()) > _pos_vector[i].distance(_controller->camera()->pos()))
		{
			Vector3 tmp = _pos_vector[i - 1];
			_pos_vector[i - 1] = _pos_vector[i];
			_pos_vector[i] = tmp;
			i = 0;
		}
	}
}

void Editor_visualizer::set_geometry_imp(Vector2 p_anchor, Vector2 p_area) {}
void Editor_visualizer::render()
{
	if (jgl::get_button(jgl::mouse_button::left) != jgl::mouse_state::up || jgl::get_button(jgl::mouse_button::right) != jgl::mouse_state::up)
	{
		for (size_t i = 0; i < _pos_vector.size(); i++)
		{
			_pointer->render_differed(_controller->camera(), _pos_vector[i] + 0.5f);
		}
	}
}