#include "erelia.h"

Scenery* scenery_list[NB_SCENERY];

jgl::String scenery_path_list[NB_SCENERY] =
{
	"ressources/model/grass-forest1.obj",
	"ressources/model/grass-forest2.obj",
	"ressources/model/grass-forest3.obj",
	"ressources/model/grass-forest4.obj",
	"ressources/model/grass-forest5.obj",
	"ressources/model/grass-forest6.obj",
	"ressources/model/grass-forest7.obj",
	"ressources/model/grass-forest8.obj",
	"ressources/model/grass-forest9.obj",
	"ressources/model/grass-forest10.obj",
	"ressources/model/grass-montain1.obj",
	"ressources/model/grass-montain2.obj",
	"ressources/model/grass-montain3.obj",
	"ressources/model/grass-montain4.obj",
	"ressources/model/grass-montain5.obj",
	"ressources/model/grass-montain6.obj",
	"ressources/model/grass-montain7.obj",
	"ressources/model/grass-montain8.obj",
	"ressources/model/grass-montain9.obj",
	"ressources/model/grass-montain10.obj",
};

jgl::Vector3 scenery_delta_pos[NB_SCENERY] = {
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
	jgl::Vector3(0.5f, 1.0f, 0.5f),
};

jgl::Vector3 scenery_start_size[NB_SCENERY] = {
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 2, 1),
	jgl::Vector3(1, 2, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 1, 1),
	jgl::Vector3(1, 2, 1),
	jgl::Vector3(1, 2, 1),
};

void create_scenery_list()
{
	for (size_t i = 0; i < NB_SCENERY; i++)
	{
		scenery_list[i] = new Scenery(i, new jgl::Mesh(scenery_path_list[i], scenery_delta_pos[i], 0, 1), scenery_start_size[i]);
	}
}

void delete_scenery_list()
{
	for (size_t i = 0; i < NB_SCENERY; i++)
	{
		delete scenery_list[i];
	}
}