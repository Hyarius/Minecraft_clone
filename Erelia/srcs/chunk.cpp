#include "erelia.h"

extern jgl::Array<float> block_alpha_array;
extern Vector3 voxel_neighbour[9];
extern Vector2 voxel_uv[35];
extern Vector3 voxel_normales[9];
extern jgl::Array<Vector2> block_uv_delta;

Vector3 chunk_size = Vector3(16, 20, 16);

Chunk::Chunk(jgl::Material* p_material, Vector3 p_pos)
{
	_material = p_material;
	_pos = p_pos;

	_voxels = new Voxel ***[static_cast<int>(floor(chunk_size.x))];
	for (int i = 0; i < chunk_size.x; i++)
	{
		_voxels[i] = new Voxel **[static_cast<int>(floor(chunk_size.y))];
		for (int j = 0; j < chunk_size.y; j++)
		{
			_voxels[i][j] = new Voxel *[static_cast<int>(floor(chunk_size.z))];
			for (int k = 0; k < chunk_size.z; k++)
			{
				Vector3 tmp = Vector3(i, j, k);
				_voxels[i][j][k] = new Voxel(tmp, -1);
			}
		}
	}

	_mesh = new jgl::Mesh(0);
	init_mesh(_mesh);
	_mesh->set_material(_material, true);

	_mesh_transparent = new jgl::Mesh(0);
	init_mesh(_mesh_transparent);
	_mesh_transparent->set_transparency(0.5f);
	_mesh_transparent->set_material(_material, true);
}

Chunk::~Chunk()
{
	for (int i = 0; i < chunk_size.x; i++)
	{
		for (int j = 0; j < chunk_size.y; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				delete _voxels[i][j][k];
			}
			delete[] _voxels[i][j];
		}
		delete[] _voxels[i];
	}
	delete[] _voxels;
	delete _mesh;
	delete _mesh_transparent;
}

void Chunk::create_base_content_data(Vector2 unit)
{
	if (_chunk_content_vertice.size() == 0)
		_chunk_content_vertice.resize(static_cast<int>(chunk_size.y));
	if (_chunk_content_uvs.size() == 0)
		_chunk_content_uvs.resize(static_cast<int>(chunk_size.y));
	if (_chunk_content_normales.size() == 0)
		_chunk_content_normales.resize(static_cast<int>(chunk_size.y));
	for (int level = 0; level < chunk_size.y; level++)
	{
		if (_chunk_content_vertice[level]->size() == 0)
		{
			for (int j = level * 2; j <= (level + 1) * 2; j++)
				for (int k = 0; k <= chunk_size.z * 2; k++)
					for (int i = 0; i <= chunk_size.x * 2; i++)
						_chunk_content_vertice[level]->push_back(Vector3(i / 2.0f, j / 2.0f, k / 2.0f));
		}
		if (_chunk_content_uvs[level]->size() == 0)
		{
			for (size_t type = 0; type < block_uv_delta.size(); type++)
				for (size_t i = 0; i < 35; i++)
					_chunk_content_uvs[level]->push_back(unit * (voxel_uv[i] + block_uv_delta[type]));
		}
		if (_chunk_content_normales[level]->size() == 0)
		{
			for (size_t i = 0; i < 9; i++)
				_chunk_content_normales[level]->push_back(voxel_normales[i]);
		}
	}
}

void Chunk::delete_base_content_data()
{

}

void Chunk::init_mesh(jgl::Mesh *target)
{
	for (int level = 0; level < chunk_size.y; level++)
	{
		target->set_vertices(_chunk_content_vertice[level], level);
		target->set_uvs(_chunk_content_uvs[level], level);
		target->set_normales(_chunk_content_normales[level], level);
	}
}

bool need_bake(World* world, Chunk ** chunk_array, Voxel** voxel_array, Vector3 p_pos)
{
	bool result = false;
	Voxel* actual = chunk_array[4]->voxels(p_pos);

	if (actual->type() < 0)
		return (false);
	float base_alpha = block_alpha_array[actual->type()];
	for (size_t i = 0; i < 9; i++)
	{
		Voxel* other_voxel = nullptr;
		Chunk* other_chunk = nullptr;
		Vector3 other_pos = p_pos + voxel_neighbour[i];
		size_t chunk_index;

		chunk_index = (other_pos.x < 0 ? 0 : (other_pos.x >= chunk_size.x ? 2 : 1)) + (other_pos.z < 0 ? 0 : (other_pos.z >= chunk_size.z ? 6 : 3));
		other_chunk = chunk_array[chunk_index];

		Vector3 rel_other_pos = other_pos - chunk_neighbour[chunk_index] * chunk_size * Vector3(1, 0, 1);
		if (other_chunk != nullptr)
			other_voxel = other_chunk->voxels(rel_other_pos);

		voxel_array[i] = other_voxel;

		if (other_voxel == nullptr || other_voxel->type() <= AIR_BLOCK || block_alpha_array[other_voxel->type()] != base_alpha)
			result = true;
	}
	return (result);
}

void Chunk::bake(World* world, int level, jgl::Mesh **base_mesh)
{
	if (level < 0 || level >= chunk_size.y)
		return;

	jgl::Mesh* tmp_cube;
	if (base_mesh != nullptr)
		tmp_cube = *base_mesh;
	else
		tmp_cube = new jgl::Mesh(0, 0, 1);

	Chunk* chunk_array[9] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};
	Voxel* voxel_array[9] = {
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
	};
	Vector3 starter_pos = _pos * chunk_size;

	for (size_t i = 0; i < 9; i++)
		chunk_array[i] = world->chunks(this->pos() + chunk_neighbour[i]);

	_mesh->check_part(level)->clear_baked();
	_mesh_transparent->check_part(level)->clear_baked();

	for (int i = level; i < level + 1; i++)
	{
		for (int j = 0; j < chunk_size.x; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				if (need_bake(world, chunk_array, voxel_array, Vector3(j, i, k)) == true)
				{
					Voxel* tmp_voxel = _voxels[j][i][k];
					jgl::Mesh* target = (block_alpha_array[tmp_voxel->type()] != 1 ? _mesh_transparent : _mesh);
					jgl::Mesh_part* target_part = target->check_part(i);
					tmp_cube = tmp_voxel->construct(world, chunk_array, voxel_array, tmp_cube);
					if (tmp_cube != nullptr)
					{
						jgl::Mesh_part* other = tmp_cube->check_part(0);
						for (size_t l = 0; l < other->faces().size(); l++)
						{
							target_part->add_face(*(other->faces(l)));
						}
					}
				}
			}
		}
	}
	if (base_mesh == nullptr)
		delete tmp_cube;
	else
		*base_mesh = tmp_cube;
	_mesh->check_part(level)->bake(_mesh->rot_matrix());
	_mesh_transparent->check_part(level)->bake(_mesh->rot_matrix());
}

void Chunk::bake(World* world)
{
	jgl::Mesh* tmp_cube = nullptr;

	for (size_t i = 0; i < chunk_size.y; i++)
		bake(world, i, &tmp_cube);

	if (tmp_cube != nullptr)
		delete tmp_cube;
}

void Chunk::render(jgl::Camera* camera, const jgl::Viewport* viewport)
{
	render(camera, static_cast<size_t>(chunk_size.y), viewport);
}

void Chunk::render(jgl::Camera* camera, int height, const jgl::Viewport* viewport)
{
	if (_mesh->parts().size() == 0)
		jgl::error_exit(1, "Chunk not baked succesfully");
	for (int j = 0; j < chunk_size.y && j < height; j++)
	{
		_mesh->parts(j)->render(_mesh, camera, _pos * chunk_size, viewport);
		for (int i = 0; i < chunk_size.x; i++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				Voxel* tmp = _voxels[i][j][k];
				if (tmp->scenery() != nullptr)
				{
					tmp->scenery()->render(camera, _pos * chunk_size + Vector3(i, j, k) + Vector3(0.5f, 1.0f, 0.5f), viewport);
				}
			}
		}
	}
}

void Chunk::render_transparent(jgl::Camera* camera, const jgl::Viewport* viewport)
{
	render_transparent(camera, static_cast<size_t>(chunk_size.y), viewport);
}

void Chunk::render_transparent(jgl::Camera* camera, int height, const jgl::Viewport* viewport)
{
	for (int j = 0; j < chunk_size.y && j < height; j++)
	{
		_mesh_transparent->parts(j)->render(_mesh_transparent, camera, _pos * chunk_size, viewport);
	}
}

void Chunk::place_block(Vector3 pos, int type)
{
	if (pos.x < 0 || pos.x >= chunk_size.x || pos.y < 0 || pos.y >= chunk_size.y || pos.z < 0 || pos.z >= chunk_size.z)
		return ;

	int x = static_cast<int>(floor(pos.x));
	int y = static_cast<int>(floor(pos.y));
	int z = static_cast<int>(floor(pos.z));
	
	_voxels[x][y][z]->set_type(type);
}

void Chunk::place_scenery(World *world, jgl::Vector3 pos, Scenery* p_scenery)
{
	if (pos.x < 0 || pos.x >= chunk_size.x || pos.y < 0 || pos.y >= chunk_size.y || pos.z < 0 || pos.z >= chunk_size.z)
		return;

	int x = static_cast<int>(floor(pos.x));
	int y = static_cast<int>(floor(pos.y));
	int z = static_cast<int>(floor(pos.z));

	Scenery* actual = voxels(Vector3(x, y, z))->scenery();

	voxels(Vector3(x, y, z))->set_scenery(p_scenery);
	if (actual != nullptr)
	{
		for (float i = 0; i < actual->size().x; i++)
			for (float j = 0; j < actual->size().y; j++)
				for (float h = 0; h < actual->size().z; h++)
				{
					float x = pos.x + i / 2;
					float y = pos.y + j + 1;
					float z = pos.z + h / 2;

					Voxel* tmp = world->voxels(Vector3(x, y, z) + 0.5f + _pos * chunk_size);
					if (tmp != nullptr)
						tmp->set_type(AIR_BLOCK);
				}
	}
	if (p_scenery != nullptr)
	{
		for (float i = 0; i < p_scenery->size().x; i++)
			for (float j = 0; j < p_scenery->size().y; j++)
				for (float h = 0; h < p_scenery->size().z; h++)
				{
					float x = pos.x + i / 2;
					float y = pos.y + j + 1;
					float z = pos.z + h / 2;

					Voxel* tmp = world->voxels(Vector3(x, y, z) + 0.5f + _pos * chunk_size);
					if (tmp != nullptr)
						tmp->set_type(SCENERY_BLOCK);
				}
	}
	
}

Voxel* Chunk::voxels(Vector3 tmp_pos)
{
	if(tmp_pos.x < 0 || tmp_pos.x >= chunk_size.x || tmp_pos.y < 0 || tmp_pos.y >= chunk_size.y || tmp_pos.z < 0 || tmp_pos.z >= chunk_size.z)
		return nullptr;
	int x = static_cast<int>(tmp_pos.x);
	int y = static_cast<int>(tmp_pos.y);
	int z = static_cast<int>(tmp_pos.z);
	return (_voxels[x][y][z]);
}

void Chunk::save(std::fstream& file)
{
	for (int i = 0; i < chunk_size.x; i++)
	{
		for (int j = 0; j < chunk_size.y; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				_voxels[i][j][k]->save(_pos * chunk_size, file);
			}
		}
	}
}