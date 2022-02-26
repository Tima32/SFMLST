#include "ProgressBar.hpp"

using namespace st;
using namespace sf;

const sf::Color outline_c(85, 85, 85);
const sf::Color progress_c(0, 122, 204);
const sf::Color fill_c(51, 51, 55);

ProgressBar::ProgressBar():
	pos(0, 0), size(200, 17),
	range(100), value(0),
	text_mode(TextMode::None)
{
	fon.setOutlineThickness(1);
	fon.setOutlineColor(outline_c);
	fon.setFillColor(fill_c);

	progress.setFillColor(progress_c);

	text.setCharacterSize(11);

	setPosition(pos);
	setSize(size);
}
ProgressBar::~ProgressBar()
{}

void ProgressBar::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void ProgressBar::setPosition(sf::Vector2f pos)
{
	this->pos = pos;

	fon.setPosition(pos.x + 1, pos.y + 1);
	progress.setPosition(pos.x + 1, pos.y + 1);
	updateTextPos();
}
const sf::Vector2f& ProgressBar::getPosition() const
{
	return pos;
}

void ProgressBar::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void ProgressBar::setSize(sf::Vector2f size)
{
	this->size = size;

	reSize();
}
const sf::Vector2f& ProgressBar::getSize() const
{
	return size;
}

void ProgressBar::setRange(size_t range)
{
	this->range = range;
	reSize();
	updateText();
}
size_t ProgressBar::getRange() const
{
	return range;
}
void ProgressBar::setValue(size_t value)
{
	if (value > range)
		value = range;

	this->value = value;
	reSize();
	updateText();
}
size_t ProgressBar::getValue() const
{
	return value;
}
double ProgressBar::getProcentage() const
{
	return double(range) / double(value);
}

void ProgressBar::setTextMode(TextMode mode)
{
	text_mode = mode;
	reSize();
	updateText();
}
ProgressBar::TextMode ProgressBar::getTextMode() const
{
	return text_mode;
}
void ProgressBar::setFont(const sf::Font& font)
{
	text.setFont(font);
	updateText();
}

bool ProgressBar::event(const sf::Event& event, bool& focus)
{
	Vector2f c_pos(-1, -1); //позиция курсора
	bool o_focus = focus;   //фокус до начала обработки события

	//////////////////////////курсор на объекте?///////////////////////
	//события кнопок
	if (event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased)
	{
		c_pos = Vector2f(float(event.mouseButton.x), float(event.mouseButton.y));
		//На кнопке
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;
	}
	//событие перемещения
	if (event.type == Event::MouseMoved)
	{
		c_pos = Vector2f(float(event.mouseMove.x), float(event.mouseMove.y));
		//На объекте
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;
	}

	//установка фокуса
	if (!focus)
	{
		focus = cursor_on_object;
	}

	//цвет рамки
	if (event.type == Event::MouseMoved ||
		event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased)
	{
		if (cursor_on_object && !o_focus)
			fon.setOutlineColor(progress_c);
		else
			fon.setOutlineColor(outline_c);
	}


	return false;
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon);
	target.draw(progress);
	target.draw(text);
}

//private
float ProgressBar::getProgressSize()
{
	return (size.x - 2) * float(double(value) / double(range));
}
void ProgressBar::reSize()
{
	fon.setSize(Vector2f(size.x - 2, size.y - 2));
	progress.setSize(Vector2f(getProgressSize(), size.y - 2));
	updateTextPos();
}
void ProgressBar::updateText()
{
	if (text_mode == TextMode::None)
		text.setString(L"");
	else if (text_mode == TextMode::Value)
		text.setString(std::to_string(value) + "/" + std::to_string(range));
	else if (text_mode == TextMode::Procentage)
	{
		char buf[16];
		sprintf_s(buf, 16, "%.1f", double(value) / double(range) * 100);
		text.setString(buf);
	}

	//updateTextPos();
}
void ProgressBar::updateTextPos()
{
	float x = float(Int64(pos.x + size.x / 2 - text.getLocalBounds().width/2));
	text.setPosition(Vector2f(x, pos.y + 1));
}