#include <iostream>
#include <SFML/Graphics.hpp>
#include "..\SFMLSTGUI\Button\Button.hpp"
#include <Windows.h>

using std::cout;
using std::endl;

using namespace sf;

int main()
{
	sf::Font font;
	if (!font.loadFromFile("Fonts\\verdana.ttf"))
		cout << "Error load font" << endl;
	st::Button button;
	button.setFont(font);
	button.setPosition(50, 50);
	button.setText(L"Не трогай меня человек");
	sf::Event ev;
	sf::RenderWindow window;
	window.create(VideoMode(720, 480), L"Окно (づ｡◕‿‿◕｡)づ  ٩(｡•́‿•̀｡)۶");

	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				window.close();
			bool focus = false;
			if (button.event(ev, focus))
				MessageBoxW(NULL, LR"(/╲/\╭(ఠఠ益ఠఠ)╮/\╱\)", L"Граааа!", MB_OK);
		}
		window.clear(sf::Color::Black);
		window.draw(button);
		window.display();
	}
	


	return 0;
}