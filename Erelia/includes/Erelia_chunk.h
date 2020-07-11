#ifndef ERELIA_CHUNK_H
#define ERELIA_CHUNK_H

#include "jgl.h"

#include "erelia_voxel.h"

extern jgl::Vector3 chunk_neighbour[9];

using jgl::Vector2;
using jgl::Vector3;

class Chunk
{
private:
	//static inline jgl::Array< jgl::Array< jgl::Vector3 > > _chunk_content_vertice;
	//static inline jgl::Array< jgl::Array< jgl::Vector2 > > _chunk_content_uvs;
	//static inline jgl::Array< jgl::Array< jgl::Vector3 > > _chunk_content_normales;

	static inline jgl::Array< jgl::share_object< jgl::Array< jgl::Vector3 > > > _chunk_content_vertice;
	static inline jgl::Array< jgl::share_object< jgl::Array< jgl::Vector2 > > > _chunk_content_uvs;
	static inline jgl::Array< jgl::share_object< jgl::Array< jgl::Vector3 > > > _chunk_content_normales;

	jgl::Material* _material = nullptr;
	jgl::Mesh* _mesh;
	jgl::Mesh* _mesh_transparent;
	Vector3 _pos = -1;
	Voxel**** _voxels = nullptr;

public:	
	static void create_base_content_data(Vector2 unit);
	static void delete_base_content_data();

	Chunk(jgl::Material* p_material, Vector3 p_pos);
	~Chunk();

	void init_mesh(jgl::Mesh* target);
	void bake(class World* world);
	void bake(World* world, int y_index, jgl::Mesh** base_mesh = nullptr);
	void render(jgl::Camera* camera, const jgl::Viewport* viewport = nullptr);
	void render(jgl::Camera* camera, int height, const jgl::Viewport* viewport = nullptr);
	void render_transparent(jgl::Camera* camera, const jgl::Viewport* viewport = nullptr);
	void render_transparent(jgl::Camera *camera, int height, const jgl::Viewport* viewport = nullptr);
	void place_block(Vector3 pos, int type);
	void place_scenery(World* world, jgl::Vector3 pos, Scenery* p_scenery);
	Voxel**** voxels() { return (_voxels); }
	Voxel* voxels(jgl::Vector3 tmp_pos);
	Vector3 pos() { return (_pos); }
	void save(std::fstream& file);
};

#endif