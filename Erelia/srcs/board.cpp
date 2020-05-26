#include "erelia.h"

jgl::Mesh* tmp_tree;

Board::Board()
{
	tmp_tree = new jgl::Mesh("ressources/model/cottage_obj.obj", Vector3(3, 3, 3), 0, 0.25f);
	_chunks.clear();

	_tileset = new jgl::Sprite_sheet("ressources/texture/tile_tileset.png", Vector2(16, 18));

	int min = 0;
	int max =  0;

	for (int i = min; i <= max; i++)
		for (int j = min; j <= max; j++)
		{
			Vector3 tmp(i, 0, j);
			add_chunk(tmp);
		}

	_entities.clear();
}

Vector3 tmp_neightbour[8]{
	{1, 0, 0},
	{0, 0, 1},
	{-1, 0, 0},
	{0, 0, -1},
	{1, 0, 1},
	{1, 0, -1},
	{-1, 0, 1},
	{-1, 0, -1}
};

void Board::add_chunk(jgl::Vector3 chunk_pos)
{
	_chunks[chunk_pos] = new Chunk(this->tileset(), chunk_pos);
	_chunks[chunk_pos]->bake(this);
	for (size_t i = 0; i < 8; i++)
	{
		if (_chunks.count(chunk_pos + tmp_neightbour[i]) != 0)
			_chunks[chunk_pos + tmp_neightbour[i]]->bake(this);
	}
}

void Board::remove_chunk(jgl::Vector3 chunk_pos)
{
	_chunks.erase(chunk_pos);
	for (size_t i = 0; i < 8; i++)
	{
		if (_chunks.count(chunk_pos + tmp_neightbour[i]) != 0)
			_chunks[chunk_pos + tmp_neightbour[i]]->bake(this);
	}
}

void Board::render(jgl::Camera *camera)
{
	for (auto tmp : _chunks)
		tmp.second->render(camera);
	for (size_t i = 0; i < _entities.size(); i++)
		_entities[i]->render(camera);
	for (auto tmp : _chunks)
		tmp.second->render_transparent(camera);
	tmp_tree->render(camera);
}

void Board::update()
{

}


void Board::place_block(Vector3 pos, int type)
{
	Vector3 tmp_chunk_pos = chunk_pos(pos);
	Chunk* tmp_chunk = chunks(tmp_chunk_pos);

	Vector3 voxel_pos = this->voxel_pos(pos);

	if (tmp_chunk == nullptr)
	{
		add_chunk(tmp_chunk_pos);
		tmp_chunk = chunks(tmp_chunk_pos);
	}
	else if (voxels(voxel_pos) != nullptr)
	{
		tmp_chunk->place_block(voxel_pos, type);
		tmp_chunk->bake(this);
		for (size_t i = 0; i < 8; i++)
		{
			Vector3 other = chunk_pos(pos + tmp_neightbour[i]);
			Voxel* other_voxel = voxels(pos + tmp_neightbour[i] - jgl::Vector3::up());
			if (tmp_chunk_pos != other && chunks(other) != nullptr)
				chunks(other)->bake(this);
		}
	}
}

void Board::remove_block(Vector3 pos)
{
	place_block(pos, -1);
}

Chunk* Board::chunks(Vector3 chunk_pos)
{
	if (_chunks.count(chunk_pos) == 0)
		return (nullptr);

	return (_chunks[chunk_pos]);
}


Vector3 Board::chunk_pos(Vector3 abs_pos)
{
	Vector3 result(floor(abs_pos.x / chunk_size.x), 0.0f, floor(abs_pos.z / chunk_size.z));

	return (result);
}

Vector3 Board::voxel_pos(Vector3 abs_pos)
{
	Vector3 result = abs_pos.floor() - chunk_pos(abs_pos) * chunk_size;
	return (result);
}

Voxel* Board::voxels(Vector3 pos)
{
	Chunk* tmp_chunk = chunks(chunk_pos(pos));
	if (tmp_chunk == nullptr)
		return (nullptr);

	Voxel* result = tmp_chunk->voxels(voxel_pos(pos));
	return (result);
}