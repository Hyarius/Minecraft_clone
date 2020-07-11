#ifndef ERELIA_BOARD_H
#define ERELIA_BOARD_H

#include "jgl.h"

#include "Erelia_chunk.h"

class World
{
private:
	std::map<Vector3, Chunk*> _chunks;
	std::vector<Entity*> _entities;
	jgl::Material* _material;
	void clear_world();

public:
	World();
	World(jgl::String path);
	~World();

	void reload(jgl::String);

	void initiate_chunk(Vector3 pos);
	void prepare_empty_chunk(jgl::Vector3 chunk_pos);
	void create_voxel_array();

	void set_block(Vector3 pos, int type);
	void place_block(Vector3 pos, int type);
	void remove_block(Vector3 pos);

	bool can_place_scenery(jgl::Vector3 pos, Scenery* p_scenery);
	void place_scenery(jgl::Vector3 pos, Scenery* p_scenery);
	void remove_scenery(jgl::Vector3 pos);

	void add_chunk(Vector3 chunk_pos);
	void baking_chunk(jgl::Vector3 chunk_po);
	void remove_chunk(Vector3 chunk_pos);

	std::map<Vector3, Chunk*>& chunks() { return (_chunks); }
	std::vector<Entity*>& entities() { return _entities; }
	jgl::Material* material() { return (_material); }

	Vector3 chunk_pos(Vector3 abs_pos);
	Vector3 voxel_pos(Vector3 chunk_pos, Vector3 pos);
	Vector3 voxel_pos(Vector3 abs_pos);
	Chunk* chunks(Vector3 chunk_pos);
	Voxel* voxels(Vector3 pos);

	void update();
	void render(jgl::Camera* camera, const jgl::Viewport* viewport = nullptr);
	void render(jgl::Camera* camera, int height, const jgl::Viewport* viewport = nullptr);

	void save(std::fstream& file);
};

#endif