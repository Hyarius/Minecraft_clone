#ifndef ERELIA_PLAYER_H
#define ERELIA_PLAYER_H

#include "jgl.h"

class Player
{
private:
	bool _fly_mode = true;
	float _move_speed = 0.15f;
	float _rot_speed = 0.2f;

	jgl::Mesh* _body = nullptr;

public:
	Player(jgl::Vector3 p_pos)
	{
		_body = jgl::primitive_cube(p_pos, 0, jgl::Vector3(1, 2, 1), jgl::Color(255, 255, 255), true);
	}
	~Player()
	{
		if (_body != nullptr)
			delete _body;
	}

	void set_fly_mode(bool state) { _fly_mode = state; }
	void set_move_speed(float value) { _move_speed = value; }
	void set_rot_speed(float value) { _rot_speed = value; }

	Vector3 pos() { return (_body->pos()); }
	bool fly_mode() { return (_fly_mode); }
	float move_speed() { return (_move_speed); }
	float rot_speed() { return (_rot_speed); }
	jgl::Mesh* body() { return (_body); }
};

#endif