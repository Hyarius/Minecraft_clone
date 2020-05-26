#include "erelia.h"

Entity::Entity(Vector3 p_pos, Vector3 p_size)
{
	_pos = p_pos;
	_size = p_size;
	_mesh = new jgl::Mesh(_pos + Vector3(0.0f, 1.0f, 0.5f));
}

void Entity::create_mesh(jgl::Color color)
{
	_mesh->clear();

	_mesh->add_point(Vector3(0.0f, 0.0f, 0.0f));
	_mesh->add_point(Vector3(_size.x, 0.0f, 0.0f));
	_mesh->add_point(Vector3(_size.x, _size.y, 0.0f));
	_mesh->add_point(Vector3(0.0f, _size.y, 0.0f));

	_mesh->add_uv(Vector2(0.0f, 0.0f));
	_mesh->add_uv(Vector2(1.0f, 0.0f));
	_mesh->add_uv(Vector2(0.0f, 1.0f));
	_mesh->add_uv(Vector2(1.0f, 1.0f));

	// FACE DOWN
	_mesh->add_face(jgl::Face(new int[3]{ 2, 3, 1 }, new int[3]{ 3, 2, 1 }, new int[3]{ -1, -1, -1 }, color));
	_mesh->add_face(jgl::Face(new int[3]{ 0, 1, 3 }, new int[3]{ 0, 1, 2 }, new int[3]{ -1, -1, -1 }, color));

	// FACE TOP
	_mesh->add_face(jgl::Face(new int[3]{ 2, 1, 3 }, new int[3]{ 3, 1, 2 }, new int[3]{ -1, -1, -1 }, color));
	_mesh->add_face(jgl::Face(new int[3]{ 0, 3, 1 }, new int[3]{ 0, 2, 1 }, new int[3]{ -1, -1, -1 }, color));

	_mesh->bake();
}

void Entity::bake()
{
	_mesh->bake();
}

void Entity::render(jgl::Camera *camera)
{
	_mesh->render_differed(camera, _mesh->pos());
}