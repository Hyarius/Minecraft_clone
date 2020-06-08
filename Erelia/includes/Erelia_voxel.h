#ifndef ERELIA_VOXEL_H
#define ERELIA_VOXEL_H

#include "jgl.h"

#include "Erelia_entity.h"

using jgl::Vector2;
using jgl::Vector3;

class Voxel
{
private:
	Vector3 _rel_pos;
	int _type;
	Entity* _entity;

public:
	Voxel(Vector3 p_rel_pos, int p_type, Entity* p_entity = nullptr);

 	int type() { return (_type); }
	Entity* entity() { return _entity; }

	void set_type(int p_type) { _type = p_type; }
	void set_entity(Entity* p_entity) { _entity = p_entity; }

	void edit_voxel_comp(jgl::Sprite_sheet* tileset, jgl::Mesh* target);
	void add_voxel_comp(jgl::Sprite_sheet* tileset, jgl::Mesh* target);
	void compose_face(class Board* board, Vector3 chunk_pos, int* dest, int face, int index);
	jgl::Mesh* construct(Board* board, Vector3 chunk_pos, jgl::Mesh* target = nullptr);

	void save(Vector3 delta, std::fstream& file);
};

#endif