#ifndef ERELIA_CHUNK_H
#define ERELIA_CHUNK_H

#include "jgl.h"

#include "erelia_voxel.h"

using jgl::Vector2;
using jgl::Vector3;

class Chunk
{
private:
	jgl::Sprite_sheet* _tileset;
	jgl::Mesh* _mesh;
	jgl::Mesh* _mesh_transparent;
	Vector3 _pos = -1;
	Voxel**** _voxels;
public:
	Chunk(jgl::Sprite_sheet *p_tile_tileset, Vector3 p_pos);
	void init_mesh(jgl::Mesh* target);
	bool need_bake(class Board* board, Vector3 pos);
	void bake(class Board* board);
	void render(jgl::Camera* camera);
	void render_transparent(jgl::Camera *camera);
	void place_block(Vector3 pos, int type);
	Voxel**** voxels() { return (_voxels); }
	Voxel* voxels(jgl::Vector3 tmp_pos);
};

#endif