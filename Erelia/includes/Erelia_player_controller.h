#ifndef ERELIA_PLAYER_CONTROLLER_H
#define ERELIA_PLAYER_CONTROLLER_H

#include "jgl.h"
#include "Erelia_world.h"

using jgl::Vector3;
using jgl::Vector2;

class Player_controller : public jgl::Widget
{
protected:
	Player* _player;

	World* _world = nullptr;
	jgl::Camera* _camera = nullptr;
	jgl::Camera* _minimap_cam = nullptr;
	jgl::Vector3 _camera_target;

public:
	Player_controller(World* p_world, Player* p_player, jgl::Camera *p_minimap_cam, jgl::Widget *p_parent = nullptr);
	~Player_controller();

	World* world() { return (_world); }
	jgl::Camera* camera() { return (_camera); }

	void move_camera(Vector3 delta);
	void rotate_camera(float yaw, float pitch);

	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);

};

#endif