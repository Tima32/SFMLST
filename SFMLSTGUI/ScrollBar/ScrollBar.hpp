#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"

namespace st
{
#pragma warning(disable: 26812)//предпочитайте enum class над enum
	class SF_ST_GUI_EXPORT ScrollBarVertical : public sf::Drawable
	{
	public:
		ScrollBarVertical();
		~ScrollBarVertical();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		//set ignored x size
		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;

		void setScrollSize(float size);
		float getScrollSize() const;

		void setRange(double range);
		double getRange() const;
		void setValue(double value);
		double getValue() const;
		double getProcentage() const;

		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		bool scrollEvent(const sf::Event& event, bool& focus);

		void scrollRePosition();
		void renderTextures();

		sf::Vector2f pos, size;
		float scroll_size;
		double range, value;

		sf::Texture fon_t;
		sf::Texture scroll_t;
		sf::Sprite fon_s;
		sf::Sprite scroll_s;

		bool cursor_on_object; //курсор на объекте
		bool pressed;
		sf::Vector2f pressed_pos;
		double move_value;
	};


	class SF_ST_GUI_EXPORT ScrollBarHorizontal : public sf::Drawable
	{
	public:
		ScrollBarHorizontal();
		~ScrollBarHorizontal();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		//set ignored y size
		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;

		void setScrollSize(float size);
		float getScrollSize() const;

		void setRange(double range);
		double getRange() const;
		void setValue(double value);
		double getValue() const;
		double getProcentage() const;

		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		bool scrollEvent(const sf::Event& event, bool& focus);

		void scrollRePosition();
		void renderTextures();

		sf::Vector2f pos, size;
		float scroll_size;
		double range, value;

		sf::Texture fon_t;
		sf::Texture scroll_t;
		sf::Sprite fon_s;
		sf::Sprite scroll_s;

		bool cursor_on_object; //курсор на объекте
		bool pressed;
		sf::Vector2f pressed_pos;
		double move_value;
	};
}