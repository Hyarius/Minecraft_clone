#include "erelia.h"

extern Vector3 voxel_vertices[13];
extern int vertices_face_index[9][4];
extern Vector3 voxel_neighbour[9];
extern int face_index_order[2][3];
extern Vector2 voxel_uv[35];
extern Vector3 voxel_normales[9];
extern jgl::Array<Vector2> block_uv_delta;
extern int  uvs_face_index[9][4];
extern int uvs_top_face_index[4][2][2][2][1];
extern Vector3 neightbour_compose_face[4][3];
extern int delta_face_index[2][3];
extern jgl::Array<float> block_alpha_array;

Voxel::Voxel(Vector3 p_rel_pos, int p_type)
{
	_rel_pos = p_rel_pos;
	_type = p_type;
	_entity = nullptr;
	_scenery = nullptr;
}

#define IDENTICAL 0
#define DIFFERENT 1

void Voxel::add_voxel_comp(jgl::Sprite_sheet *tileset, jgl::Mesh* target)
{
	for (size_t i = 0; i < 13; i++)
	{
		target->add_point(voxel_vertices[i]);
	}
	for (size_t i = 0; i < 9; i++)
		target->add_normale(voxel_normales[i]);
	for (size_t i = 0; i < 35; i++)
	{
		target->add_uv((voxel_uv[i] + block_uv_delta[_type]) * (tileset == nullptr ? 0 : tileset->unit()));
	}
}

void Voxel::edit_voxel_comp(jgl::Sprite_sheet* tileset, jgl::Mesh* target)
{
	jgl::Array<Vector2>& tmp = target->control_part(0)->uvs();

	for (size_t i = 0; i < 35; i++)
	{
		tmp[i] = ((voxel_uv[i] + block_uv_delta[_type]) * (tileset == nullptr ? 0 : tileset->unit()));
	}
}

void Voxel::compose_face(Board *board, Vector3 chunk_pos, int* dest, int face, int index)
{
	if (face >= 4)
	{
		int sprite_index_delta = _type * 35;
		for (size_t j = 0; j < 3; j++)
			dest[j] = uvs_face_index[face][face_index_order[index][j]] + sprite_index_delta;
	}
	else
	{
		Vector3 tmp_pos = _rel_pos + chunk_pos * chunk_size;
		Chunk* tmp_chunk = board->chunks(chunk_pos);
		Voxel *voxels_to_try[6] = {
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 0]),
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 1]),
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 2]),
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 0] + jgl::Vector3::up()),
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 1] + jgl::Vector3::up()),
			tmp_chunk->voxels(_rel_pos + neightbour_compose_face[face][2 - 2] + jgl::Vector3::up())
		};
		int voxel_value[3] = {IDENTICAL, IDENTICAL, IDENTICAL};
		for (size_t j = 0; j < 3; j++)
		{
			if (voxels_to_try[j] == nullptr)
			{
				voxels_to_try[j] = board->voxels(tmp_pos + neightbour_compose_face[face][2 - j]);
				voxels_to_try[j + 3] = board->voxels(tmp_pos + neightbour_compose_face[face][2 - j] + jgl::Vector3::up());
			}
			if (voxels_to_try[j] == nullptr || (voxels_to_try[j + 3] != nullptr && (voxels_to_try[j + 3]->type() != -1 && block_alpha_array[voxels_to_try[j + 3]->type()] == 1.0f)) || voxels_to_try[j]->type() != _type)
				voxel_value[j] = DIFFERENT;
		}
		int sprite_index_delta = _type * 35;
		for (size_t j = 0; j < 3; j++)
		{
			int value_index = delta_face_index[index][j];
			dest[j] = uvs_top_face_index[face][voxel_value[0]][voxel_value[1]][voxel_value[2]][0] + value_index + sprite_index_delta;
		}
	}
}

jgl::Mesh* Voxel::construct(Board* board, Vector3 chunk_pos, jgl::Mesh* target)
{
	if (_type == -1)
		return (nullptr);

	Vector3 self_pos = _rel_pos + chunk_pos * chunk_size;
	jgl::Mesh* result;
	if (target == nullptr)
	{
		result = new jgl::Mesh(0);
	}
	else
	{
		result = target;
		result->clear();
	}

	float base_alpha = block_alpha_array[_type];

	for (size_t face = 0; face < 9; face++)
	{
		Vector3 tmp_next = self_pos + voxel_neighbour[face];
		Voxel* tmp_voxel = board->voxels(tmp_next);
		float tmp_alpha = (tmp_voxel == nullptr || tmp_voxel->type()  == -1 ? base_alpha : block_alpha_array[tmp_voxel->type()]);

		if (tmp_voxel == nullptr || tmp_voxel->type() == -1 || tmp_alpha != base_alpha)
		{
			for (size_t index = 0; index < 2; index++)
			{
				int tmp_vertices_index[3] = { -1, -1, -1 };
				int tmp_uvs_index[3] = { -1, -1, -1 };
				int tmp_normales_index[3] = { -1, -1, -1 };

				for (size_t j = 0; j < 3; j++)
				{
					Vector3 tmp_pos = _rel_pos * Vector3(1, 0, 1) + voxel_vertices[vertices_face_index[face][face_index_order[index][j]]];
					tmp_pos *= 2.0f;
					int tmp_index = static_cast<int>(tmp_pos.x + tmp_pos.z * (chunk_size.x * 2 + 1) + tmp_pos.y * ((chunk_size.x * 2 + 1) * (chunk_size.z * 2 + 1)));
					tmp_vertices_index[j] = tmp_index;
					tmp_normales_index[j] = face;
				}

				compose_face(board, chunk_pos, tmp_uvs_index, face, index);

				result->add_face(jgl::Face(tmp_vertices_index, tmp_uvs_index, tmp_normales_index));
			}
		}
	}

	return (result);
}

void Voxel::save(Vector3 delta, std::fstream& file)
{
	if (_type == -1)
		return;

	Vector3 final = _rel_pos + delta;
	file << jgl::itoa(static_cast<int>(final.x)) << "/" << jgl::itoa(static_cast<int>(final.y)) << "/" << jgl::itoa(static_cast<int>(final.z)) << "/" << _type;
	file << std::endl;
}
