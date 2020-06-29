#include "erelia.h"

extern jgl::Array<float> block_alpha_array;
extern Vector3 voxel_neighbour[9];
extern Vector2 voxel_uv[35];
extern Vector3 voxel_normales[9];
extern jgl::Array<Vector2> block_uv_delta;

Vector3 chunk_size = Vector3(9, 20, 9);

Chunk::Chunk(jgl::Material* p_material, Vector3 p_pos)
{
	_material = p_material;
	_pos = p_pos;
	_voxels = new Voxel * **[static_cast<int>(floor(chunk_size.x))];
	int height = static_cast<int>(chunk_size.y) / 4;
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
	delete [] _voxels;
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
		if (_chunk_content_vertice[level].size() == 0)
		{
			for (int j = level * 2; j <= (level + 1) * 2; j++)
				for (int k = 0; k <= chunk_size.z * 2; k++)
					for (int i = 0; i <= chunk_size.x * 2; i++)
						_chunk_content_vertice[level].push_back(Vector3(i / 2.0f, j / 2.0f, k / 2.0f));
		}
		if (_chunk_content_uvs[level].size() == 0)
		{
			for (size_t type = 0; type < block_uv_delta.size(); type++)
				for (size_t i = 0; i < 35; i++)
					_chunk_content_uvs[level].push_back(unit * (voxel_uv[i] + block_uv_delta[type]));
		}
		if (_chunk_content_normales[level].size() == 0)
		{
			for (size_t i = 0; i < 9; i++)
				_chunk_content_normales[level].push_back(voxel_normales[i]);
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

bool Chunk::need_bake(Board* board, Vector3 p_pos)
{
	Voxel* actual = board->voxels(p_pos);
	Voxel* tmp_next;

	float base_alpha = block_alpha_array[actual->type()];
	for (size_t i = 3; i < 9; i++)
	{
		tmp_next = board->voxels(p_pos + voxel_neighbour[i]);
		float tmp_alpha = (tmp_next == nullptr || tmp_next->type() == -1 ? base_alpha : block_alpha_array[tmp_next->type()]);
		if (tmp_next == nullptr || tmp_next->type() == -1 || tmp_alpha != base_alpha)
			return (true);
	}
	return (false);
}

void Chunk::bake(Board* board, int level, jgl::Mesh **base_mesh)
{
	if (level < 0 || level >= chunk_size.y)
		return;

	jgl::Mesh* tmp_cube = *base_mesh;

	Vector3 starter_pos = _pos * chunk_size;

	_mesh->check_part(level)->clear_baked();
	_mesh_transparent->check_part(level)->clear_baked();

	for (int i = level; i < level + 1; i++)
	{
		for (int j = 0; j < chunk_size.x; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				Voxel* tmp_voxel = _voxels[j][i][k];
				if (tmp_voxel->type() != -1 && need_bake(board, Vector3(starter_pos.x + j, starter_pos.y + i, starter_pos.z + k)) == true)
				{
					jgl::Mesh* target = (block_alpha_array[tmp_voxel->type()] != 1 ? _mesh_transparent : _mesh);
					jgl::Mesh_part* target_part = target->check_part(i);
					tmp_cube = tmp_voxel->construct(board, _pos, tmp_cube);
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

void Chunk::bake(Board* board)
{
	jgl::Mesh* tmp_cube = nullptr;

	for (size_t i = 0; i < chunk_size.y; i++)
		bake(board, i, &tmp_cube);

	if (tmp_cube != nullptr)
		delete tmp_cube;
}

void Chunk::render(jgl::Camera* camera)
{
	_mesh->render_differed(camera, _pos * chunk_size);
	for (int i = 0; i < chunk_size.x; i++)
	{
		for (int j = 0; j < chunk_size.y; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				Voxel* tmp = _voxels[i][j][k];
				if (tmp->scenery() != nullptr)
				{
					tmp->scenery()->render(camera, _pos * chunk_size + Vector3(i, j, k) + Vector3(0.5f, 1.0f, 0.5f));
				}
			}
		}
	}
}

void Chunk::render_transparent(jgl::Camera* camera)
{
	_mesh_transparent->render_differed(camera, _pos * chunk_size);
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

void Chunk::place_scenery(jgl::Vector3 pos, Scenery* p_scenery)
{
	if (pos.x < 0 || pos.x >= chunk_size.x || pos.y < 0 || pos.y >= chunk_size.y || pos.z < 0 || pos.z >= chunk_size.z)
		return;

	int x = static_cast<int>(floor(pos.x));
	int y = static_cast<int>(floor(pos.y));
	int z = static_cast<int>(floor(pos.z));

	_voxels[x][y][z]->set_scenery(p_scenery);
}

Voxel* Chunk::voxels(Vector3 tmp_pos)
{
	if(tmp_pos.x < 0 || tmp_pos.x >= chunk_size.x || tmp_pos.y < 0 || tmp_pos.y >= chunk_size.y || tmp_pos.z < 0 || tmp_pos.z >= chunk_size.z)
		return nullptr;
	int x = static_cast<int>(floor(tmp_pos.x));
	int y = static_cast<int>(floor(tmp_pos.y));
	int z = static_cast<int>(floor(tmp_pos.z));
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