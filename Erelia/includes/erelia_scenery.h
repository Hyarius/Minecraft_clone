#ifndef ERELIA_SCENERY_H
#define ERELIA_SCENERY_H

#include "jgl.h"

class Scenery
{
private:
	size_t _id = 0;
	jgl::Vector3 _size = {1, 1, 1};
	jgl::Mesh* _body = nullptr;

public:
	Scenery(size_t p_id, jgl::Mesh* p_body, jgl::Vector3 p_size)
	{
		_id = p_id;
		_size = p_size;
		_body = p_body;
	}
	~Scenery()
	{
		if (_body != nullptr)
			delete _body;
	}

	size_t id() { return (_id); }
	jgl::Vector3 size() { return (_size); }
	jgl::Mesh* body() { return (_body); }

	void render(jgl::Camera* camera, jgl::Vector3 pos, const jgl::Viewport* viewport)
	{
		_body->render_differed(camera, pos, viewport);
	}
};


#endif