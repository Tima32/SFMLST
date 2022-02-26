#include "MainWindow.hpp"
#include <iostream>
#include <windows.h>

using namespace std;
using namespace sf;

MainWindow::MainWindow():
	event_e()
{
	string s("Fonts\\verdana.ttf");
	if (!font.loadFromFile(s))
		cout << "Error load font" << endl;

	button.setFont(font);
	button.setText(L"Добавить.");
	button.setCharacterSize(11);
	button.setPosition(Vector2f(50, 50));
	//button.setSize(Vector2f(75, 23));

	line_edit.setSize(200);
	line_edit.setPosition(50, 80);
	line_edit.setFont(font);
	line_edit.setString(L"Привет мир");

	check_box.setPosition(line_edit.getPosition().x + line_edit.getSize().x + 10, line_edit.getPosition().y);
	check_box.setFont(font);
	check_box.setString(L"Check box");
	check_box.setState(st::CheckBox::State::Mixed);

	//progress_bar
	progress_bar.setFont(font);
	progress_bar.setTextMode(st::ProgressBar::TextMode::Procentage);
	progress_bar.setPosition(50, 100);
	progress_bar.setRange(200);

	//raddio
	radio0.setFont(font);
	radio1.setFont(font);
	radio2.setFont(font);
	radio0.setString(L"Radio 0");
	radio1.setString(L"Radio 1");
	radio2.setString(L"Radio 2");
	radio_c.arr.push_back(&radio0);
	radio_c.arr.push_back(&radio1);
	radio_c.arr.push_back(&radio2);
	radio_c.alignHorizontally(Vector2f(50, 120));

	//scroll bar vert
	scroll_b_v.setSize(0, 720);
	scroll_b_v.setPosition(1024 - scroll_b_v.getSize().x, 0);
	scroll_b_v.setValue(50);

	progress_bar_for_scroll.setFont(font);
	progress_bar_for_scroll.setTextMode(st::ProgressBar::TextMode::Value);
	progress_bar_for_scroll.setPosition(50, 140);
	progress_bar_for_scroll.setRange(200);
	progress_bar_for_scroll.setValue(50);

	//scroll bar horiz
	scroll_b_h.setPosition(50, 160);
	scroll_b_h.setScrollSize(30);
	scroll_b_h.setRange(200);
	scroll_b_h.setValue(50);

	//slider horizontal
	slider_eh.setPosition(50, 180);
	slider_eh.setMin(-5);
	slider_eh.setMax(5);
	slider_eh.setFont(font);
	slider_eh.setTextMode(st::SliderEditHorizontal::TextMode::Value);
	slider_eh.setTextPrecision(2);
	
	//graph
	graph.setFont(font);
	graph.setPosition(400, 50);
	graph.setSize(500, 500);
	graph.addGraph("float y1 = y/(x/50); return sqrt(x*x+y1*y1) - 50;", Color::Red);
	graph.addPoint(60, 60, Color::Green);
	graph.setAccuracy(0.1f);

	//graph func line
	graph_func.setPosition(400, graph.getPosition().y + graph.getSize().y + 5);
	graph_func.setFont(font);
	graph_func.setString(L"float y1 = y/(x/50); return sqrt(x*x+y1*y1) - 50;");
	graph_func.setSize(500);
	
	//graph func add
	graph_func_add.setPosition(400, graph_func.getPosition().y + graph_func.getSize().y + 5);
	graph_func_add.setFont(font);
	graph_func_add.setText(L"Добавить график");
	graph_func_add.setSize(graph.getSize().x, graph_func_add.getSize().y);


	window.create(VideoMode(1024, 720), L"Главное окно   (づ｡◕‿‿◕｡)づ");
	window.setFramerateLimit(60);
}
MainWindow::~MainWindow()
{}

void MainWindow::autoStep()
{
	while (window.isOpen())
		step();
}

//private
void MainWindow::step()
{
	while (window.pollEvent(event_e))
		event();
	draw();
}
void MainWindow::event()
{
	if (event_e.type == Event::Resized)
	{
		auto view = window.getView();
		view.reset(FloatRect(0, 0, float(event_e.size.width), float(event_e.size.height)));
		window.setView(view);

		scroll_b_v.setSize(0, float(event_e.size.height));
		scroll_b_v.setPosition(event_e.size.width - scroll_b_v.getSize().x, 0);
	}
	if (event_e.type == Event::Closed)
	{
		window.close();
	}

	bool focus = false;
	
	if (graph_func_add.event(event_e, focus))
		graph.addGraph(graph_func.getString(), Color::Blue);
	graph_func.event(event_e, focus);
	graph.event(event_e, focus);

	slider_eh.event(event_e, focus);
	if (scroll_b_h.event(event_e, focus))
		progress_bar_for_scroll.setValue(size_t(scroll_b_h.getValue()));
	progress_bar_for_scroll.event(event_e, focus);
	scroll_b_v.event(event_e, focus);
	radio_c.event(event_e, focus);
	progress_bar.event(event_e, focus);
	check_box.event(event_e, focus);
	line_edit.event(event_e, focus);
	if (button.event(event_e, focus))
	{
		cout << "Pressed!" << endl;
		button.setText(button.getText()+L".");
		MessageBoxW(NULL, line_edit.getString().toWideString().c_str(), L"Уиии", MB_OK);
		
	}
}
void MainWindow::draw()
{
	window.clear(Color(30, 30, 30));

	window.draw(button);
	window.draw(line_edit);
	window.draw(check_box);

	progress_bar.setValue(size_t(lock.getElapsedTime().asSeconds()));
	window.draw(progress_bar);

	window.draw(radio_c);

	window.draw(scroll_b_v);
	window.draw(progress_bar_for_scroll);
	window.draw(scroll_b_h);

	window.draw(slider_eh);

	window.draw(graph);
	window.draw(graph_func);
	window.draw(graph_func_add);

	window.display();
}