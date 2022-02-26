#include "Radio.h"
#include <iostream>

using namespace st;
using namespace sf;
using namespace std;


//----------------------------[Radio]-----------------------------

const sf::Color outline_c(85, 85, 85);
const sf::Color sellect_c(0, 122, 204);
const sf::Color fill_c(51, 51, 55);

Radio::Radio():
	pos(0, 0), size(0, 0), checked(false),
	cursor_on_object(false), pressed(false)
{
	text.setCharacterSize(11);

	fon.setOutlineThickness(1);
	fon.setOutlineColor(outline_c);
	fon.setFillColor(fill_c);
	fon.setRadius(7);

	check.setRadius(4);
	check.setFillColor(Color(153, 153, 153));

	size.y = 17;
	rePosition();
	calcSize();
}
Radio::~Radio()
{}

void Radio::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void Radio::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	rePosition();
}
sf::Vector2f Radio::getPosition() const
{
	return pos;
}

sf::Vector2f Radio::getSize() const
{
	return size;
}

void Radio::setString(const sf::String& str)
{
	text.setString(str);
	calcSize();
}
const sf::String& Radio::getString() const
{
	return text.getString();
}

void Radio::setFont(const sf::Font& font)
{
	text.setFont(font);
	calcSize();
}
const sf::Font* Radio::getFont() const
{
	return text.getFont();
}

void Radio::setCheck(bool ch)
{
	checked = ch;
}
bool Radio::getCheck() const
{
	return checked;
}

bool Radio::event(const sf::Event& event, bool& focus)
{
	Vector2f c_pos(-1, -1); //позиция курсора
	bool o_focus = focus;   //фокус до начала обработки события
	bool ret = false;

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


	///////////////////////////////////////////////////////////////////////

	//цвет рамки
	if (event.type == Event::MouseMoved ||
		event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased)
	{
		if (cursor_on_object && !o_focus)
			fon.setOutlineColor(sellect_c);
		else
			fon.setOutlineColor(outline_c);
	}

	//нажатие
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && cursor_on_object && !o_focus)
	{
		pressed = true;
	}
	//отжатие
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		if (pressed && cursor_on_object && !o_focus)
		{
			checked = true;
			ret = true;
		}

		pressed = false;
	}


	return ret;
}
void Radio::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon);
	if (checked)
		target.draw(check);
	target.draw(text);
	
}

//private
void Radio::rePosition()
{
	fon.setPosition(Vector2f(pos.x + 1, pos.y + 1));
	check.setPosition(Vector2f(pos.x + 4, pos.y + 4));
	text.setPosition(Vector2f(pos.x + 20, pos.y + 1));
}
void Radio::calcSize()
{
	size.x = 20 + text.getLocalBounds().width;
}

//----------------------------[RadioController]-----------------------------

RadioController::RadioController():
	destructor_delete_radio(false)
{}
RadioController::~RadioController()
{
	if (!destructor_delete_radio)
		return;

	for (size_t i = 0; i < arr.size(); i++)
		delete arr[i];
}

void RadioController::alignVertically(sf::Vector2f pos)
{
	for (size_t i = 0; i < arr.size(); i++)
		arr[i]->setPosition(Vector2f(pos.x, pos.y + float(20*i)));
}
void RadioController::alignHorizontally(sf::Vector2f pos)
{
	float tpos = 0;
	for (size_t i = 0; i < arr.size(); i++)
	{
		float size = arr[i]->getSize().x; 
		arr[i]->setPosition(Vector2f(pos.x + tpos, pos.y));
		tpos += size + 10;
	}
}

bool RadioController::event(const sf::Event& event, bool& focus)
{
	bool ret = false;

	for (size_t i = 0; i < arr.size(); i++)
	{
		bool set = arr[i]->event(event, focus);
		if (set == true)
		{
			ret = true;
			for (size_t i2 = 0; i2 < arr.size(); i2++) {
				arr[i2]->setCheck(i == i2);
			}
		}
	}
	return ret;
}
void RadioController::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (size_t i = 0; i < arr.size(); i++)
		target.draw(*arr[i]);
}

//----------------------------[]-----------------------------