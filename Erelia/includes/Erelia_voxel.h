#ifndef ERELIA_VOXEL_H
#define ERELIA_VOXEL_H

#include "jgl.h"

#include "Erelia_entity.h"
#include "Erelia_scenery.h"

using jgl::Vector2;
using jgl::Vector3;

#define AIR_BLOCK -1
#define SCENERY_BLOCK -2

class Voxel
{
private:
	Vector3 _rel_pos;
	int _type;
	Scenery* _scenery;
	Entity* _entity;

public:
	Voxel(Vector3 p_rel_pos, int p_type);

	Vector3 pos() { return (_rel_pos); }
	int type() { return (_type); }
	Entity* entity() { return _entity; }
	Scenery* scenery() { return _scenery; }

	void set_scenery(Scenery* p_scenery) { _scenery = p_scenery; }
	void set_type(int p_type) { _type = p_type; }
	void set_entity(Entity* p_entity) { _entity = p_entity; }

	void edit_voxel_comp(jgl::Sprite_sheet* tileset, jgl::Mesh* target);
	void add_voxel_comp(jgl::Sprite_sheet* tileset, jgl::Mesh* target);
	void compose_face(class World* world, Vector3 chunk_pos, int* dest, int face, int index);
	jgl::Mesh* construct(class World* world, class Chunk** chunks, Voxel **voxel_array, jgl::Mesh* target);

	void save(Vector3 delta, std::fstream& file);
};

#endif