#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"


namespace st
{
	class SF_ST_GUI_EXPORT ButtonBase : public sf::Drawable
	{
	public:
		ButtonBase(sf::Vector2f pos, sf::Vector2f size);
		virtual ~ButtonBase();

		void setPosition(float x, float y);
		virtual void setPosition(sf::Vector2f pos) = 0;
		virtual sf::Vector2f getPosition() const;

		void setSize(float x, float y);
		virtual void setSize(sf::Vector2f size) = 0;
		virtual sf::Vector2f getSize() const;

		virtual bool event(const sf::Event& event, bool& focus);

	protected:
		sf::Vector2f pos;
		sf::Vector2f size;


		bool pressed; //кнопка нажата
		bool cursor_on_object; //курсор на объекте
	};

#pragma warning(disable: 26812)//предпочитайте enum class над enum
	class SF_ST_GUI_EXPORT Button : public ButtonBase
	{
	public:
		Button();
		virtual ~Button();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos) override;
		void setSize(float x, float y);
		void setSize(sf::Vector2f size) override;
		void setFont(const sf::Font& font);
		void setAutoSize(bool);

		void setText(const sf::String& str);
		const sf::String& getText()const;

		void setCharacterSize(unsigned int size);
		sf::Uint32 getCharacterSize() const;

		bool event(const sf::Event& event, bool& focus) override;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		void textPos();
		void fonSize();

		sf::RectangleShape fon;
		sf::Text t;

		bool auto_size;
		sf::Color no_press_c;
		sf::Color press_c;
		sf::Color outline_c;
	};
}