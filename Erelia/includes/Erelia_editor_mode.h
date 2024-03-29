#ifndef ERELIA_EDITOR_MODE_H
#define ERELIA_EDITOR_MODE_H

#include "jgl.h"
#include "Erelia_game_mode.h"
#include "Erelia_debug_screen.h"
#include "Erelia_minimap.h"
#include "Erelia_editor_inventory.h"
#include "Erelia_player_controller.h"

class Editor_visualizer : public jgl::Widget
{
private:
	Player_controller* _controller = nullptr;

	jgl::Mesh* _pointer = nullptr;
	Vector3 _first_voxel;
	Vector3 _second_voxel;

public:
	Editor_visualizer(Player_controller* p_controller, jgl::Widget* p_parent);
	~Editor_visualizer()
	{
		if (_pointer != nullptr)
			delete _pointer;
	}
	void set_voxels(Vector3 p_first, Vector3 p_second);
	void recalculate();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
	void render();
};

class Editor_mode : public Game_mode
{
protected:
	class Game_engine* _engine;
	World* _world = nullptr;
	Player* _player;

	bool _dodge = false;

	jgl::Contener* _editor_contener;

	File_menu* _saver_menu;
	File_menu* _loader_menu;

	Player_controller* _controller = nullptr;
	Minimap* _minimap = nullptr;

	Editor_inventory* _editor_inventory = nullptr;

	jgl::Frame* _echap_menu_frame = nullptr;
	jgl::Button* _save_button = nullptr;
	jgl::Button* _load_button = nullptr;
	jgl::Button* _settings_button = nullptr;
	jgl::Button* _save_quit_button = nullptr;
	jgl::Button* _quit_button = nullptr;

	Debug_screen* _debug_screen = nullptr;

	Vector3 _voxel_source;
	Vector3 _voxel_target;

	Editor_visualizer* _visualizer;
	Vector3 _first_voxel;
	Vector3 _second_voxel;

	Uint32 _actual_tick;
	Uint32 _timer;
	Uint32 _delta_time;
	bool _edited;

public:
	Editor_mode(class Game_engine* p_engine, World* p_world, Player* p_player);

	bool dodge() { return (_dodge); }
	bool edited() { return (_edited); }
	void set_dodge(bool p_state) { _dodge = p_state; }
	void set_edited(bool p_state) { _edited = p_state; _timer = _actual_tick + _delta_time; }

	void create_editor_panel();
	void create_saver_panel();
	void create_loader_panel();
	void create_echap_panel();

	jgl::Contener* editor_contener() {return (_editor_contener);}
	File_menu* saver_menu() {return (_saver_menu);}
	File_menu* loader_menu() {return (_loader_menu);}

	bool voxel_raycast(Vector3 pos, Vector3 direction, Vector3* voxel_source, Vector3* voxel_target);
	void use_item_left(Vector3 A, Vector3 B, Item* item);
	void use_item_right(Vector3 A, Vector3 B, Item *item);
	Editor_inventory* editor_inventory() { return (_editor_inventory); }
	jgl::Frame* echap_menu_frame() { return (_echap_menu_frame); }
	Player_controller* controller() { return (_controller); }
	void update();
	bool handle_keyboard();
	bool handle_mouse();
	void render();
	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area);
};

#endif