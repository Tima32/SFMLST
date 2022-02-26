#include "LineEdit.hpp"
#include <iostream>
#include <chrono>

#ifdef _WINDOWS
#include <Windows.h>
#endif

using namespace sf;
using namespace st;
using namespace std;


//const sf::Color no_press_c (51, 51, 55);
//const sf::Color press_c    (0, 122, 204);
//const sf::Color outline_c  (85, 85, 85);
const sf::Color sellect_c  (0, 122, 204);

#pragma warning(disable: 26812)//предпочитайте enum class над enum
LineEdit::LineEdit():
	pos(0, 0), size(200, 17),
	capture_input(false), cursor_on_object(false),
	cursor_pos(0),
	text_sellect_begin(0), text_sellect_end(0), text_sellect_begin_b(false),//Выдиление
	key_press(Keyboard::Key::Unknown)
{
	fon.setOutlineThickness(1);
	fon.setOutlineColor(outline_color);
	fon.setFillColor(fill_color);

	text.setCharacterSize(11);

	cursor.setSize(Vector2f(1, size.y-2));

	//выдиление текста
	text_sellect_rsh.setSize(Vector2f(0, size.y - 2));
	text_sellect_rsh.setPosition(Vector2f(pos.x + 1, pos.y));
	text_sellect_rsh.setFillColor(sellect_c);


	setSize(size);
	setPosition(pos);

	all_line_edit_mutex.lock();
	all_line_edit.push_back(this);
	all_line_edit_mutex.unlock();
}
LineEdit::~LineEdit()
{
	all_line_edit_mutex.lock();
	for (auto i = all_line_edit.begin(); i != all_line_edit.end(); i++)
	{
		if (*i == this)
		{
			all_line_edit.erase(i);
			break;
		}
	}
	all_line_edit_mutex.unlock();
}

void LineEdit::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void LineEdit::setPosition(const sf::Vector2f& pos)
{
	this->pos = pos;
	fon.setPosition(Vector2f(pos.x + 1, pos.y + 1));
	text_s.setPosition(Vector2f(pos.x + 1, pos.y + 1));
	setCursorPosition();
}
const sf::Vector2f& LineEdit::getPosition() const
{
	return pos;
}

void LineEdit::setSize(float x)
{
	setSize(Vector2f(x, 17));
}
void LineEdit::setSize(sf::Vector2f size)
{
	size.y = 17;
	this->size = size;
	fon.setSize(Vector2f(size.x-2, size.y-2));
	text_t.create(unsigned int(size.x - 2), unsigned int(size.y - 2));
	text_s.setTextureRect(IntRect(0, 0, int(size.x) - 2, int(size.y) - 2));
}
const sf::Vector2f& LineEdit::getSize() const
{
	return size;
}

void LineEdit::setFont(const sf::Font& font)
{
	text.setFont(font);
}

const sf::String& LineEdit::getString() const
{
	return text.getString();
}
void LineEdit::setString(const sf::String& str)
{
	text.setString(str);
	cursor_pos = text_sellect_begin = text_sellect_end = 0;
	text.setPosition(0, text.getPosition().y);
}

void LineEdit::setFillColor(sf::Color fill_color)
{
	this->fill_color = fill_color;
	fon.setFillColor(fill_color);
}
void LineEdit::setOutlineColor(sf::Color outline_color)
{
	this->outline_color = outline_color;
	if (cursor_on_object || capture_input)
	{
		//fon.setOutlineColor(active_outline_color);
	}
	else
	{
		fon.setOutlineColor(outline_color);
	}
}
void LineEdit::setActiveOutlineColor(sf::Color active_outline_color)
{
	this->active_outline_color = active_outline_color;
	if (cursor_on_object || capture_input)
	{
		fon.setOutlineColor(active_outline_color);
	}
	else
	{
		//fon.setOutlineColor(outline_color);
	}
}

sf::Color LineEdit::getFillColor() const
{
	return fill_color;
}
sf::Color LineEdit::getOutlineColor() const
{
	return outline_color;
}
sf::Color LineEdit::getActiveOutlineColor() const
{
	return active_outline_color;
}

//callbacks
void LineEdit::setEnterFilter(EnterCallback enter_callback)
{
	this->enter_callback = enter_callback;
}
void LineEdit::setFocusCallback(FocusCallback focus_callback)
{
	this->focus_callback = focus_callback;
}
void LineEdit::setLostFocusCallback(LostFocusCallback lost_focus_callback)
{
	this->lost_focus_callback = lost_focus_callback;
}

bool LineEdit::IsSustemSymbol(sf::Uint32 symbol)
{
	if (0 <= symbol && symbol <= 0x1f ||
		0x80 <= symbol && symbol <= 0x9f)
		return true;
	return false;
}

bool LineEdit::event(const sf::Event& event, bool& focus)
{
	Vector2f c_pos(-1, -1); //позиция курсора
	bool o_focus = focus;   //фокус до начала обработки события

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
		//На объекте
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;

		//Передвижение курсора
		if (capture_input && text_sellect_begin_b)
		{
			setCursorPosition(float(event.mouseMove.x) - pos.x - 1);
		}

		//выделение текста
		if (text_sellect_begin_b)
		{
			text_sellect_end = getCharPos(float(event.mouseMove.x) - pos.x - 1);  //конец выделения
			setSellectShape();                                                    //изменение выделения
		}
	}

	//фокус
	if (cursor_on_object)
		focus = true;

	//нажатие
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left && cursor_on_object)
	{
		if (focus_callback)
			focus_callback(*this);

		capture_input = true;
		setCursorPosition(float(event.mouseButton.x)-pos.x-1);

		//выделение текста
		text_sellect_begin_b = true;                                             //выделение начато
		text_sellect_begin = text_sellect_end = getCharPos(float(event.mouseButton.x) - pos.x - 1); //начало выделения
		setSellectShape();
		text_sellect_rsh.setFillColor(sellect_c);
	}
	//нажатие вне объекта
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Button::Left && !cursor_on_object)
	{
		if (capture_input && lost_focus_callback)
			lost_focus_callback(*this);

		capture_input = false;
		text_sellect_rsh.setFillColor(outline_color);
	}
	//отжатие
	if (event.type == Event::MouseButtonReleased)
	{
		//printf("LineEdit: Release\n");

		//Передвижение курсора
		if (capture_input)
		{
			setCursorPosition(float(event.mouseButton.x) - pos.x - 1);
		}

		//выделение текста
		if (text_sellect_begin_b)
		{
			text_sellect_begin_b = false;                                         //выделение завершено
			text_sellect_end = getCharPos(float(event.mouseButton.x) - pos.x - 1);//конец выделения
			setSellectShape();                                                    //изменение выделения
		}
	}

	//ввод текста
	if (event.type == Event::TextEntered && capture_input)
		textEntered(event);
	//клавиши
	if (event.type == Event::KeyPressed)
	{
		keyPressed(event.key.code);
	}
	if (event.type == Event::KeyReleased)
	{
		keyRelease(event.key.code);
	}

	//цвет коткра
	if (event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased ||
		event.type == Event::MouseMoved)
	{
		if (cursor_on_object || capture_input)
		{
			fon.setOutlineColor(active_outline_color);
		}
		else
		{
			fon.setOutlineColor(outline_color);
		}
	}

	return false;
}

void LineEdit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fon);

	text_t.clear(fill_color);
	if (text_sellect_begin != text_sellect_end)
		text_t.draw(text_sellect_rsh);
	text_t.draw(text);
	text_t.display();

	text_s.setTexture(text_t.getTexture());
	target.draw(text_s);

	if ((cursor_c.getElapsedTime().asSeconds() - double(size_t(cursor_c.getElapsedTime().asSeconds())) < 0.5 || key_press != -1) &&
		capture_input)
		target.draw(cursor);
}

//private
void LineEdit::textEntered(const Event& event)
{
	cursor_c.restart();
	String text_s = text.getString();

	if (enter_callback != nullptr && !enter_callback(*this, event.text.unicode, cursor_pos))
		return;

	//ctrl+v
	if (event.text.unicode == 22)
	{
		textPaste(text_s);
	}
	//ctrl+c
	else if (event.text.unicode == 3)
	{
		textCopy();
	}
	//ctrl+a
	else if (event.text.unicode == 1)
	{
		text_sellect_begin = 0;
		text_sellect_end = cursor_pos = text.getString().getSize();
	}
	//ctrl+x
	else if (event.text.unicode == 24)
	{
		textCut(text_s);
	}


	if (1 <= event.text.unicode && event.text.unicode <= 29 && event.text.unicode != 8)
		goto end_input;

	if (event.text.unicode == 0xD)//Return
	{
	}
	else if (event.text.unicode == 0x8)//Backspace
	{
		//удаление выделенного текста
		if (text_sellect_begin != text_sellect_end)
		{
			textSellectClear(text_s);
		}
		else if (cursor_pos != 0)
		{
			text_s.erase(cursor_pos - 1, 1);
			cursor_pos--;
		}
	}
	else if (event.text.unicode == 127)//Backspace + ctrl
	{
	}
	else //Ввод
	{
		if (text_sellect_begin != text_sellect_end)
			textSellectClear(text_s);
		
		text_s.insert(cursor_pos, event.text.unicode);
		cursor_pos++;
		
		text_sellect_begin = text_sellect_end = 0;//сброс выдиления
	}

	end_input:

	text.setString(text_s);
	textShift();
	setCursorPosition();
	setSellectShape();
}

#pragma warning(disable: 26812)//предпочитайте enum class над enum
void LineEdit::keyPressed(sf::Keyboard::Key key)
{
	if (key == Keyboard::LShift)
		lshift_press = true;
	if (key == Keyboard::RShift)
		rshift_press = true;

	//выход если объект не в фокусе
	if (!capture_input)
		return;

	//delete
	if (key == Keyboard::Delete)
	{
		String text_s = text.getString();
		if (text_sellect_begin != text_sellect_end)
			textSellectClear(text_s);
		else
			text_s.erase(cursor_pos, 1);
		text.setString(text_s);
		textShift();
		setCursorPosition();
		key_press = Keyboard::Delete;
		key_press_c.restart();
	}
	else if (key == Keyboard::Right)
	{
		//Передвижение выдиления
		if (lshift_press || rshift_press)
			textSellectMoveRight();
		else
			text_sellect_begin = text_sellect_end = 0;


		if (text.getString().getSize() > cursor_pos)
			cursor_pos++;
		textShift();
		setCursorPosition();
		setSellectShape();
		key_press = Keyboard::Right;
		key_press_c.restart();
	}
	else if (key == Keyboard::Left)
	{
		//Передвижение выдиления
		if (lshift_press || rshift_press)
			textSellectMoveLeft();
		else
			text_sellect_begin = text_sellect_end = 0;

		if (cursor_pos != 0)
			cursor_pos--;
		textShift();
		setCursorPosition();
		setSellectShape();
		key_press = Keyboard::Left;
		key_press_c.restart();
	}
}
void LineEdit::keyRelease(sf::Keyboard::Key key)
{
	if (key == Keyboard::LShift)
		lshift_press = false;
	if (key == Keyboard::RShift)
		rshift_press = false;

	//delete
	if (key == Keyboard::Delete)
	{
		if (key_press == Keyboard::Delete)
			key_press = Keyboard::Unknown;
	}
	else if (key == Keyboard::Right)
	{
		if (key_press == Keyboard::Right)
			key_press = Keyboard::Unknown;
	}
	else if (key == Keyboard::Left)
	{
		if (key_press == Keyboard::Left)
			key_press = Keyboard::Unknown;
	}
}
void LineEdit::keyTaskWork()
{
	if (key_press_c.getElapsedTime().asSeconds() < 0.5)
		return;

	if (key_press == Keyboard::Delete)
	{
		String text_s = text.getString();
		text.setString(text_s);
		textShift();
		setCursorPosition();
	}
	else if (key_press == Keyboard::Right)
	{
		//Передвижение выдиления
		if (lshift_press || rshift_press)
			textSellectMoveRight();
		else
			text_sellect_begin = text_sellect_end = 0;


		if (text.getString().getSize() > cursor_pos)
			cursor_pos++;
		textShift();
		setCursorPosition();
		setSellectShape();
	}
	else if (key_press == Keyboard::Left)
	{
		//Передвижение выдиления
		if (lshift_press || rshift_press)
			textSellectMoveLeft();
		else
			text_sellect_begin = text_sellect_end = 0;

		if (cursor_pos != 0)
			cursor_pos--;
		textShift();
		setCursorPosition();
		setSellectShape();
	}
}
void LineEdit::setCursorPosition(float x)
{
	for (size_t i = 0; i <= text.getString().getSize(); i++)
	{
		if (x > text.findCharacterPos(i).x)
			cursor_pos = i;
		if (x < text.findCharacterPos(i).x)
			break;
	}
	setCursorPosition();
}
void LineEdit::setCursorPosition()
{
	Vector2f c_p;
	c_p.y = pos.y + 1;
	c_p.x = text.findCharacterPos(cursor_pos).x + pos.x;
	cursor.setPosition(c_p);
}
void LineEdit::textShift()
{
	//курсор за левой границей
 	if (text.findCharacterPos(cursor_pos).x < 0)
	{
		float x = float(text.getPosition().x - text.findCharacterPos(cursor_pos).x + size.x * 0.25);
		if (x > 0) //проверка на отрыв от границы
			x = 0;
		text.setPosition(x, text.getPosition().y);
	}
	//курсор за правой границей
	else if (text.findCharacterPos(cursor_pos).x > size.x-2)
	{
		float x = text.getPosition().x - (text.findCharacterPos(cursor_pos).x - (size.x - 2));
		text.setPosition(x, text.getPosition().y);
	}
}
void LineEdit::setSellectShape()
{
	Vector2f pos = text_sellect_rsh.getPosition();
	Vector2f size = text_sellect_rsh.getSize();

	pos.x = text.findCharacterPos(text_sellect_begin).x;
	size.x = text.findCharacterPos(text_sellect_end).x - pos.x;

	text_sellect_rsh.setPosition(pos);
	text_sellect_rsh.setSize(size);
}
size_t LineEdit::getCharPos(float x) const
{
	size_t num = 0;
	for (size_t i = 0; i <= text.getString().getSize(); i++)
	{
		if (x > text.findCharacterPos(i).x)
			num = i;
		if (x < text.findCharacterPos(i).x)
			break;
	}
	return num;
}
void LineEdit::textSellectClear(sf::String& text_s)
{
	size_t begin, size;
	if (text_sellect_end < text_sellect_begin)
	{
		begin = text_sellect_end;
		size = text_sellect_begin - text_sellect_end;
	}
	else
	{
		begin = text_sellect_begin;
		size = text_sellect_end - text_sellect_begin;
		cursor_pos -= size;
	}

	text_s.erase(begin, size);
	text_sellect_begin = text_sellect_end = 0;//сброс выдиления
}
void LineEdit::textSellectMoveRight()
{
	//если нет выделенного текста
	if (text_sellect_begin == text_sellect_end)
		text_sellect_begin = text_sellect_end = cursor_pos;

	//Выделенно до конца?
	if (text.getString().getSize() == text_sellect_end)
		return;
	text_sellect_end++;
}
void LineEdit::textSellectMoveLeft()
{
	//если нет выделенного текста
	if (text_sellect_begin == text_sellect_end)
		text_sellect_begin = text_sellect_end = cursor_pos;

	//Выделенно до начала?
	if (text_sellect_end == 0)
		return;
	text_sellect_end--;
}

void LineEdit::textCopy()const
{
	size_t begin, size;
	if (text_sellect_end < text_sellect_begin)
	{
		begin = text_sellect_end;
		size = text_sellect_begin - text_sellect_end;
	}
	else
	{
		begin = text_sellect_begin;
		size = text_sellect_end - text_sellect_begin;
	}

#ifdef _WINDOWS
	if (OpenClipboard(0))//открываем буфер обмена
	{
		HGLOBAL hgBuffer;
		wchar_t* chBuffer;
		EmptyClipboard(); //очищаем буфер
		hgBuffer = GlobalAlloc(GMEM_DDESHARE, (size + 1) * 2);//выделяем память
		if (hgBuffer == nullptr)
			goto close_clipboard;
		chBuffer = (wchar_t*)GlobalLock(hgBuffer); //блокируем память
		if (chBuffer == nullptr)
			goto close_clipboard;

		memcpy(chBuffer, text.getString().toWideString().substr(begin, size).c_str(), (size + 1) * 2);
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_UNICODETEXT, hgBuffer);//помещаем текст в буфер обмена

	close_clipboard:
		CloseClipboard(); //закрываем буфер обмена
	}
#endif
}
void LineEdit::textPaste(sf::String& text_s)
{
	#ifdef _WINDOWS
	textSellectClear(text_s);
	wchar_t* str;
	if (OpenClipboard(0))
	{
		HANDLE hData = GetClipboardData(CF_UNICODETEXT);
		str = (wchar_t*)GlobalLock(hData);
		if (str)
		{
			text_s.insert(cursor_pos, str);
			cursor_pos += wcslen(str);
		}
		GlobalUnlock(hData);
		CloseClipboard();
	}
	#endif
}
void LineEdit::textCut(sf::String& text_s)
{
	textCopy();
	textSellectClear(text_s);
}

bool LineEdit::lshift_press = false;
bool LineEdit::rshift_press = false;

//-----------------[менеджер задач]-------------------
using namespace std::chrono_literals;

std::mutex LineEdit::all_line_edit_mutex;
std::vector<LineEdit*> LineEdit::all_line_edit;
std::thread LineEdit::task_worker_thread(TaskWorker);

void LineEdit::TaskWorker()
{
	task_worker_thread.detach();
task_worker_begin:

	all_line_edit_mutex.lock();
	for (auto& le : all_line_edit)
	{
		le->keyTaskWork();
	}
	all_line_edit_mutex.unlock();
	this_thread::sleep_for(50ms);

	goto task_worker_begin;
}
//-------------------------[]-------------------------