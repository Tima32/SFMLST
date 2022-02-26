#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"


namespace st
{
	#pragma warning(disable: 26812)//предпочитайте enum class над enum
	class SF_ST_GUI_EXPORT Radio : public sf::Drawable
	{
	public:
		Radio();
		~Radio();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		sf::Vector2f getSize() const;

		void setString(const sf::String& str);
		const sf::String& getString() const;

		void setFont(const sf::Font& font);
		const sf::Font* getFont() const;

		void setCheck(bool ch);
		bool getCheck() const;

		bool event(const sf::Event& event, bool& focus);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void rePosition();
		void calcSize();

		sf::Vector2f pos;
		sf::Vector2f size;
		bool checked;

		bool cursor_on_object;
		bool pressed;

		sf::Text text;
		sf::CircleShape fon;
		sf::CircleShape check;
	};

	class SF_ST_GUI_EXPORT RadioController : public sf::Drawable
	{
	public:
		RadioController();
		~RadioController();

		void alignVertically(sf::Vector2f pos);
		void alignHorizontally(sf::Vector2f pos);

		bool event(const sf::Event& event, bool& focus);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		std::vector<Radio*> arr;
		bool destructor_delete_radio; //если true деструктор освободит память выделенную под ratio
	private:
	};
}