#include "Table.hpp"

#include <iostream>

using namespace std;
using namespace st;
using namespace sf;

Table::Table()
{
	left_up_index.setPosition(1, 1);
	left_up_index.setOutlineThickness(1);
	left_up_index.setFillColor(intex_fill_color);
	left_up_index.setOutlineColor(intex_outline_color);

	outline.setOutlineColor(outline_color);
	outline.setOutlineThickness(1);
	outline.setFillColor(Color::Transparent);
	
	setSize(size);
	setPosition(0, 0);
	setTableSize(3, 3);
	rePos();
}
Table::~Table()
{
	for (size_t x = 0; x < line_edit_table.size(); ++x)
	{
		for (size_t y = 0; y < line_edit_table[x].size(); ++y)
		{
			delete line_edit_table[x][y];
		}
	}
	line_edit_table.clear();
}

void Table::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void Table::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	outline.setPosition(pos.x+1, pos.y+1);
	sprite.setPosition(pos);
	vertical_scroll.setPosition(pos.x + size.x - vertical_scroll.getSize().x, pos.y);
	horizontal_scroll.setPosition(pos.x, pos.y + size.y - horizontal_scroll.getSize().y);
}
sf::Vector2f Table::getPosition() const
{
	return pos;
}

void Table::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void Table::setSize(sf::Vector2f size)
{
	this->size = size;

	const float scroll_hori = horizontal_scroll_shown ? horizontal_scroll.getSize().y : 0;
	const float scroll_vert = vertical_scroll_shown ? vertical_scroll.getSize().x : 0;

	texture.create(Uint32(size.x - scroll_vert), Uint32(size.y - scroll_hori));
	sprite.setTextureRect(IntRect(0, 0, Int32(size.x - scroll_vert), Int32(size.y - scroll_hori)));
	vertical_scroll.setSize(size.x, size.y - scroll_hori);
	horizontal_scroll.setSize(size.x - scroll_vert, size.y);

	vertical_scroll.setRange(default_cell_size.y * size_table.y - size.y + default_cell_size.y + scroll_hori);
	horizontal_scroll.setRange(default_cell_size.x * size_table.x - size.x + default_cell_size.y + scroll_vert);

	outline.setSize({size.x - 2 - scroll_vert, size.y - 2 - scroll_hori });

	setPosition(pos);

	rePos();
}
sf::Vector2f Table::getSize() const
{
	return size;
}

void Table::setDefaultCellSize(float x, float y)
{
	setDefaultCellSize(Vector2f(x, y));
}
void Table::setDefaultCellSize(sf::Vector2f default_cell_size)
{
	this->default_cell_size = default_cell_size;

	for (size_t x = 0; x < size_table.x; ++x)
	{
		horizontal_index[x].setSize({ default_cell_size.x - 2, default_cell_size.y - 2 });
	}

	rePos();
}
sf::Vector2f Table::getDefaultCellSize() const
{
	return default_cell_size;
}

void Table::setFont(const sf::Font& font)
{
	this->font = &font;

	//шрифт на индексы
	for (size_t i = 0; i < horizontal_index_text.size(); ++i)
		horizontal_index_text[i].setFont(font);
	for (size_t i = 0; i < vertical_index_text.size(); ++i)
		vertical_index_text[i].setFont(font);

	//шрифт на поля
	for (size_t x = 0; x < size_table.x; ++x)
	{
		for (size_t y = 0; y < size_table.y; ++y)
		{
			line_edit_table[x][y]->setFont(font);
		}
	}

	rePos();
}

void Table::setHorizontalIngexString(size_t num, const sf::String& str)
{
	horizontal_index_text[num].setString(str);
	rePosText();
}
const sf::String& Table::getHorizontalIngexString(size_t num) const
{
	return horizontal_index_text[num].getString();
}
void Table::setVerticalIngexString(size_t num, const sf::String& str)
{
	vertical_index_text[num].setString(str);
	rePosText();
}
const sf::String& Table::getVerticalIngexString(size_t num) const
{
	return vertical_index_text[num].getString();
}

void Table::setTableSize(size_t x, size_t y)
{
	setTableSize(st::Vector2s(x, y));
}
void Table::setTableSize(st::Vector2s size_table)
{
	const size_t x_old = this->size_table.x;
	const size_t y_old = this->size_table.y;

	const size_t x_size = size_table.x;
	const size_t y_size = size_table.y;

	this->size_table = size_table;

	horizontal_index.resize(x_size);
	vertical_index.resize(y_size);

	horizontal_index_text.resize(x_size);
	vertical_index_text.resize(y_size);

	horisontal_length.resize(x_size);
	vertical_length.resize(y_size);

	for (size_t x = 0; x < line_edit_table.size(); ++x)
	{
		for (size_t y = 0; y < line_edit_table[x].size(); ++y)
		{
			//удаление лишних ячеек
			if (x < x_size && y < y_size){}
			else
			{
				delete line_edit_table[x][y];
			}
		}
		line_edit_table[x].resize(y_size, nullptr);//изменение размера по y
	}
	vector<st::LineEdit*> y_le_template;
	y_le_template.resize(y_size, nullptr);
	line_edit_table.resize(x_size, y_le_template);//изменение размера по x

	sf::RectangleShape index_r;
	index_r.setSize(Vector2f(default_cell_size.x-2, default_cell_size.y-2));
	index_r.setOutlineThickness(1);
	index_r.setFillColor(intex_fill_color);
	index_r.setOutlineColor(intex_outline_color);

	sf::Text text;
	if (font)
		text.setFont(*font);
	text.setCharacterSize(11);

	//горизонтальные индексы
	for (size_t x = x_old; x < x_size; ++x)
	{
		horizontal_index[x] = index_r;

		text.setString(toCharIndex(x)/*to_wstring(x+1)*/);
		horizontal_index_text[x] = text;

		horisontal_length[x] = default_cell_size.x;
	}

	//вертикальные индексы
	index_r.setSize(Vector2f(default_cell_size.y - 2, default_cell_size.y - 2));
	for (size_t y = y_old; y < y_size; ++y)
	{
		vertical_index[y] = index_r;

		text.setString(to_wstring(y + 1));
		vertical_index_text[y] = text;

		vertical_length[y] = default_cell_size.y;
	}

	//Таблица строк
	//line_edit_table.reserve(x_size);
	for (size_t x = 0; x < x_size; ++x)
	{
		for (size_t y = 0; y < y_size; ++y)
		{
			if (line_edit_table[x][y] != nullptr)
				continue;
			st::LineEdit* line_edit = new st::LineEdit;
			if (font)
				line_edit->setFont(*font);
			line_edit->setPosition(index_r.getSize());

			line_edit_table[x][y] = line_edit;
		}
	}

	const float scroll_hori = horizontal_scroll_shown ? horizontal_scroll.getSize().y : 0;
	const float scroll_vert = vertical_scroll_shown ? vertical_scroll.getSize().x : 0;
	vertical_scroll.setRange(default_cell_size.y * y_size - size.y + default_cell_size.y + scroll_hori);
	horizontal_scroll.setRange(default_cell_size.x * x_size - size.x + default_cell_size.y + scroll_vert);

	rePos();
}
st::Vector2s Table::getTableSize() const
{
	return size_table;
}

void Table::verticalScrollShown(bool show)
{
	vertical_scroll_shown = show;
	setSize(size);
}
void Table::horizontalScrollShown(bool show)
{
	horizontal_scroll_shown = show;
	setSize(size);
}

st::LineEdit& Table::getCell(size_t x, size_t y)
{
	return getCell(Vector2s(x, y));
}
st::LineEdit& Table::getCell(st::Vector2s pos)
{
	return *line_edit_table[pos.x][pos.y];
}

void Table::setOutlineColor(sf::Color outline_color)
{
	this->outline_color = outline_color;
	outline.setOutlineColor(outline_color);
}
sf::Color Table::getOutlineColor() const
{
	return outline_color;
}

bool Table::event(const sf::Event& event, bool& focus)
{
	bool result = false;

	bool v_scroll = false;
	bool h_scroll = false;
	if (vertical_scroll_shown)
		v_scroll = vertical_scroll.event(event, focus);
	if (horizontal_scroll_shown)
		h_scroll = horizontal_scroll.event(event, focus);

	if (v_scroll || h_scroll)
	{
		//rePos();
	}
	result |= v_scroll;
	result |= h_scroll;

	float horizontal_s = rintf(float(horizontal_scroll.getValue()));
	float vertical_s = rintf(float(vertical_scroll.getValue()));
	sf::Event event_moved = event;
	if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased)
	{
		event_moved.mouseButton.x += int(horizontal_s - pos.x);
		event_moved.mouseButton.y += int(vertical_s - pos.y);
	}
	else if (event.type == Event::MouseMoved)
	{
		event_moved.mouseMove.x += int(horizontal_s - pos.x);
		event_moved.mouseMove.y += int(vertical_s - pos.y);
	}

	for (size_t x = 0; x < size_table.x; ++x)
	{
		for (size_t y = 0; y < size_table.y; ++y)
		{
			result |= line_edit_table[x][y]->event(event_moved, focus);
		}
	}

	return result;
}

void Table::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	texture.clear(Color::Transparent);
	//texture.clear(Color::Red);

	float horizontal_s = rintf(float(horizontal_scroll.getValue()));
	float vertical_s = rintf(float(vertical_scroll.getValue()));

	sf::View save_view = texture.getView();
	sf::View table_view = save_view;
	table_view.setCenter(save_view.getCenter().x + horizontal_s, save_view.getCenter().y + vertical_s);
	texture.setView(table_view);

	for (size_t x = 0; x < size_table.x; ++x)
	{
		for (size_t y = 0; y < size_table.y; ++y)
		{
			if (0 <= line_edit_table[x][y]->getPosition().x + line_edit_table[x][y]->getSize().x - horizontal_s && line_edit_table[x][y]->getPosition().x - horizontal_s < size.x &&
				0 <= line_edit_table[x][y]->getPosition().y + line_edit_table[x][y]->getSize().y - vertical_s && line_edit_table[x][y]->getPosition().y - vertical_s < size.y)
			{
				texture.draw(*line_edit_table[x][y]);
			}
		}
	}

	table_view.setCenter(save_view.getCenter().x + horizontal_s, save_view.getCenter().y);
	texture.setView(table_view);
	for (size_t x = 0; x < horizontal_index.size(); ++x)
	{
		texture.draw(horizontal_index[x]);
		texture.draw(horizontal_index_text[x]);
	}

	table_view.setCenter(save_view.getCenter().x, save_view.getCenter().y + vertical_s);
	texture.setView(table_view);
	for (size_t y = 0; y < vertical_index.size(); ++y)
	{
		texture.draw(vertical_index[y]);
		texture.draw(vertical_index_text[y]);
	}
	texture.setView(save_view);

	texture.draw(left_up_index);

	texture.display();

	sprite.setTexture(texture.getTexture());
	target.draw(sprite);
	
	target.draw(outline);
	if (vertical_scroll_shown)
		target.draw(vertical_scroll);
	if (horizontal_scroll_shown)
	target.draw(horizontal_scroll);
}

//private
void Table::rePos()
{
	float x_size = default_cell_size.x;
	float y_size = default_cell_size.y;

	left_up_index.setSize(Vector2f(default_cell_size.y-2, default_cell_size.y -2));

	//Горизонтальные индексы
	for (size_t x = 0; x < horizontal_index.size(); ++x)
	{
		if (x == 0)
			horizontal_index[x].setPosition(left_up_index.getSize().x + 3/* - rintf(horizontal_scroll.getValue())*/, 1);
		else
			horizontal_index[x].setPosition(horizontal_index[x-1].getPosition().x + horizontal_index[x - 1].getSize().x + 2, 1);

		//horizontal_index_text[x].setPosition(rintf(horizontal_index[x].getPosition().x + horizontal_index[x].getSize().x/2 - horizontal_index_text[x].getLocalBounds().width/2), horizontal_index[x].getPosition().y + 1);
	}

	//Вуртикальные индексы
	for (size_t y = 0; y < vertical_index.size(); ++y)
	{
		if (y == 0)
			vertical_index[y].setPosition(1, left_up_index.getSize().y + 3/* - rintf(vertical_scroll.getValue())*/);
		else
			vertical_index[y].setPosition(1, vertical_index[y - 1].getPosition().y + vertical_index[y - 1].getSize().y + 2);

		//vertical_index_text[y].setPosition(rintf(vertical_index[y].getPosition().x + vertical_index[y].getSize().x / 2 - vertical_index_text[y].getLocalBounds().width / 2), vertical_index[y].getPosition().y + 1);
	}
	rePosText();

	//Таблица строк
	for (size_t x = 0; x < size_table.x; ++x)
	{
		for (size_t y = 0; y < size_table.y; ++y)
		{
			line_edit_table[x][y]->setPosition(horizontal_index[x].getPosition().x - 1, vertical_index[y].getPosition().y - 1);
			line_edit_table[x][y]->setSize(horizontal_index[x].getSize().x + 2);
		}
	}
}
void Table::rePosText()
{
	for (size_t x = 0; x < horizontal_index.size(); ++x)
	{
		horizontal_index_text[x].setPosition(rintf(horizontal_index[x].getPosition().x + horizontal_index[x].getSize().x / 2 - horizontal_index_text[x].getLocalBounds().width / 2), horizontal_index[x].getPosition().y + 1);
	}

	for (size_t y = 0; y < vertical_index.size(); ++y)
	{
		vertical_index_text[y].setPosition(rintf(vertical_index[y].getPosition().x + vertical_index[y].getSize().x / 2 - vertical_index_text[y].getLocalBounds().width / 2), vertical_index[y].getPosition().y + 1);
	}
}

sf::String Table::toCharIndex(size_t num) const
{
	const size_t base = L'z' - L'a' + 1;
	wchar_t buffer[256];
	
	errno_t error;
	error = _ui64tow_s(num, buffer, 256, base);

	if (error)
		return String();

	for (size_t i = 0; buffer[i] != L'\0'; ++i)
	{
		if (L'0' <= buffer[i] && buffer[i] <= L'9')
		{
			buffer[i] += L'a' - L'0';
		}
		else if (L'a' <= buffer[i] && buffer[i] <= L'z')
		{
			buffer[i] += 10;
		}
		else
		{
			cout << "<st:Table:toCharIndex>Error: out of range." << endl;
		}

		buffer[i] = buffer[i] + (L'A' - L'a');
		if (i == 0 && buffer[i + 1] != L'\0')
			buffer[i] -= 1;
	}
	
	return String(buffer);
}