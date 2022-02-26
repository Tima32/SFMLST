#pragma once
#include <functional>
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"



namespace st
{
	#pragma warning(disable: 26812)//предпочитайте enum class над enum
	class SF_ST_GUI_EXPORT CheckBox : public sf::Drawable
	{
	public:
		enum class State
		{
			Unchecked,
			Checked,
			Mixed
		};

		CheckBox();
		~CheckBox();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		sf::Vector2f getSize() const;

		void setFont(const sf::Font& font);

		void setString(const sf::String& str);
		const sf::String& getString() const;

		void setState(State s);
		State getState() const;

		//callbacks
		typedef std::function<void(CheckBox& le, State state)> ChangeStateCallback;
		void setChangeStateCallback(ChangeStateCallback change_state_callback);

		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void calcSize();

		sf::Vector2f pos;
		sf::Vector2f size;
		State state;

		sf::RectangleShape fon;
		sf::Sprite checked_s;
		sf::RectangleShape checked_mixed;
		sf::Text text;

		bool cursor_on_object;//курсор на объекте?
		bool pressed;

		//callbacks
		ChangeStateCallback change_state_callback{ nullptr };
	};
}