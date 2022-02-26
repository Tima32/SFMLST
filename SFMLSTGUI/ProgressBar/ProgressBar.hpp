#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"



namespace st
{
#pragma warning(disable: 26812)//предпочитайте enum class над enum

	//ProgressBar size_t
	class SF_ST_GUI_EXPORT ProgressBar : public sf::Drawable
	{
	public:
		enum class TextMode
		{
			None,
			Value,
			Procentage
		};

		ProgressBar();
		~ProgressBar();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		const sf::Vector2f& getPosition() const;

		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		const sf::Vector2f& getSize() const;

		void setRange(size_t range);
		size_t getRange() const;
		void setValue(size_t value);
		size_t getValue() const;
		double getProcentage() const;

		void setTextMode(TextMode mode);
		TextMode getTextMode() const;
		void setFont(const sf::Font& font);

		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		float getProgressSize();
		void reSize();
		void updateText();
		void updateTextPos();

		sf::Vector2f pos, size;
		size_t range, value;

		sf::RectangleShape fon;
		sf::RectangleShape progress;
		sf::Text text;
		TextMode text_mode;

		bool cursor_on_object;//курсор на объекте?
	};
}