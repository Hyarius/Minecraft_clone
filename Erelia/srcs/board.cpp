#include "erelia.h"

void Board::empty_initialize()
{
	int min = 0;
	int max = 0;

	for (int i = min; i <= max; i++)
		for (int j = min; j <= max; j++)
		{
			Vector3 tmp(i, 0, j);
			add_chunk(tmp);
			prepare_empty_chunk(tmp);
			baking_chunk(tmp);
		}
}

jgl::Mesh *test = nullptr;

Board::Board()
{
	_chunks.clear();
	_entities.clear();
	_tileset = new jgl::Sprite_sheet("ressources/texture/tile_tileset.png", Vector2(40, 30));
	empty_initialize();
	test = new jgl::Mesh("ressources/model/forest-rock.obj", Vector3(0.5f, 1.0f, 0.5f), 0, 1);
}

void Board::reload(jgl::String path)
{
	_chunks.clear();
	_entities.clear();
	std::fstream input_file = jgl::open_file(path, std::ios_base::in);
	if (input_file.is_open() == false)
		empty_initialize();
	else
	{
		size_t i = 0;
		while (input_file.eof() == false)
		{
			jgl::String line = jgl::get_str(input_file);
			if (line == "")
				break;
			std::vector<jgl::String> tab = jgl::strsplit(line, "/");
			if (tab.size() < 4)
				jgl::error_exit(1, "Map content corrupted at line [" + jgl::itoa(i) + "]");
			jgl::Vector3 pos = jgl::Vector3(stoi(tab[0]), stoi(tab[1]), stoi(tab[2]));
			int type = stoi(tab[3]);
			i++;
			set_block(pos, type);
		}
	}
	for (auto tmp : _chunks)
	{
		tmp.second->bake(this);
	}
}

Board::Board(jgl::String path)
{
	_tileset = new jgl::Sprite_sheet("ressources/texture/tile_tileset.png", Vector2(40, 30));
	reload(path);
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

void Board::prepare_empty_chunk(jgl::Vector3 chunk_pos)
{
	Vector3 pos = 0;
	for (size_t i = 0; i < chunk_size.x; i++)
		for (size_t j = 0; j < chunk_size.z; j++)
		{
			Vector3 voxel_pos = this->voxel_pos(chunk_pos * chunk_size + jgl::Vector3(i, size_t(0), j));
			_chunks[chunk_pos]->place_block(voxel_pos, 0);
		}
}

void Board::baking_chunk(jgl::Vector3 chunk_pos)
{
	_chunks[chunk_pos]->bake(this);
	for (size_t i = 0; i < 8; i++)
	{
		if (_chunks.count(chunk_pos + tmp_neightbour[i]) != 0)
			_chunks[chunk_pos + tmp_neightbour[i]]->bake(this);
	}
}

void Board::add_chunk(jgl::Vector3 chunk_pos)
{
	_chunks[chunk_pos] = new Chunk(this->tileset(), chunk_pos);
	
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
	if (test != nullptr)
		test->render(camera);
}

void Board::update()
{

}

void Board::set_block(Vector3 pos, int type)
{
	Vector3 tmp_chunk_pos = chunk_pos(pos);
	Chunk* tmp_chunk = chunks(tmp_chunk_pos);

	if (tmp_chunk == nullptr)
	{
		add_chunk(tmp_chunk_pos);
		tmp_chunk = chunks(tmp_chunk_pos);
	}
	Vector3 voxel_pos = this->voxel_pos(pos);
	if (voxels(pos) != nullptr)
	{
		tmp_chunk->place_block(voxel_pos, type);
	}
}

void Board::place_block(Vector3 pos, int type)
{
	Vector3 tmp_chunk_pos = chunk_pos(pos);
	Chunk* tmp_chunk = chunks(tmp_chunk_pos);

	if (tmp_chunk == nullptr)
	{
		add_chunk(tmp_chunk_pos);
		tmp_chunk = chunks(tmp_chunk_pos);
	}
	Vector3 voxel_pos = this->voxel_pos(pos);
	if (voxels(voxel_pos) != nullptr)
	{
		tmp_chunk->place_block(voxel_pos, type);
		for (int j = -1; j <= 1; j++)
			tmp_chunk->bake(this , static_cast<int>(voxel_pos.y) + j);
		for (size_t i = 0; i < 8; i++)
		{
			Vector3 other = chunk_pos(pos + tmp_neightbour[i]);
			if (tmp_chunk_pos != other && chunks(other) != nullptr)
			{
				for (int j = -1; j <= 1; j++)
					chunks(other)->bake(this , static_cast<int>(voxel_pos.y) + j);
			}
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


void Board::save(std::fstream& file)
{
	for (auto tmp : _chunks)
		tmp.second->save(file);
}