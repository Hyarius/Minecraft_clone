#include "erelia.h"

void validate_load(jgl::Data param)
{
	Editor_mode* editor = param.acces<Editor_mode*>(0);
	Board* board = param.acces<Board*>(1);
	jgl::String base_path = editor->loader_menu()->path() + "\\";
	jgl::String extension = editor->loader_menu()->extension();
	jgl::String path = base_path + param.acces<jgl::Text_entry*>(2)->text() + (extension == "*" ? "" : extension);

	std::cout << "Load board from path [" << path << "]" << std::endl;
	board->reload(path);
	if (editor != nullptr)
	{
		editor->editor_contener()->activate();
		editor->loader_menu()->desactivate();
		editor->echap_menu_frame()->desactivate();
		editor->editor_inventory()->set_shortcut_frozen(false);
	}
}

void cancel_load(jgl::Data param)
{
	Editor_mode* editor = param.acces<Editor_mode*>(0);

	if (editor != nullptr)
	{
		editor->editor_contener()->activate();
		editor->loader_menu()->desactivate();
		editor->echap_menu_frame()->desactivate();
		editor->editor_inventory()->set_shortcut_frozen(false);
	}
}

void Editor_mode::create_loader_panel()
{
	_loader_menu = new File_menu("ressources/maps", ".map", _contener);
	_loader_menu->set_valid_interation(validate_load, jgl::Data(3, this, _board, _loader_menu->entry()));
	_loader_menu->set_cancel_interation(cancel_load, this);
}