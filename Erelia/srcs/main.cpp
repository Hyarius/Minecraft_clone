#include "erelia.h"

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2(0.8f, 0.8f));
	app.set_font_path("ressources/font/karma suture.ttf");

	Game_engine engine = Game_engine();
	engine.set_geometry(0, g_application->size());
	engine.activate();

	return (app.run());
}
