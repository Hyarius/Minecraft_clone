#include "erelia.h"

void validate_save(jgl::Data param)
{
	Editor_mode *editor = param.acces<Editor_mode*>(0);
	Board* board = param.acces<Board*>(1);
	jgl::String base_path = editor->saver_menu()->path() + "\\";
	jgl::String extension = editor->saver_menu()->extension();
	jgl::String path = base_path + param.acces<jgl::Text_entry*>(2)->text() + (extension == "*" ? "" : extension);

	std::fstream output_file = open_file(path, std::ios_base::out);
	board->save(output_file);
	output_file.close();

	if (editor != nullptr)
	{
		editor->editor_contener()->activate();
		editor->saver_menu()->desactivate();
		editor->echap_menu_frame()->desactivate();
		editor->editor_inventory()->set_shortcut_frozen(false);
	}
}

void cancel_save(jgl::Data param)
{
	Editor_mode* editor = param.acces<Editor_mode*>(0);

	if (editor != nullptr)
	{
		editor->editor_contener()->activate();
		editor->saver_menu()->desactivate();
		editor->echap_menu_frame()->desactivate();
		editor->editor_inventory()->set_shortcut_frozen(false);
	}
}

void Editor_mode::create_saver_panel()
{
	_saver_menu = new File_menu("ressources/maps", ".map", _contener);
	_saver_menu->set_valid_interation(validate_save, jgl::Data(3, this, _board, _saver_menu->entry()));
	_saver_menu->set_cancel_interation(cancel_save, this);
}