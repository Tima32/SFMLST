#include "Slider.hpp"
#include <iostream>
#include <charconv>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


using namespace std;
using namespace sf;
using namespace st;


const sf::Color outline_c(85, 85, 85);
const sf::Color scroll_c(86, 91, 96);
const sf::Color fill_c(51, 51, 55);


//--------------------<SliderHorizontal>------------------//
SliderHorizontal::SliderHorizontal():
	pos(0, 0), size(200, 17),
	slider_size(7),
	range(100), value(0),
	cursor_on_object(false), pressed(false),
	text_mode(TextMode::None)
{
	fon.setFillColor(fill_c);
	slider.setFillColor(scroll_c);
	slider.setSize(Vector2f(slider_size, 13));

	text_t.setCharacterSize(11);

	setPosition(pos);
	setSize(size);
}
SliderHorizontal::~SliderHorizontal()
{}

void SliderHorizontal::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void SliderHorizontal::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	fon.setPosition(pos);
	sliderRePosition();
	textUpdatePos();
}
sf::Vector2f SliderHorizontal::getPosition() const
{
	return pos;
}

//set ignored y size
void SliderHorizontal::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void SliderHorizontal::setSize(sf::Vector2f size)
{
	fon.setSize(size);
	sliderRePosition();
	textUpdatePos();
}
sf::Vector2f SliderHorizontal::getSize() const
{
	return size;
}

void SliderHorizontal::setSliderSize(float size)
{
	slider_size = size;
	slider.setSize(Vector2f(slider_size, 15));
	sliderRePosition();
}
float SliderHorizontal::getSliderSize() const
{
	return slider_size;
}

void SliderHorizontal::setRange(double range)
{
	this->range = range;
	sliderRePosition();
	textUpdateText();
	textUpdatePos();
}
double SliderHorizontal::getRange() const
{
	return range;
}
void SliderHorizontal::setValue(double value)
{
	if (value > range)
		value = range;
	if (value < 0)
		value = 0;
	this->value = value;
	sliderRePosition();
	textUpdateText();
	textUpdatePos();
}
double SliderHorizontal::getValue() const
{
	return value;
}
double SliderHorizontal::getProcentage() const
{
	return value / range;
}

void SliderHorizontal::TextMode::None(sf::String& out_str, const SliderHorizontal& sh)
{
	out_str.clear();
}
void SliderHorizontal::TextMode::Procentage(sf::String& out_str, const SliderHorizontal& sh)
{
	char buf[16];
	sprintf_s(buf, 16, "%.1lf", double(sh.getValue()) / double(sh.getRange()) * 100);
	out_str = buf;
}
void SliderHorizontal::TextMode::Value(sf::String& out_str, const SliderHorizontal& sh)
{
	char buf[16];
	sprintf_s(buf, 16, "%.1lf", sh.getValue());
	out_str = buf;
}
void SliderHorizontal::setTextMode(TextModeT tm)
{
	text_mode = tm;
	textUpdateText();
	textUpdatePos();
}
void SliderHorizontal::setFont(const sf::Font& font)
{
	text_t.setFont(font);
	textUpdatePos();
}
const sf::Font* SliderHorizontal::getFont() const
{
	return text_t.getFont();
}

bool SliderHorizontal::event(const sf::Event& event, bool& focus)
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
		if (!(slider.getPosition().x <= c_pos.x && c_pos.x <= slider.getPosition().x + slider.getLocalBounds().height))
		{
			double s = (double(c_pos.x) - pos.x - slider_size / 2) * range / (double(size.x) - slider_size);
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
			double coefficent = (double)range / ((double)size.x - 4 - slider_size);
			setValue(move_value + (double)offset * coefficent);
			ret = true;
		}

		pressed = false;
	}
	//перемещение ползунка
	if (event.type == Event::MouseMoved && pressed)
	{
		double offset = (double)c_pos.x - pressed_pos.x;
		double coefficent = (double)range / ((double)size.x - 4 - slider_size);
		setValue(move_value + (double)offset * coefficent);
		ret = true;
	}

	return ret;
}

void SliderHorizontal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon);
	target.draw(slider);
	target.draw(text_t);
}

//private
void SliderHorizontal::sliderRePosition()
{
	// +- 2 поправка на рамку
	double p = double(value) / double(range);
	slider.setPosition(Vector2f(pos.x + 2 + Uint64(p * (double(size.x) - 4 - slider_size)), pos.y + 2));
}
void SliderHorizontal::textUpdateText()
{
	text_mode(text_s, *this);
	text_t.setString(text_s);
}
void SliderHorizontal::textUpdatePos()
{
	float x = float(Int64(pos.x + size.x / 2 - text_t.getLocalBounds().width / 2));
	text_t.setPosition(Vector2f(x, pos.y + 1));
}
//---------------------------<>--------------------//



//--------------------<SliderEditHorizontal>------------------//
SliderEditHorizontal::SliderEditHorizontal() :
	pos(0, 0), size(200, 17),
	slider_size(7),
	min(0), max(100), value(0),
	cursor_on_object(false), pressed(false), enter(false),
	precision(1), text_mode(TextMode::None)
{
	fon.setFillColor(fill_c);
	slider.setFillColor(scroll_c);
	slider.setSize(Vector2f(slider_size, 13));

	text_t.setCharacterSize(11);

#ifdef _WIN32
	double_click_time = GetDoubleClickTime();
#else
	double_click_time = 620;
#endif // _WIN32

	setPosition(pos);
	setSize(size);
}
SliderEditHorizontal::~SliderEditHorizontal()
{}

void SliderEditHorizontal::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void SliderEditHorizontal::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	fon.setPosition(pos);
	line_edit.setPosition(pos);
	sliderRePosition();
	textUpdatePos();
}
sf::Vector2f SliderEditHorizontal::getPosition() const
{
	return pos;
}

//set ignored y size
void SliderEditHorizontal::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void SliderEditHorizontal::setSize(sf::Vector2f size)
{
	fon.setSize(size);
	line_edit.setSize(size);
	sliderRePosition();
	textUpdatePos();
}
sf::Vector2f SliderEditHorizontal::getSize() const
{
	return size;
}

void SliderEditHorizontal::setSliderSize(float size)
{
	slider_size = size;
	slider.setSize(Vector2f(slider_size, 15));
	sliderRePosition();
}
float SliderEditHorizontal::getSliderSize() const
{
	return slider_size;
}

void SliderEditHorizontal::setMin(double min)
{
	this->min = min;
	sliderRePosition();
	textUpdateText();
	textUpdatePos();
}
double SliderEditHorizontal::getMin() const
{
	return min;
}
void SliderEditHorizontal::setMax(double max)
{
	this->max = max;
	sliderRePosition();
	textUpdateText();
	textUpdatePos();
}
double SliderEditHorizontal::getMax() const
{
	return max;
}
void SliderEditHorizontal::setValue(double value)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;
	this->value = value;
	sliderRePosition();
	textUpdateText();
	textUpdatePos();
}
double SliderEditHorizontal::getValue() const
{
	return value;
}
double SliderEditHorizontal::getProcentage() const
{
	return value / (max - min);
}

void SliderEditHorizontal::TextMode::None(sf::String& out_str, const SliderEditHorizontal& sh)
{
	out_str.clear();
}
void SliderEditHorizontal::TextMode::Procentage(sf::String& out_str, const SliderEditHorizontal& sh)
{
	char buf[256];
	buf[0] = 0;
	std::to_chars(buf, buf + sizeof(buf), 
		double(sh.getValue() - sh.getMin()) / double(sh.getMax() - sh.getMin()) * 100, 
		chars_format::fixed, sh.getTextPrecision()).ptr[0] = 0;
	out_str = buf;
}
void SliderEditHorizontal::TextMode::Value(sf::String& out_str, const SliderEditHorizontal& sh)
{
	char buf[256];
	buf[0] = 0;
	to_chars(buf, buf + sizeof(buf), sh.getValue(), chars_format::fixed, sh.getTextPrecision()).ptr[0] = 0;
	out_str = buf;
}
void SliderEditHorizontal::setTextMode(TextModeT tm)
{
	text_mode = tm;
	textUpdateText();
	textUpdatePos();
}
void SliderEditHorizontal::setTextPrecision(int precision)
{
	this->precision = precision;
	textUpdateText();
	textUpdatePos();
}
int SliderEditHorizontal::getTextPrecision()const
{
	return precision;
}
void SliderEditHorizontal::setFont(const sf::Font& font)
{
	text_t.setFont(font);
	line_edit.setFont(font);
	textUpdatePos();
}
const sf::Font* SliderEditHorizontal::getFont() const
{
	return text_t.getFont();
}

//callbacks
void SliderEditHorizontal::setChangeValueEndCallback(ChangeValueEndCallback change_value_end_callback)
{
	this->change_value_end_callback = change_value_end_callback;
}

bool SliderEditHorizontal::event(const sf::Event& event, bool& focus)
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
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && cursor_on_object && !o_focus && !enter)
	{
		//перемещение ползунка к курсору
		if (!(slider.getPosition().x <= c_pos.x && c_pos.x <= slider.getPosition().x + slider.getLocalBounds().height))
		{
			double s = (double(c_pos.x) - pos.x - slider_size / 2) * double(max - min) / (double(size.x) - slider_size);
			setValue(s+min);
			ret = true;
		}

		//выойной клик
		if (pressed_pos == c_pos && double_click.getElapsedTime().asMilliseconds() <= double_click_time && !enter)
		{
			enter = true;
			char buf[256];
			std::to_chars(buf, buf + sizeof(buf), value, chars_format::fixed, precision).ptr[0] = 0;
			line_edit.setString(buf);
		}

		pressed = true;
		double_click.restart();
		pressed_pos = c_pos;
		move_value = value;
	}
	//нажатие вне объекта
	if (event.type == Event::MouseButtonPressed && !cursor_on_object)
	{
		if (enter)
		{
			enter = false;
			try {
				double v = std::stod(line_edit.getString().toAnsiString().c_str());
				setValue(v);
				if (change_value_end_callback)
					change_value_end_callback(*this, value);
			}
			catch (...)
			{
			}
		}
	}
	//отжатие
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		if (pressed && cursor_on_object && !o_focus)
		{
			double offset = (double)c_pos.x - pressed_pos.x;
			double coefficent = double(max - min) / ((double)size.x - 4 - slider_size);
			auto value{ move_value + (double)offset * coefficent };
			setValue(value);
			if (change_value_end_callback)
				change_value_end_callback(*this, value);
			ret = true;
		}

		if (pressed && change_value_end_callback)
			change_value_end_callback(*this, value);

		pressed = false;
	}
	//перемещение ползунка
	if (event.type == Event::MouseMoved && pressed)
	{
		double offset = (double)c_pos.x - pressed_pos.x;
		double coefficent = double(max-min) / ((double)size.x - 4 - slider_size);
		setValue(move_value + (double)offset * coefficent);
		ret = true;
	}

	//отправка событий строке
	if (enter)
	{
		bool focus = false;
		line_edit.event(event, focus);
	}

	return ret;
}

void SliderEditHorizontal::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (enter)
	{
		target.draw(line_edit);
	}
	else
	{
		target.draw(fon);
		target.draw(slider);
		target.draw(text_t);
	}
}

//private
void SliderEditHorizontal::sliderRePosition()
{
	// +- 2 поправка на рамку
	double p = double(value - min) / double(max-min);
	slider.setPosition(Vector2f(pos.x + 2 + Uint64(p * (double(size.x) - 4 - slider_size)), pos.y + 2));
}
void SliderEditHorizontal::textUpdateText()
{
	text_mode(text_s, *this);
	text_t.setString(text_s);
}
void SliderEditHorizontal::textUpdatePos()
{
	float x = float(Int64(pos.x + size.x / 2 - text_t.getLocalBounds().width / 2));
	text_t.setPosition(Vector2f(x, pos.y + 1));
}

sf::Int32 SliderEditHorizontal::double_click_time;
//--------------------------------<>-------------------------//