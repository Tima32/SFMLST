#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

#include "../ImportExport.h"

namespace st
{
	class SF_ST_GUI_EXPORT FunctionGraph : public sf::Drawable
	{
	public:
		typedef float (*GraphFunction)(float x, float y);

		FunctionGraph();
		~FunctionGraph();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition()const;

		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize()const;

		void setFunctionX(GraphFunction fgx);
		void setFunctionY(GraphFunction fgy);
		void setZoom(float zoom);
		void setAccuracy(float accuracy = 0.001);
		void setEpsilon(float epsilon = 1.0);
		
		bool addGraph(const std::string& code, sf::Color line_color = sf::Color(255, 255, 255));
		void ereaseGraph(sf::Uint64 number);
		void clearGraph();

		sf::Uint64 addPoint(float x, float y, sf::Color color = sf::Color(255, 192, 203));
		sf::Uint64 addPoint(sf::Vector2f pos, sf::Color color = sf::Color(255, 192, 203));
		void ereasePoint(sf::Uint64 number);
		void clearPoint();

		void setFont(sf::Font& font);

		float getZoom() const;

		void event(const sf::Event& event, bool& focus);

		void draw(sf::RenderTarget& target, sf::RenderStates state) const;
	private:
		struct Point
		{
			sf::Vector2f pos;
			sf::Color color;
		};

		void drawGraph() const;
		bool activityZoneCheck(sf::Vector2f cursor);

		sf::Vector2f pos, size;

		sf::RectangleShape background;

		GraphFunction gfx;
		GraphFunction gfy;
		sf::Vector2f center;
		bool activ;

		mutable sf::RenderTexture graph;
		sf::RectangleShape graph_rectangle;
		mutable sf::Sprite graph_sprite;
		mutable bool drawable;
		float zoom;
		float accuracy;
		float epsilon;

		mutable std::vector<sf::Shader*> shaders;
		std::vector<sf::Color> line_colors;
		sf::Vector2f offset;

		//события
		bool cursor_on_object;
		bool key_constrol;
		bool key_up;
		bool key_doun;
		bool key_right;
		bool key_left;

		//move mouse
		sf::Vector2f mouse_pos;
		bool mouse_move;

		//points
		std::vector<Point> points;

		//вывод координат курсора
		sf::Vector2f cursor_pos;
		bool cursor_show;
		mutable sf::Text text_x;
		mutable sf::Text text_y;
		sf::Font* font;
	};
}