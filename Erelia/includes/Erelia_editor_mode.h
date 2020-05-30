#ifndef ERELIA_EDITOR_MODE_H
#define ERELIA_EDITOR_MODE_H

#include "jgl.h"
#include "Erelia_game_mode.h"
#include "Erelia_debug_screen.h"
#include "Erelia_editor_inventory.h"

class Editor_mode : public Game_mode
{
protected:
	Board* _board;
	jgl::Camera* _camera;
	Uint32 _actual_tick;
	Uint32 _timer; 
	Uint32 _delta_time;

	int _to_place;
	bool _should_draw;
	jgl::Mesh* _pointer;

	Vector3 _camera_target;
	Vector3 _voxel_source;
	Vector3 _voxel_target;

	jgl::Button* _save_button;
	jgl::Button* _load_button;

	Editor_inventory* _editor_inventory;

	Debug_screen* _debug_screen;

public:
	Editor_mode(class Game_engine* p_engine, Board* p_board);

	void move_camera(Vector3 delta);
	void rotate_camera(int yaw, int pitch);

	bool voxel_raycast(Vector3 pos, Vector3 direction, Vector3* voxel_source, Vector3* voxel_target);

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
};

#endif