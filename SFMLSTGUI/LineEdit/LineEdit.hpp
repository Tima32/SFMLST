#pragma once
#include <mutex>
#include <thread>
#include <functional>
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"


namespace st
{
	class SF_ST_GUI_EXPORT LineEdit : public sf::Drawable
	{
	public:
		LineEdit();
		~LineEdit();

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& pos);
		const sf::Vector2f& getPosition() const;

		void setSize(float x);
		void setSize(sf::Vector2f size); //y - ignored
		const sf::Vector2f& getSize() const;

		void setFont(const sf::Font& font);

		const sf::String& getString() const;
		void setString(const sf::String& str);

		void setFillColor(sf::Color fill_color);
		void setOutlineColor(sf::Color outline_color);
		void setActiveOutlineColor(sf::Color active_outline_color);
		sf::Color getFillColor() const;
		sf::Color getOutlineColor() const;
		sf::Color getActiveOutlineColor() const;

		//callbacks
		typedef std::function<bool(LineEdit& le, sf::Uint32 c, size_t pos)> EnterCallback;
		void setEnterFilter(EnterCallback enter_callback);

		typedef std::function<void(LineEdit& le)> FocusCallback;
		void setFocusCallback(FocusCallback focus_callback);

		typedef std::function<void(LineEdit& le)> LostFocusCallback;
		void setLostFocusCallback(LostFocusCallback lost_focus_callback);

		static bool IsSustemSymbol(sf::Uint32 symbol);


		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		void textEntered(const sf::Event& event);
		void keyPressed(sf::Keyboard::Key key);
		void keyRelease(sf::Keyboard::Key key);
		void keyTaskWork();
		void setCursorPosition(float x) ;         //вычислить cursor_pos
		void setCursorPosition();                 //вычисляет x курсора
		void textShift();                         //сдвиг текста в поле
		void setSellectShape();                   //выдиление текста
		size_t getCharPos(float x) const;         //плучение номера символа в строке
		void textSellectClear(sf::String& text_s);//удаляет выделенный текст
		void textSellectMoveRight();              //Выделение стрелками
		void textSellectMoveLeft();               //Выделение стрелками

		void textCopy()const;                     //скопировать в буфер
		void textPaste(sf::String& text_s);       //вставить из буфера
		void textCut(sf::String& text_s);         //вырезать в буфер

		sf::Vector2f pos;
		sf::Vector2f size;

		sf::Text text;
		mutable sf::RenderTexture text_t;
		mutable sf::Sprite text_s;
		sf::RectangleShape fon;

		//Цвета
		sf::Color fill_color = { 51, 51, 55 };
		sf::Color outline_color = { 85, 85, 85 };
		sf::Color active_outline_color = { 0, 122, 204 };

		//курсор
		sf::RectangleShape cursor;
		std::size_t cursor_pos;
		sf::Clock cursor_c;

		//Выделение
		sf::RectangleShape text_sellect_rsh;
		sf::Uint64 text_sellect_begin;
		sf::Uint64 text_sellect_end;
		bool text_sellect_begin_b;

		sf::Keyboard::Key key_press; //зажатая клавиша
		sf::Clock key_press_c;
		static bool lshift_press;
		static bool rshift_press;

		bool capture_input;
		bool cursor_on_object;

		//менеджер задач
		static std::mutex all_line_edit_mutex;
		static std::vector<LineEdit*> all_line_edit;
		static std::thread task_worker_thread;
		static void TaskWorker();

		//callback
		EnterCallback enter_callback = nullptr;
		FocusCallback focus_callback = nullptr;
		LostFocusCallback lost_focus_callback = nullptr;
	};
}