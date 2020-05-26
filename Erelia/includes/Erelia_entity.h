#ifndef ERELIA_ENTITY_H
#define ERELIA_ENTITY_H

#include "jgl.h"

using jgl::Vector2;
using jgl::Vector3;

class Entity
{
private:
	jgl::Mesh* _mesh;
	Vector3 _pos;
	Vector3 _size;
public:
	Entity(Vector3 p_pos, Vector3 p_size);
	void create_mesh(jgl::Color color);
	void bake();
	void render(jgl::Camera* camera);
};

#endif