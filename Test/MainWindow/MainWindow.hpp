#pragma once
#include <SFML/Graphics.hpp>
#include "..\..\SFMLSTGUI\Button\Button.hpp"
#include "..\..\SFMLSTGUI\LineEdit\LineEdit.hpp"
#include "..\..\SFMLSTGUI\CheckBox\CheckBox.hpp"
#include "..\..\SFMLSTGUI\ProgressBar\ProgressBar.hpp"
#include "..\..\SFMLSTGUI\Radio\Radio.h"
#include "..\..\SFMLSTGUI\ScrollBar\ScrollBar.hpp"
#include "..\..\SFMLSTGUI\Slider\Slider.hpp"
#include "..\..\SFMLSTGUI\Graph\FunctionGraph.hpp"

class MainWindow
{
public:
	MainWindow();
	~MainWindow();

	void autoStep();
private:
	void step();
	void event();
	void draw();

	sf::RenderWindow window;
	sf::Event event_e;
	sf::Font font;
	sf::Clock lock;

	st::Button button;
	st::LineEdit line_edit;
	st::CheckBox check_box;
	st::ProgressBar progress_bar;

	st::Radio radio0;
	st::Radio radio1;
	st::Radio radio2;

	st::RadioController radio_c;
	st::ScrollBarVertical scroll_b_v;

	st::ProgressBar progress_bar_for_scroll;
	st::ScrollBarHorizontal scroll_b_h;

	st::SliderEditHorizontal slider_eh;
	
	st::FunctionGraph graph;
	st::LineEdit graph_func;
	st::Button graph_func_add;
};