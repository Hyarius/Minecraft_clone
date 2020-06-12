#ifndef ERELIA_SCENERY_H
#define ERELIA_SCENERY_H

#include "jgl.h"

#define NB_SCENERY 20

class Scenery
{
private:
	size_t _id;
	jgl::Vector3 _size;
	jgl::Mesh* _body;

public:
	Scenery(size_t p_id, jgl::Mesh* p_body, jgl::Vector3 p_size)
	{
		_id = p_id;
		_size = p_size;
		_body = p_body;
	}

	size_t id() { return (_id); }
	jgl::Vector3 size() { return (_size); }
	jgl::Mesh* body() { return (_body); }

	void render(jgl::Camera* camera, jgl::Vector3 pos)
	{
		_body->render_differed(camera, pos);
	}
};

extern Scenery* scenery_list[NB_SCENERY];

#endif