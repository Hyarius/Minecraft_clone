#include "erelia.h"

void World::create_voxel_array()
{

}

void World::initiate_chunk(Vector3 pos)
{
	add_chunk(pos);
	prepare_empty_chunk(pos);
	baking_chunk(pos);
}

World::World()
{
	_chunks.clear();
	_entities.clear();
	_material = new jgl::Material("World Material");
	_material->diffuse_texture = new jgl::Image("ressources/texture/tile_tileset.png");
	Chunk::create_base_content_data(Vector2(1, 1) / Vector2(40, 30));
	
	initiate_chunk(0);
}

void World::clear_world()
{
	for (auto tmp : _chunks)
		delete tmp.second;
	_chunks.clear();
	_entities.clear();
}
World::~World()
{
	clear_world();
	if (_material != nullptr)
		delete _material;
	for (auto tmp : _chunks)
		delete tmp.second;
	Chunk::delete_base_content_data();
}

void World::reload(jgl::String path)
{
	clear_world();
	std::fstream input_file = jgl::open_file(path, std::ios_base::in);
	if (input_file.is_open() == false)
		error_exit(1, "Error while opening map " + path);
	else
	{
		size_t i = 0;
		while (input_file.eof() == false)
		{
			jgl::String line = jgl::get_str(input_file);
			if (line == "")
				break;
			jgl::Array<jgl::String> tab = jgl::strsplit(line, "/");
			if (tab.size() < 4)
				jgl::error_exit(1, "Map content corrupted at line [" + jgl::itoa(i) + "]");
			jgl::Vector3 pos = jgl::Vector3(stoi(tab[0]), stoi(tab[1]), stoi(tab[2]));
			int type = stoi(tab[3]);
			set_block(pos, type);
			if (tab.size() >= 5)
				place_scenery(pos, scenery_list[stoi(tab[4])]);
			i++;
		}
	}
	for (auto tmp : _chunks)
		tmp.second->bake(this);
}

World::World(jgl::String path)
{
	_material = new jgl::Material("World Material");
	_material->diffuse_texture = new jgl::Image("ressources/texture/tile_tileset.png");
	Chunk::create_base_content_data(Vector2(1, 1) / Vector2(40, 30));
	reload(path);
}

void World::prepare_empty_chunk(jgl::Vector3 chunk_pos)
{
	for (size_t i = 0; i < chunk_size.x; i++)
		for (size_t k = 0; k < chunk_size.y / 2; k++)
			for (size_t j = 0; j < chunk_size.z; j++)
			{
				Vector3 voxel_pos = this->voxel_pos(chunk_pos * chunk_size + jgl::Vector3(i, k, j));
				_chunks[chunk_pos]->place_block(voxel_pos, 0);
			}
}

void World::baking_chunk(jgl::Vector3 chunk_pos)
{
	for (size_t i = 0; i < 9; i++)
	{
		if (_chunks.count(chunk_pos + chunk_neighbour[i]) != 0)
			_chunks[chunk_pos + chunk_neighbour[i]]->bake(this);
	}
}

void World::add_chunk(jgl::Vector3 chunk_pos)
{
	_chunks[chunk_pos] = new Chunk(this->material(), chunk_pos);
	
}

void World::remove_chunk(jgl::Vector3 chunk_pos)
{
	_chunks.erase(chunk_pos);
	for (size_t i = 0; i < 9; i++)
	{
		if (_chunks.count(chunk_pos + chunk_neighbour[i]) != 0)
			_chunks[chunk_pos + chunk_neighbour[i]]->bake(this);
	}
}

void World::render(jgl::Camera* camera, const jgl::Viewport* viewport)
{
	render(camera, static_cast<size_t>(chunk_size.y), viewport);
}

void World::render(jgl::Camera* camera, int height, const jgl::Viewport* viewport)
{
	for (auto tmp : _chunks)
	{
		tmp.second->render(camera, height, viewport);
	}
	for (auto tmp : _chunks)
	{
		tmp.second->render_transparent(camera, height, viewport);
	}
}

void World::update()
{

}

void World::set_block(Vector3 pos, int type)
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

void World::place_block(Vector3 pos, int type)
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
		for (size_t i = 0; i < 9; i++)
		{
			Vector3 other = chunk_pos(pos + chunk_neighbour[i]);
			if (tmp_chunk_pos != other && chunks(other) != nullptr)
			{
				for (int j = -1; j <= 1; j++)
					chunks(other)->bake(this , static_cast<int>(voxel_pos.y) + j);
			}
		}
	}
}

void World::remove_block(Vector3 pos)
{
	place_block(pos, -1);
}

bool World::can_place_scenery(jgl::Vector3 pos, Scenery* p_scenery)
{
	if (p_scenery == nullptr)
		return (true);
	if (voxels(pos)->type() < 0)
		return (false);
	for (float i = 0; i < p_scenery->size().x; i++)
		for (float j = 0; j < p_scenery->size().y; j++)
			for (float h = 0; h < p_scenery->size().z; h++)
			{
				float x = pos.x + i / 2;
				float y = pos.y + j + 1;
				float z = pos.z + h / 2;

				Voxel* tmp = voxels(Vector3(x, y, z) + 0.5f);
				if (tmp != nullptr)
					if (tmp->type() != AIR_BLOCK)
						return (false);
			}
	return (true);
}

void World::place_scenery(jgl::Vector3 pos, Scenery* p_scenery)
{
	Vector3 tmp_chunk_pos = chunk_pos(pos);
	Chunk* tmp_chunk = chunks(tmp_chunk_pos);

	if (tmp_chunk == nullptr)
	{
		add_chunk(tmp_chunk_pos);
		tmp_chunk = chunks(tmp_chunk_pos);
	}
	Vector3 voxel_pos = this->voxel_pos(pos);
	if (voxels(voxel_pos) != nullptr && can_place_scenery(pos, p_scenery) == true)
		tmp_chunk->place_scenery(this, voxel_pos, p_scenery);
}

void World::remove_scenery(jgl::Vector3 pos)
{
	place_scenery(pos, nullptr);
}

Chunk* World::chunks(Vector3 chunk_pos)
{
	if (_chunks.count(chunk_pos.floor()) == 0)
		return (nullptr);

	return (_chunks[chunk_pos]);
}

Vector3 World::chunk_pos(Vector3 abs_pos)
{
	Vector3 result(floor(abs_pos.x / chunk_size.x), 0.0f, floor(abs_pos.z / chunk_size.z));

	return (result);
}

Vector3 World::voxel_pos(Vector3 tmp, Vector3 pos)
{
	Vector3 result = pos - tmp * chunk_size;
	return (result);
}

Vector3 World::voxel_pos(Vector3 abs_pos)
{
	return (voxel_pos(chunk_pos(abs_pos), abs_pos));
}

Voxel* World::voxels(Vector3 pos)
{
	Vector3 tmp = chunk_pos(pos);
	Chunk* tmp_chunk = chunks(tmp);
	if (tmp_chunk == nullptr)
		return (nullptr);

	Voxel* result = tmp_chunk->voxels(voxel_pos(tmp, pos));
	return (result);
}


void World::save(std::fstream& file)
{
	for (auto tmp : _chunks)
		tmp.second->save(file);
}