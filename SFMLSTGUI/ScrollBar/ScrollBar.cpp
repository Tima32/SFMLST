#include "ScrollBar.hpp"
#include <iostream>

using namespace std;
using namespace sf;
using namespace st;

const sf::Color progress_c(86, 91, 96);
const sf::Color fill_c(46, 52, 60);


//-------------<ScrollBarVertical>-------------//
ScrollBarVertical::ScrollBarVertical():
	pos(0, 0), size(8, 200),
	scroll_size(100),
	range(100), value(0),
	cursor_on_object(false), pressed(false)
{
	scrollRePosition();
	renderTextures();
	fon_s.setTexture(fon_t);
	scroll_s.setTexture(scroll_t);
}
ScrollBarVertical::~ScrollBarVertical()
{}

void ScrollBarVertical::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void ScrollBarVertical::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	fon_s.setPosition(pos);
	scrollRePosition();
}
sf::Vector2f ScrollBarVertical::getPosition() const
{
	return pos;
}

void ScrollBarVertical::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void ScrollBarVertical::setSize(sf::Vector2f size)
{
	size.x = 8;
	this->size = size;
	scrollRePosition();
	renderTextures();
}
sf::Vector2f ScrollBarVertical::getSize() const
{
	return size;
}

void ScrollBarVertical::setScrollSize(float size)
{
	scroll_size = size;
	scrollRePosition();
	renderTextures();
}
float ScrollBarVertical::getScrollSize() const
{
	return scroll_size;
}

void ScrollBarVertical::setRange(double range)
{
	this->range = range;
	scrollRePosition();
}
double ScrollBarVertical::getRange() const
{
	return range;
}
void ScrollBarVertical::setValue(double value)
{
	if (value > range)
		value = range;
	if (value < 0)
		value = 0;
	this->value = value;
	scrollRePosition();
}
double ScrollBarVertical::getValue() const
{
	return value;
}
double ScrollBarVertical::getProcentage() const
{
	return double(value) / double(range);
}

bool ScrollBarVertical::event(const sf::Event& event, bool& focus)
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
	//нажатие
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && cursor_on_object && !o_focus)
	{
		//перемещение ползунка к курсору
		if (!(scroll_s.getPosition().y <= c_pos.y && c_pos.y <= scroll_s.getPosition().y + scroll_s.getLocalBounds().height))
		{
			double s = (double(c_pos.y) - pos.y - scroll_size / 2) * range / (double(size.y) - scroll_size);
			setValue(s);
			ret = true;
		}

		pressed = true;
		pressed_pos = c_pos;
		move_value = value;
	}
	//отжатие
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		if (pressed && cursor_on_object && !o_focus)
		{
			double offset = (double)c_pos.y - pressed_pos.y;
			double coefficent = (double)range / ((double)size.y - scroll_size);
			setValue(move_value + (double)offset * coefficent);
			ret = true;
		}

		pressed = false;
	}
	//перемещение ползунка
	if (event.type == Event::MouseMoved && pressed)
	{
		double offset = (double)c_pos.y - pressed_pos.y;
		double coefficent = (double)range / ((double)size.y - scroll_size);
		setValue(move_value + (double)offset * coefficent);
		ret = true;
	}

	return ret;
}

void ScrollBarVertical::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon_s);
	target.draw(scroll_s);
}

//private
bool ScrollBarVertical::scrollEvent(const sf::Event& event, bool& focus)
{
	return false;
}

void ScrollBarVertical::scrollRePosition()
{
	double p = double(value) / double(range);
	scroll_s.setPosition(Vector2f(pos.x, pos.y + Uint64(p*(double(size.y)-scroll_size))));
}
void ScrollBarVertical::renderTextures()
{
	sf::CircleShape circle;
	sf::RectangleShape rectangle;

	sf::RenderTexture rt;
	rt.create(Uint32(size.x), Uint32(size.y));
	circle.setRadius(4);

	//fon
	circle.setFillColor(fill_c);
	rectangle.setFillColor(fill_c);

	rectangle.setSize(Vector2f(8, size.y - 8));
	rectangle.setPosition(Vector2f(0, 4));

	rt.clear(sf::Color::Transparent);
	rt.draw(circle);
	rt.draw(rectangle);

	circle.setPosition(Vector2f(0, size.y-8));
	rt.draw(circle);
	rt.display();

	fon_t = rt.getTexture();

	//scroll
	rt.clear(sf::Color::Transparent);
	circle.setPosition(Vector2f(0, 0));
	circle.setFillColor(progress_c);
	rt.draw(circle);

	rectangle.setSize(Vector2f(8, scroll_size - 8));
	rectangle.setFillColor(progress_c);
	rt.draw(rectangle);

	circle.setPosition(Vector2f(0, scroll_size - 8));
	rt.draw(circle);
	rt.display();

	scroll_t = rt.getTexture();

	//fon_s.setTexture(fon_t);
	fon_s.setTextureRect(IntRect(0, 0, fon_t.getSize().x, fon_t.getSize().y));
	//scroll_s.setTexture(scroll_t);
}

//--------------------<>-----------------//


//-------------<ScrollBarHorizontal>-------------//
ScrollBarHorizontal::ScrollBarHorizontal() :
	pos(0, 0), size(200, 8),
	scroll_size(100),
	range(100), value(0),
	cursor_on_object(false), pressed(false)
{
	scrollRePosition();
	renderTextures();
	fon_s.setTexture(fon_t);
	scroll_s.setTexture(scroll_t);
}
ScrollBarHorizontal::~ScrollBarHorizontal()
{}

void ScrollBarHorizontal::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void ScrollBarHorizontal::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	fon_s.setPosition(pos);
	scrollRePosition();
}
sf::Vector2f ScrollBarHorizontal::getPosition() const
{
	return pos;
}

void ScrollBarHorizontal::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void ScrollBarHorizontal::setSize(sf::Vector2f size)
{
	size.y = 8;
	this->size = size;
	scrollRePosition();
	renderTextures();
}
sf::Vector2f ScrollBarHorizontal::getSize() const
{
	return size;
}

void ScrollBarHorizontal::setScrollSize(float size)
{
	scroll_size = size;
	scrollRePosition();
	renderTextures();
}
float ScrollBarHorizontal::getScrollSize() const
{
	return scroll_size;
}

void ScrollBarHorizontal::setRange(double range)
{
	this->range = range;
	scrollRePosition();
}
double ScrollBarHorizontal::getRange() const
{
	return range;
}
void ScrollBarHorizontal::setValue(double value)
{
	if (value > range)
		value = range;
	if (value < 0)
		value = 0;
	this->value = value;
	scrollRePosition();
}
double ScrollBarHorizontal::getValue() const
{
	return value;
}
double ScrollBarHorizontal::getProcentage() const
{
	return double(value) / double(range);
}

bool ScrollBarHorizontal::event(const sf::Event& event, bool& focus)
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
	//нажатие
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && cursor_on_object && !o_focus)
	{
		//перемещение ползунка к курсору
		if (!(scroll_s.getPosition().x <= c_pos.x && c_pos.x <= scroll_s.getPosition().x + scroll_s.getLocalBounds().height))
		{
			double s = (double(c_pos.x) - pos.x - scroll_size / 2) * range / (double(size.x) - scroll_size);
			setValue(s);
			ret = true;
		}

		pressed = true;
		pressed_pos = c_pos;
		move_value = value;
	}
	//отжатие
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		if (pressed && cursor_on_object && !o_focus)
		{
			double offset = (double)c_pos.x - pressed_pos.x;
			double coefficent = (double)range / ((double)size.x - scroll_size);
			setValue(move_value + (double)offset * coefficent);
			ret = true;
		}

		pressed = false;
	}
	//перемещение ползунка
	if (event.type == Event::MouseMoved && pressed)
	{
		double offset = (double)c_pos.x - pressed_pos.x;
		double coefficent = (double)range / ((double)size.x - scroll_size);
		setValue(move_value + (double)offset * coefficent);
		ret = true;
	}

	return ret;
}

void ScrollBarHorizontal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon_s);
	target.draw(scroll_s);
}

//private
bool ScrollBarHorizontal::scrollEvent(const sf::Event& event, bool& focus)
{
	return false;
}

void ScrollBarHorizontal::scrollRePosition()
{
	double p = double(value) / double(range);
	scroll_s.setPosition(Vector2f(pos.x + Uint64(p * (double(size.x) - scroll_size)), pos.y));
}
void ScrollBarHorizontal::renderTextures()
{
	sf::CircleShape circle;
	sf::RectangleShape rectangle;

	sf::RenderTexture rt;
	rt.create(Uint32(size.x), Uint32(size.y));
	circle.setRadius(4);

	//fon
	circle.setFillColor(fill_c);
	rectangle.setFillColor(fill_c);

	rectangle.setSize(Vector2f(size.x - 8, 8));
	rectangle.setPosition(Vector2f(4, 0));

	rt.clear(sf::Color::Transparent);
	rt.draw(circle);
	rt.draw(rectangle);

	circle.setPosition(Vector2f(size.x - 8, 0));
	rt.draw(circle);
	rt.display();

	fon_t = rt.getTexture();

	//scroll
	rt.clear(sf::Color::Transparent);
	circle.setPosition(Vector2f(0, 0));
	circle.setFillColor(progress_c);
	rt.draw(circle);

	rectangle.setSize(Vector2f(scroll_size - 8, 8));
	rectangle.setFillColor(progress_c);
	rt.draw(rectangle);

	circle.setPosition(Vector2f(scroll_size - 8, 0));
	rt.draw(circle);
	rt.display();

	scroll_t = rt.getTexture();

	//fon_s.setTexture(fon_t);
	fon_s.setTextureRect(IntRect(0, 0, fon_t.getSize().x, fon_t.getSize().y));
	//scroll_s.setTexture(scroll_t);
}
//-----------------------<>----------------------//