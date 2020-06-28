#include "erelia.h"

void print_pool_content()
{
	const jgl::Pool < jgl::Unique_string>& pool = jgl::Pool_object<jgl::Unique_string>::elem_pool();
	const jgl::Array < jgl::Unique_string*>& array = pool.content();

	std::cout << "Pool content : " << std::endl;
	for (size_t i = 0; i < array.size(); i++)
	{
		if (i != 0)
			std::cout << " - ";
		std::cout << "(Ref " << array[i] << ")";
	}
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	jgl::Application app = jgl::Application("Erelia", jgl::Vector2(0.8f, 0.8f));
	jgl::set_font_path("ressources/font/karma suture.ttf");

	Game_engine* engine = new Game_engine();
	engine->set_geometry(0, g_application->size());
	engine->activate();

	return (app.run());
}
