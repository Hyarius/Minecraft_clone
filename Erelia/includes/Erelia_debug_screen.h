#ifndef ERELIA_DEBUG_SCREEN_H
#define ERELIA_DEBUG_SCREEN_H

#include "jgl.h"

using jgl::Vector2;
using jgl::Vector3;
using jgl::Vector4;

struct Debug_line
{
	jgl::String content = "";
	jgl::alignment align = jgl::alignment::left;
	jgl::text_color text_color = jgl::text_color::white;
	int line = 0;

	jgl::Image* text_image;

	Debug_line()
	{
	
	}
	Debug_line(jgl::String p_content) : Debug_line(p_content, jgl::alignment::left, jgl::text_color::white, -1)
	{
	
	}
	Debug_line(jgl::String p_content, jgl::alignment p_align, jgl::text_color p_text_color = jgl::text_color::white, int p_line = -1)
	{
		content = jgl::String(p_content);
		align = p_align;
		text_color = p_text_color;
		line = p_line;
		text_image = nullptr;
	}
	void set_text(jgl::String p_content)
	{
		content = jgl::String(p_content);
		if (text_image != nullptr)
			delete text_image;
		text_image = nullptr;
	}
	void create_text_image(size_t size)
	{
		text_image = get_string_image(content, size, 0, text_color);
	}
};

class Debug_screen : public jgl::Widget
{
private:
	std::vector<Debug_line> _to_print;

	std::vector<Debug_line *> _line_order[3];

	Vector2 _begin_pos = {20, 20};
	size_t _text_size = 16;
public:
	Debug_screen(jgl::Widget* p_parent = nullptr) : jgl::Widget(p_parent)
	{
		_to_print.clear();
	}

	void calc_line_order()
	{
		for (size_t i = 0; i < 3; i++)
			_line_order[i].clear();
		for (size_t i = 0; i < _to_print.size(); i++)
		{
			int part = static_cast<int>(_to_print[i].align);
			size_t tmp_line = (_to_print[i].line == -1 ? 0 : _to_print[i].line);

			while (tmp_line < _line_order[part].size() && _line_order[part][tmp_line] != nullptr)
				tmp_line++;
			while (tmp_line >= _line_order[part].size())
				_line_order[part].push_back(nullptr);
			_line_order[part][tmp_line] = &(_to_print[i]);
		}
	}

	void add_line(Debug_line line)
	{
		_to_print.push_back(line);
		calc_line_order();
	}

	void set_text(size_t index, jgl::String text)
	{
		if (index >= _to_print.size())
		{
			Debug_line tmp = Debug_line(text);
			add_line(tmp);
		}
		else
		{
			_to_print[index].set_text(text);
		}
	}

	void set_text(size_t index, jgl::String text, jgl::alignment align)
	{
		int part = static_cast<int>(align);
		if (index >= _line_order[part].size() || _line_order[part][index] == nullptr)
		{
			Debug_line tmp = Debug_line(text, align);
			add_line(tmp);
		}
		else
		{
			_line_order[part][index]->set_text(text);
		}
	}

	void set_text_size(size_t p_text_size) { _text_size = p_text_size; }

	int calc_line_heigth(int heigth)
	{
		return (static_cast<int>(_begin_pos.y) + heigth * (_text_size + (heigth == 0 ? 0 : _text_size / 2)));
	}

	void set_geometry_imp(Vector2 p_anchor, Vector2 p_area)
	{
		
	}
	void render()
	{
		for (size_t type = 0; type < 3; type++)
		{
			for (size_t i = 0; i < _line_order[type].size(); i++)
			{
				if (_line_order[type][i] != nullptr)
				{
					if (_line_order[type][i]->text_image == nullptr)
						_line_order[type][i]->create_text_image(_text_size);
					Vector2 pos = _begin_pos + Vector2(0, calc_line_heigth(i));
					if (_line_order[type][i]->align != jgl::alignment::left)
					{
						int len = static_cast<int>(_line_order[type][i]->text_image->size().x);
						pos.x = (_line_order[type][i]->align == jgl::alignment::right ? g_application->size().x - _begin_pos.x - len : g_application->size().x / 2 - len / 2);
					}
					_line_order[type][i]->text_image->draw(pos, _line_order[type][i]->text_image->size(), 1.0f, _viewport);
				}
			}
		}
	}
};

#endif