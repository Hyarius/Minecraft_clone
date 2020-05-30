#include "erelia.h"

extern float block_alpha_array[12];
extern Vector3 voxel_neighbour[9];
extern Vector2 voxel_uv[35];
extern Vector3 voxel_normales[9];
extern std::vector<Vector2> uv_type_delta;

std::vector<jgl::Vector3> chunk_content_vertice;
std::vector<jgl::Vector2> chunk_content_uvs;
std::vector<jgl::Vector3> chunk_content_normales;

jgl::Mesh* tmp_cube = nullptr;
Vector3 chunk_size = Vector3(9, 9, 9);

Chunk::Chunk(jgl::Sprite_sheet* p_tileset, Vector3 p_pos)
{
	_tileset = p_tileset;
	_pos = p_pos;
	_voxels = new Voxel * **[static_cast<int>(floor(chunk_size.x))];
	int height = chunk_size.y / 2;
	for (int i = 0; i < chunk_size.x; i++)
	{
		_voxels[i] = new Voxel **[static_cast<int>(floor(chunk_size.y))];
		for (int j = 0; j < chunk_size.y; j++)
		{
			_voxels[i][j] = new Voxel *[static_cast<int>(floor(chunk_size.z))];
			for (int k = 0; k < chunk_size.z; k++)
			{
				Vector3 tmp = Vector3(i, j, k);
				_voxels[i][j][k] = new Voxel(tmp, (j <= height ? 0 : -1), nullptr);
			}
		}
	}
	_mesh = new jgl::Mesh(0);
	_mesh->set_diffuse_texture(_tileset);
	init_mesh(_mesh);
	_mesh_transparent = new jgl::Mesh(0);
	_mesh_transparent->set_transparency(0.5f);
	_mesh_transparent->set_diffuse_texture(_tileset);
	init_mesh(_mesh_transparent);
}

void Chunk::init_mesh(jgl::Mesh *target)
{
	if (chunk_content_vertice.size() == 0)
	{
		for (int k = 0; k <= chunk_size.z * 2; k++)
			for (int j = 0; j <= chunk_size.y * 2; j++)
				for (int i = 0; i <= chunk_size.x * 2; i++)
					chunk_content_vertice.push_back(Vector3(i / 2.0f, j / 2.0f, k / 2.0f));
	}
	if (chunk_content_uvs.size() == 0)
	{
		for (size_t type = 0; type < uv_type_delta.size(); type++)
			for (size_t i = 0; i < 35; i++)
				chunk_content_uvs.push_back(_tileset->unit() * (voxel_uv[i] + uv_type_delta[type]));
	}
	if (chunk_content_normales.size() == 0)
	{
		for (size_t i = 0; i < 9; i++)
			chunk_content_normales.push_back(voxel_normales[i]);
	}
	jgl::Mesh_part* part = target->check_part(0);
	part->set_vertices(chunk_content_vertice);
	part->set_uvs(chunk_content_uvs);
	part->set_normales(chunk_content_normales);
}

bool Chunk::need_bake(Board* board, Vector3 p_pos)
{
	Voxel* actual = board->voxels(p_pos);
	Voxel* tmp_next;

	float base_alpha = block_alpha_array[actual->type()];
	for (size_t i = 3; i < 9; i++)
	{
		tmp_next = board->voxels(p_pos + voxel_neighbour[i]);
		float tmp_alpha = (tmp_next == nullptr ? base_alpha : block_alpha_array[tmp_next->type()]);
		if (tmp_next == nullptr || tmp_next->type() == -1 || tmp_alpha != base_alpha)
			return (true);
	}
	return (false);
}

void Chunk::bake(Board *board)
{
	Vector3 starter_pos = _pos * chunk_size;

	_mesh->clear_baked();
	_mesh_transparent->clear_baked();
	for (int i = 0; i < chunk_size.y; i++)
	{
		for (int j = 0; j < chunk_size.x; j++)
		{
			for (int k = 0; k < chunk_size.z; k++)
			{
				Voxel* tmp_voxel = _voxels[j][i][k];
				if (tmp_voxel->type() != -1 && need_bake(board, Vector3(starter_pos.x + j, starter_pos.y + i, starter_pos.z + k)) == true)
				{
					tmp_cube = tmp_voxel->construct(board, _pos, tmp_cube);
					if (tmp_cube != nullptr)
					{
						jgl::Mesh_part *other = tmp_cube->check_part(0);
						for (size_t i = 0; i < other->faces().size(); i++)
						{
							jgl::Mesh *target = (block_alpha_array[tmp_voxel->type()] != 1 ? _mesh_transparent : _mesh);
							target->add_face(*(other->faces(i)));
						}
					}
				}
			}
		}
	}
	_mesh->bake();
	_mesh_transparent->bake();
}

void Chunk::render(jgl::Camera* camera)
{
	_mesh->render_differed(camera, _pos * chunk_size);
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

Voxel* Chunk::voxels(Vector3 tmp_pos)
{
	if(tmp_pos.x < 0 || tmp_pos.x >= chunk_size.x || tmp_pos.y < 0 || tmp_pos.y >= chunk_size.y || tmp_pos.z < 0 || tmp_pos.z >= chunk_size.z)
		return nullptr;
	int x = static_cast<int>(floor(tmp_pos.x));
	int y = static_cast<int>(floor(tmp_pos.y));
	int z = static_cast<int>(floor(tmp_pos.z));
	return (_voxels[x][y][z]);
}