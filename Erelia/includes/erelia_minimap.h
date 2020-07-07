#ifndef ERELIA_MINIMAP_H
#define ERELIA_MINIMAP_H

#include "jgl.h"

class Minimap : public jgl::Widget
{
private:
	World* _world = nullptr;
	Player* _player;
	jgl::Camera* _camera = nullptr;

public:
	Minimap(World* p_world, Player* p_player, jgl::Widget* p_parent = nullptr);

	jgl::Camera* camera() { return (_camera); }

	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
	void render();
};

#endif