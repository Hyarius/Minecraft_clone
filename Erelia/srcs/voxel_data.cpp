#include "erelia.h"

float block_alpha_array[12] = {
	1.0f,
	1.0f,
	0.5f,
	1.0f,
	1.0f,
	1.0f,
	0.5f,
	1.0f,
	1.0f,
	1.0f,
	0.5f,
	1.0f
};

Vector3 voxel_vertices[13]{
	{0.0f, 0.0f, 0.0f}, // A
	{1.0f, 0.0f, 0.0f}, // B
	{0.0f, 0.0f, 1.0f}, // C
	{1.0f, 0.0f, 1.0f}, // D
	{0.0f, 1.0f, 0.0f}, // E
	{0.5f, 1.0f, 0.0f}, // F
	{0.0f, 1.0f, 0.5f}, // G
	{0.5f, 1.0f, 0.5f}, // H
	{1.0f, 1.0f, 0.0f}, // I
	{1.0f, 1.0f, 0.5f}, // J
	{0.0f, 1.0f, 1.0f}, // K
	{0.5f, 1.0f, 1.0f}, // L
	{1.0f, 1.0f, 1.0f}, // M
};

int vertices_face_index[9][4]{
	{7, 6, 5, 4}, // Up down left
	{9, 7, 8, 5}, // Up down right
	{12, 11, 9, 7}, // Up top right
	{11, 10, 7, 6}, // Up top left
	{1, 0, 3, 2}, // Down
	{8, 4, 1, 0}, // Front
	{10, 12, 2, 3}, // Back
	{12, 8, 3, 1}, // Right
	{4, 10, 0, 2}, // Left
};

Vector3 voxel_neighbour[9]{
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, -1, 0},
	{0, 0, -1},
	{0, 0, 1},
	{1, 0, 0},
	{-1, 0, 0}
};

int face_index_order[2][3]{
	{0, 1, 2},
	{2, 1, 3}
};

Vector2 voxel_uv[35]{
	{0, 0},
	{1, 0},
	{2, 0},
	{3, 0},
	{4, 0},
	{0, 1},
	{1, 1},
	{2, 1},
	{3, 1},
	{4, 1},
	{0, 2},
	{1, 2},
	{2, 2},
	{3, 2},
	{4, 2},
	{0, 3},
	{1, 3},
	{2, 3},
	{3, 3},
	{4, 3},
	{0, 4},
	{1, 4},
	{2, 4},
	{3, 4},
	{4, 4},
	{0, 5},
	{1, 5},
	{2, 5},
	{3, 5},
	{4, 5},
	{0, 6},
	{1, 6},
	{2, 6},
	{3, 6},
	{4, 6}
};

Vector3 voxel_normales[9]{
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, 1, 0},
	{0, -1, 0},
	{0, 0, -1},
	{0, 0, 1},
	{1, 0, 0},
	{-1, 0, 0}
};

std::vector<Vector2> uv_type_delta = {
	{0, 0},
	{4, 0},
	{8, 0},
	{12, 0},
	{0, 6},
	{4, 6},
	{8, 6},
	{12, 12},
	{0, 12},
	{4, 12},
	{8, 12},
	{12, 12},
};

int  uvs_face_index[9][4] = {
	{22, 32, 20, 30}, // Up down left
	{24, 34, 22, 32}, // Up down right
	{14, 24, 12, 22}, // Up top right
	{12, 22, 10, 20}, // Up top left
	{2, 12, 0, 10}, // Down
	{2, 12, 0, 10}, // Front
	{2, 12, 0, 10}, // Back
	{2, 12, 0, 10}, // Right
	{2, 12, 0, 10}, // Left
};

int uvs_top_face_index[4][2][2][2][1]{
	{ // Up down left
		{ // A same type
			{// B same type
				{ // C same type
					22
				},
				{ // C different type
					21
				},
			},
			{ // B different type
				{ // C same type
					8
				},
				{ // C different type
					21
				},
			},
		},
		{ // A different type
			{// B same type
				{ // C same type
					27
				},
				{ // C different type
					26
				},
			},
			{ // B different type
				{ // C same type
					27
				},
				{ // C different type
					26
				},
			},
		},
	},
	{// Up down right
		{ // A same type
			{// B same type
				{ // C same type
					23
				},
				{ // C different type
					28
				},
			},
			{ // B different type
				{ // C same type
					9
				},
				{ // C different type
					28
				},
			},
		},
		{ // A different type
			{// B same type
				{ // C same type
					24
				},
				{ // C different type
					29
				},
			},
			{ // B different type
				{ // C same type
					24
				},
				{ // C different type
					29
				},
			},
		},
	},
	{// Up top right
		{ // A same type
			{// B same type
				{ // C same type
					18
				},
				{ // C different type
					19
				},
			},
			{ // B different type
				{ // C same type
					4
				},
				{ // C different type
					19
				},
			},
		},
		{ // A different type
			{// B same type
				{ // C same type
					13
				},
				{ // C different type
					14
				},
			},
			{ // B different type
				{ // C same type
					13
				},
				{ // C different type
					14
				},
			},
		},
	},
	{// Up top left
		{ // A same type
			{// B same type
				{ // C same type
					17
				},
				{ // C different type
					12
				},
			},
			{ // B different type
				{ // C same type
					3
				},
				{ // C different type
					12
				},
			},
		},
		{ // A different type
			{// B same type
				{ // C same type
					16
				},
				{ // C different type
					11
				},
			},
			{ // B different type
				{ // C same type
					16
				},
				{ // C different type
					11
				},
			},
		},
	},
};

Vector3 neightbour_compose_face[4][3]{
	{// Up down left
		{-1, 0, 0}, // Voxel A
		{-1, 0, -1}, // Voxel B
		{0, 0, -1}, // Voxel C
	},
	{// Up down right
		{0, 0, -1}, // Voxel A
		{1, 0, -1}, // Voxel B
		{1, 0, 0}, // Voxel C
	},
	{// Up top right
		{1, 0, 0}, // Voxel A
		{1, 0, 1}, // Voxel B
		{0, 0, 1}, // Voxel C
	},
	{// Up top left
		{0, 0, 1}, // Voxel A
		{-1, 0, 1}, // Voxel B
		{-1, 0, 0}, // Voxel C
	},
};

int delta_face_index[2][3]{
	{0, -1, 5},
	{5, -1, 4}
};

