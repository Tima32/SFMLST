#include "Button.hpp"


using namespace sf;
using namespace st;

//---------[ButtonBase]----------
ButtonBase::ButtonBase(sf::Vector2f pos, sf::Vector2f size) :
	pos(pos), size(size), pressed(false), cursor_on_object(false)
{}
ButtonBase::~ButtonBase()
{}

void ButtonBase::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
sf::Vector2f ButtonBase::getPosition() const
{
	return pos;
}

void  ButtonBase::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
sf::Vector2f ButtonBase::getSize() const
{
	return size;
}

bool ButtonBase::event(const sf::Event& event, bool& focus)
{
	Vector2f c_pos(-1, -1); //позиция курсора
	bool o_focus = focus;   //фокус до начала обработки события
	bool click = false;

	//курсор на объекте?
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
		//На кнопке
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;
	}

	//фокус
	if (cursor_on_object)
		focus = true;

	//нажатие
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left &&cursor_on_object)
		pressed = true;
	//отжатие
	if (event.type == Event::MouseButtonReleased)
	{
		if (cursor_on_object && event.mouseButton.button == Mouse::Button::Left && pressed)
			click = true;
		pressed = false;
	}

	return click;
}


//---------------[Button]---------------
constexpr float x_t_indent = 7;
constexpr float xl_t_indent = 7;
constexpr float y_t_indent = 3;
constexpr float yt_t_indent = 7;

Button::Button():
	ButtonBase(Vector2f(0, 0), Vector2f(40, 20)),
	auto_size(true),
	no_press_c(63, 63, 70), press_c(0, 122, 204), outline_c(85, 85, 85)
{
	
	setPosition(pos);
	//setSize(size);

	fon.setOutlineThickness(1);
	fon.setSize(size);
	fon.setFillColor(no_press_c);
	fon.setOutlineColor(outline_c);

	t.setCharacterSize(11);
}
Button::~Button()
{}

void Button::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void Button::setPosition(sf::Vector2f pos)
{
	this->pos = pos;

	fon.setPosition(Vector2f(pos.x + 1, pos.y + 1));

	textPos();
}
void Button::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void Button::setSize(sf::Vector2f size)
{
	this->size = size;
	//auto_size = false;

	textPos();
	//fonSize();
	fon.setSize(Vector2f(size.x - 2, size.y - 2));
}
void Button::setFont(const Font& font)
{
	t.setFont(font);
	textPos();
}
void Button::setAutoSize(bool auto_size)
{
	this->auto_size = auto_size;
}

void Button::setText(const sf::String& str)
{
	t.setString(str);

	if (auto_size)
		fonSize();
	textPos();
}
const sf::String& Button::getText() const
{
	return t.getString();
}

void Button::setCharacterSize(unsigned int size)
{
	t.setCharacterSize(size);

	if (auto_size)
		fonSize();
	textPos();
}
sf::Uint32 Button::getCharacterSize() const
{
	return t.getCharacterSize();
}

bool Button::event(const sf::Event& event, bool& focus)
{
	bool click = ButtonBase::event(event, focus);

	//изменениц цвета при наведении
	if (event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased ||
		event.type == Event::MouseMoved)
	{
		if (cursor_on_object)
			fon.setOutlineColor(press_c);
		else
			fon.setOutlineColor(outline_c);
	}

	//изменение цвета при лике
	if ((event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased) &&
		event.mouseButton.button == Mouse::Button::Left)
	{
		if (pressed)
			fon.setFillColor(press_c);
		else
			fon.setFillColor(no_press_c);
	}

	return click;
}

void Button::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(fon);
	target.draw(t);
}

//private
void Button::textPos()
{
	FloatRect bounds = t.getLocalBounds();
	Vector2f t_size(
		bounds.width + bounds.left/* + x_t_indent * 2 + xl_t_indent*/,
		bounds.height + bounds.top /*+ y_t_indent * 2 + yt_t_indent*/
	);
	Vector2f center(pos.x + size.x / 2, pos.y + size.y / 2);

	t.setPosition(Vector2f(
		float(Int64(center.x - t_size.x / 2)), 
		float(Int64(center.y - t_size.y / 2))
	));

	return;
}
void Button::fonSize()
{
	FloatRect bounds = t.getLocalBounds();
	Vector2f t_size(
		bounds.width + bounds.left + x_t_indent * 2 + xl_t_indent,
		bounds.height + bounds.top + y_t_indent * 2 + yt_t_indent
	);

	fon.setSize(Vector2f(
		float(Int64(t_size.x))-2,
		float(Int64(t_size.y)) - 2));
	size.x = float(Int64(t_size.x));
	size.y = float(Int64(t_size.y));
}