#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"
#include "..\LineEdit\LineEdit.hpp"

namespace st
{
	class SF_ST_GUI_EXPORT SliderHorizontal : public sf::Drawable
	{
	public:
		SliderHorizontal();
		~SliderHorizontal();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		//set ignored y size
		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;

		void setSliderSize(float size);
		float getSliderSize() const;

		void setRange(double range);
		double getRange() const;
		void setValue(double value);
		double getValue() const;
		double getProcentage() const;
		
		typedef void(*TextModeT)(sf::String& out_str, const SliderHorizontal& sh);
		class SF_ST_GUI_EXPORT TextMode
		{
		public:
			static void None(sf::String& out_str, const SliderHorizontal& sh);
			static void Procentage(sf::String& out_str, const SliderHorizontal& sh);
			static void Value(sf::String& out_str, const SliderHorizontal& sh);
		private:
			TextMode() = delete;
		};
		void setTextMode(TextModeT tm);
		void setFont(const sf::Font& font);
		const sf::Font* getFont() const;

		bool event(const sf::Event& event, bool& focus);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		void sliderRePosition();
		void textUpdateText();
		void textUpdatePos();

		sf::Vector2f pos, size;
		float slider_size;
		double range, value;

		sf::RectangleShape fon;
		sf::RectangleShape slider;
		sf::Text text_t;
		sf::String text_s;
		TextModeT text_mode;

		bool cursor_on_object; //курсор на объекте
		bool pressed;
		sf::Vector2f pressed_pos;
		double move_value;
	};

	class SF_ST_GUI_EXPORT SliderEditHorizontal : public sf::Drawable
	{
	public:
		SliderEditHorizontal();
		~SliderEditHorizontal();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		//set ignored y size
		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;

		void setSliderSize(float size);
		float getSliderSize() const;

		void setMin(double range);
		double getMin() const;
		void setMax(double range);
		double getMax() const;
		void setValue(double value);
		double getValue() const;
		double getProcentage() const;

		typedef void(*TextModeT)(sf::String& out_str, const SliderEditHorizontal& sh);
		class SF_ST_GUI_EXPORT TextMode
		{
		public:
			static void None(sf::String& out_str, const SliderEditHorizontal& sh);
			static void Procentage(sf::String& out_str, const SliderEditHorizontal& sh);
			static void Value(sf::String& out_str, const SliderEditHorizontal& sh);
		private:
			TextMode() = delete;
		};
		void setTextMode(TextModeT tm);
		void setTextPrecision(int precision);
		int getTextPrecision()const;
		void setFont(const sf::Font& font);
		const sf::Font* getFont() const;

		//callbacks
		typedef std::function<void(SliderEditHorizontal& seh, double value)> ChangeValueEndCallback;
		void setChangeValueEndCallback(ChangeValueEndCallback change_value_end_callback);

		bool event(const sf::Event& event, bool& focus);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		void sliderRePosition();
		void textUpdateText();
		void textUpdatePos();

		sf::Vector2f pos, size;
		float slider_size;
		double min, max, value;

		sf::RectangleShape fon;
		sf::RectangleShape slider;
		sf::Text text_t;
		sf::String text_s;
		int precision;
		TextModeT text_mode;
		LineEdit line_edit;

		bool cursor_on_object; //курсор на объекте
		bool pressed;
		static sf::Int32 double_click_time;
		sf::Clock double_click;
		bool enter;
		sf::Vector2f pressed_pos;
		double move_value;

		//callbacks
		ChangeValueEndCallback change_value_end_callback{ nullptr };
	};
}