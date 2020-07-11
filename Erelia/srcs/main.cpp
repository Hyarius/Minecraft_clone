#include "erelia.h"

Game_engine* g_engine;

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2(0.8f, 0.8f));
	jgl::set_font_path("ressources/font/karma suture.ttf");

	g_engine = new Game_engine();
	g_engine->set_geometry(0, g_application->size());
	g_engine->activate();

	return (app.run());
}
