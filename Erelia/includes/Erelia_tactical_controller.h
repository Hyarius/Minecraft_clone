#ifndef ERELIA_TACTICAL_CONTROLLER_H
#define ERELIA_TACTICAL_CONTROLLER_H

#include "jgl.h"
#include "Erelia_board.h"

using jgl::Vector3;
using jgl::Vector2;

class Tactical_controller : public jgl::Widget
{
protected:
	Board* _board;
	jgl::Camera* _camera;

	Vector3 _camera_target;

public:
	Tactical_controller(Board* p_board, jgl::Widget *p_parent = nullptr);
	~Tactical_controller();

	Board* board() { return (_board); }
	jgl::Camera* camera() { return (_camera); }

	void move_camera(Vector3 delta);
	void rotate_camera(int yaw, int pitch);

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);

};

#endif