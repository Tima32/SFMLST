#pragma once
#include <SFML\Graphics.hpp>
#include "..\ImportExport.h"
#include "..\Vector2.hpp"
#include "..\LineEdit\LineEdit.hpp"
#include "..\ScrollBar\ScrollBar.hpp"

namespace st
{
#pragma warning(disable: 26812)//предпочитайте enum class над enum
	class SF_ST_GUI_EXPORT Table : public sf::Drawable
	{
	public:
		Table();
		~Table();

		void setPosition(float x, float y);
		void setPosition(sf::Vector2f pos);
		sf::Vector2f getPosition() const;

		void setSize(float x, float y);
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize() const;

		void setDefaultCellSize(float x, float y);
		void setDefaultCellSize(sf::Vector2f default_cell_size);
		sf::Vector2f getDefaultCellSize() const;

		void setFont(const sf::Font& font);

		void setHorizontalIngexString(size_t num, const sf::String& str);
		const sf::String& getHorizontalIngexString(size_t num) const;
		void setVerticalIngexString(size_t num, const sf::String& str);
		const sf::String& getVerticalIngexString(size_t num) const;

		//колличество ячеек в таблице
		void setTableSize(size_t x, size_t y);
		void setTableSize(st::Vector2s size_table);
		st::Vector2s getTableSize() const;

		//Показать / скрыть скролы
		void verticalScrollShown(bool show);
		void horizontalScrollShown(bool show);

		st::LineEdit& getCell(size_t x, size_t y);
		st::LineEdit& getCell(st::Vector2s pos);

		void setOutlineColor(sf::Color outloine_color);
		sf::Color getOutlineColor() const;

		bool event(const sf::Event& event, bool& focus);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	private:
		void rePos();
		void rePosText();

		sf::String toCharIndex(size_t num) const;

		sf::Vector2f pos;
		sf::Vector2f size = {500, 250};
		st::Vector2s size_table;
		sf::Vector2f default_cell_size = {100, 17};
		sf::Vector2f scroll_pos = {0, 0};

		sf::Font const* font = nullptr;

		sf::Color outline_color = { 85, 85, 85 };
		sf::Color intex_fill_color = { 63, 63, 70 };
		sf::Color intex_outline_color = { 85, 85, 85 };

		sf::RectangleShape outline;
		sf::RectangleShape left_up_index;
		std::vector<sf::RectangleShape> horizontal_index;
		std::vector<sf::RectangleShape> vertical_index;
		std::vector<sf::Text> horizontal_index_text;
		std::vector<sf::Text> vertical_index_text;

		std::vector<float> horisontal_length;
		std::vector<float> vertical_length;

		std::vector<std::vector<st::LineEdit*>> line_edit_table;

		st::ScrollBarVertical vertical_scroll;
		bool vertical_scroll_shown = true;
		st::ScrollBarHorizontal horizontal_scroll;
		bool horizontal_scroll_shown = true;

		mutable sf::Sprite sprite;
		mutable sf::RenderTexture texture;
	};
}