#ifndef ERELIA_BOARD_H
#define ERELIA_BOARD_H

#include "jgl.h"

#include "Erelia_chunk.h"

class Board
{
private:
	std::map<Vector3, Chunk*> _chunks;
	std::vector<Entity*> _entities;
	jgl::Sprite_sheet* _tileset;

public:
	Board();

	void place_block(Vector3 pos, int type);
	void remove_block(Vector3 pos);

	void add_chunk(Vector3 chunk_pos);
	void remove_chunk(Vector3 chunk_pos);

	std::map<Vector3, Chunk*>& chunks() { return (_chunks); }
	std::vector<Entity*>& entities() { return _entities; }
	jgl::Sprite_sheet* tileset() { return (_tileset); }

	Vector3 chunk_pos(Vector3 abs_pos);
	Vector3 voxel_pos(Vector3 abs_pos);
	Chunk* chunks(Vector3 chunk_pos);
	Voxel* voxels(Vector3 pos);

	void update();
	void render(jgl::Camera* camera);
};

#endif