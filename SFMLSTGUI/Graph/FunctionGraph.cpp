//#include <conio.h>
#include <iostream>

#define SFML_NO_DEPRECATED_WARNINGS
#include "FunctionGraph.hpp"

using namespace std;
using namespace sf;
using namespace st;


st::FunctionGraph::FunctionGraph():
	pos(0, 0), size(100, 100),
	gfx(nullptr),
	gfy(nullptr),
	activ(false),
	drawable(false),
	zoom(1),
	accuracy(0.1f),
	epsilon(1),
	cursor_on_object(false), key_constrol(false),
	key_up(false),
    key_doun(false),
	key_right(false),
	key_left(false),
	mouse_move(false),
	cursor_pos(0, 0),
	cursor_show(false),
	font(nullptr)
{
	background.setFillColor(Color(85, 92, 94));
	setPosition(pos);
	setSize(size);
}
st::FunctionGraph::~FunctionGraph()
{
	for (Uint64 i = 0; i < shaders.size(); ++i)
	{
		delete shaders[i];
	}
}

void st::FunctionGraph::setPosition(float x, float y)
{
	setPosition(Vector2f(x, y));
}
void st::FunctionGraph::setPosition(sf::Vector2f pos)
{
	this->pos = pos;
	background.setPosition(pos);
	graph_sprite.setPosition(Vector2f(pos.x + 10, pos.y + 10));
}
sf::Vector2f st::FunctionGraph::getPosition()const
{
	return pos;
}

void st::FunctionGraph::setSize(float x, float y)
{
	setSize(Vector2f(x, y));
}
void st::FunctionGraph::setSize(sf::Vector2f size)
{
	this->size = size;
	background.setSize(size);
	graph.create((Uint32)size.x - 20, (Uint32)size.y - 20);
	graph_rectangle.setSize(Vector2f(float(graph.getSize().x), float(graph.getSize().y)));

	drawable = false;
}
sf::Vector2f st::FunctionGraph::getSize()const
{
	return size;
}

void st::FunctionGraph::setFunctionX(GraphFunction gfx)
{
	this->gfx = gfx;
	drawable = false;
}
void st::FunctionGraph::setFunctionY(GraphFunction gfy)
{
	this->gfy = gfy;
	drawable = false;
}
void st::FunctionGraph::setZoom(float zoom)
{
	this->zoom = zoom;
	drawable = false;
}
void st::FunctionGraph::setAccuracy(float accuracy)
{
	this->accuracy = accuracy;
	drawable = false;
}
void st::FunctionGraph::setEpsilon(float epsilon)
{
	this->epsilon = epsilon;
	drawable = false;
}

void st::FunctionGraph::setFont(Font& font)
{
	text_x.setFont(font);
	text_y.setFont(font);

	this->font = &font;
	drawable = false;
}

float st::FunctionGraph::getZoom() const
{
	return zoom;
}

bool st::FunctionGraph::addGraph(const std::string& code, sf::Color line_color)
{
	const std::string fragmentShader =
		"#version 330\n"
		"uniform sampler2D texture;\n"

		"#ifdef GL_ES\n"
		"precision mediump float;\n"
		"#endif\n"

		"uniform vec4      color;"
		"uniform vec2      offset;\n"
		"uniform float     zoom;\n"
		"uniform float     accuracy;\n"
		"uniform float     epsilon;\n"

		"float myF(float x, float y)\n"
		"{\n"
		+
		code
		+
		"}\n"

		"vec4 bruteForce()\n"
		"{\n"
		"    float add_x = 0.0;\n"
		"    float add_y = 0.0;\n"
		"    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);\n"

		"    while(add_x <= epsilon)\n"
		"    {\n"
		"        while(add_y <= epsilon)\n"
		"        {\n"
		"            float r = abs(myF((gl_FragCoord.x + add_x)*zoom - offset.x, (gl_FragCoord.y + add_y)*zoom - offset.y));\n"
		"            if (r <= zoom)\n"
		"			 {\n"
		"                result = color;\n"
		"            }\n"
		"            add_y += accuracy;\n"
		"        }\n"
		"        add_x += accuracy;\n"
		"    }\n"
		"    return result;\n"
		"}\n"

		"void main()\n"
		"{\n"
		"    //vec4 pixel = texture2D(texture, gl_FragCoord.xy / resolution.xy);\n"

		"    gl_FragColor = bruteForce();\n"
		"}\n";

	Shader* sh = new Shader();

	if (!sh->loadFromMemory(fragmentShader, sf::Shader::Fragment))
	{
		cout << "Shader compilation ERROR!" << endl;
		delete sh;
		return false;
	}

	shaders.push_back(sh);
	line_colors.push_back(line_color);
	drawable = false;

	return true;
}
void st::FunctionGraph::ereaseGraph(sf::Uint64 number)
{
	delete shaders[number];
	shaders.erase(shaders.begin() + number);
	line_colors.erase(line_colors.begin() + number);
	drawable = false;
}
void st::FunctionGraph::clearGraph()
{
	for (Uint64 i = 0; i < shaders.size(); ++i)
	{
		delete shaders[i];
	}
	shaders.clear();
	line_colors.clear();


	drawable = false;
}

Uint64 st::FunctionGraph::addPoint(float x, float y, sf::Color color)
{
	return addPoint(Vector2f(x, y), color);
}
Uint64 st::FunctionGraph::addPoint(sf::Vector2f pos, sf::Color color)
{
	Uint64 i = points.size();

	Point p;
	p.pos = pos;
	p.color = color;

	points.push_back(p);
	drawable = false;
	return i;
}
void st::FunctionGraph::ereasePoint(sf::Uint64 number)
{
	points.erase(points.begin() + number);
	drawable = false;
}
void st::FunctionGraph::clearPoint()
{
	points.clear();
	drawable = false;
}

void st::FunctionGraph::event(const sf::Event& event, bool& focus)
{
	Vector2f c_pos(-1, -1); //позиция курсора
	bool o_focus = focus;   //фокус до начала обработки события
	bool ret = false;

	//////////////////////////курсор на объекте?///////////////////////
	//события кнопок
	if (event.type == Event::MouseButtonPressed ||
		event.type == Event::MouseButtonReleased)
	{
		c_pos = Vector2f(float(event.mouseButton.x), float(event.mouseButton.y));
		//На кнопке
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;
	}
	//событие перемещения
	if (event.type == Event::MouseMoved)
	{
		c_pos = Vector2f(float(event.mouseMove.x), float(event.mouseMove.y));
		//На объекте
		if (pos.x <= c_pos.x && c_pos.x <= pos.x + size.x &&
			pos.y <= c_pos.y && c_pos.y <= pos.y + size.y &&
			o_focus == false)
			cursor_on_object = true;
		else
			cursor_on_object = false;
	}

	//constrol
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::LControl)
		key_constrol = true;
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Key::LControl)
		key_constrol = false;

	//up
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::Up)
		key_up = true;
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Key::Up)
		key_up = false;

	//down
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::Down)
		key_doun = true;
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Key::Down)
		key_doun = false;

	//right
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::Right)
		key_right = true;
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Key::Right)
		key_right = false;
	
	//left
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Key::Left)
		key_left = true;
	if (event.type == Event::KeyReleased && event.key.code == Keyboard::Key::Left)
		key_left = false;

	//установка фокуса
	if (!focus)
	{
		focus = cursor_on_object;
	}


	////////////////////////////////////////////////////////////////////////////

	//активность
	if (event.type == Event::MouseButtonPressed)
	{
		if (cursor_on_object && !o_focus)
			activ = true;
		else
			activ = false;
	}
		
	//zoom
	if (event.type == sf::Event::MouseWheelScrolled &&
		key_constrol && cursor_on_object && !o_focus)
	{
		if (event.mouseWheelScroll.delta == 1)
		{
			setZoom(float(zoom * 1.15));
		}
		if (event.mouseWheelScroll.delta == -1)
		{
			setZoom(float(zoom / 1.15));
		}
	}

	//move
	if (event.type == sf::Event::KeyPressed && activ)
	{
		constexpr float speed = 10.0;

		if (key_up)
		{
			offset.y -= speed * zoom;
			drawable = false;
		}
		if (key_doun)
		{
			offset.y += speed * zoom;
			drawable = false;
		}
		if (key_right)
		{
			offset.x -= speed * zoom;
			drawable = false;
		}
		if (key_left)
		{
			offset.x += speed * zoom;
			drawable = false;
		}
	}

	//move mouse
	if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && cursor_on_object)
	{
		mouse_pos = c_pos;
		mouse_move = true;
	}
	if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
	{
		mouse_move = false;
	}
	if (mouse_move)
	{
		if (c_pos != mouse_pos)
			drawable = false;

		offset.x = offset.x + (c_pos.x - mouse_pos.x) * zoom;
		offset.y = offset.y + (-1 * (c_pos.y - mouse_pos.y)) * zoom;
		mouse_pos = c_pos;
	}

	//координаты курсора
	if (cursor_on_object &&	!o_focus)
	{
		cursor_pos = c_pos;
		cursor_show = true;
	}
	else
	{
		cursor_show = false;
	}
}

void st::FunctionGraph::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(background);
	

	if (drawable == false)
		drawGraph();
	graph_sprite.setTexture(graph.getTexture());
	target.draw(graph_sprite);


	//отривсовка координат курсора
	Vector2f cp;
	cp.x = float(cursor_pos.x - graph_sprite.getPosition().x - graph_rectangle.getSize().x / 2.0);
	cp.y = float(cursor_pos.y - graph_sprite.getPosition().y - graph_rectangle.getSize().y / 2.0);

	cp.x = float(cp.x * zoom - offset.x);
	cp.y = float(cp.y * zoom * -1.0 - offset.y);

	if (cursor_show)
	{
		text_x.setString("x: " + to_string(cp.x));
		text_y.setString("y: " + to_string(cp.y));
	}
	else
	{
		text_x.setString("");
		text_y.setString("");
	}
	text_x.setCharacterSize(11);
	text_y.setCharacterSize(11);

	Vector2f size;
	size.x = text_x.getGlobalBounds().height;
	size.y = text_x.getGlobalBounds().width;

	if (size.x < text_y.getGlobalBounds().height)
		size.x = text_y.getGlobalBounds().height;
	size.y += text_y.getGlobalBounds().width;

	text_x.setPosition(graph_sprite.getPosition().x,
		graph_sprite.getPosition().y);
	text_y.setPosition(text_x.getPosition().x,
		float(text_x.getPosition().y + text_x.getGlobalBounds().height + 5.0));

	target.draw(text_x);
	target.draw(text_y);
}

//private
void st::FunctionGraph::drawGraph() const
{
	drawable = true;
	
	graph.clear(Color::Transparent);

	for (Uint64 i = 0; i < shaders.size(); ++i)
	{
		shaders[i]->setParameter("color", line_colors[i]);
		shaders[i]->setUniform("offset", Vector2f(graph_rectangle.getSize().x / 2 * zoom + offset.x,
			graph_rectangle.getSize().y / 2 * zoom + offset.y));
		shaders[i]->setUniform("zoom", zoom);
		shaders[i]->setUniform("accuracy", accuracy);
		shaders[i]->setUniform("epsilon", epsilon);

		graph.draw(graph_rectangle, shaders[i]);
	}

	sf::CircleShape shape(10.f);
	for (Uint64 i = 0; i < points.size(); ++i)
	{
		shape.setPosition(Vector2f(
			float((points[i].pos.x-10.0 * zoom) / zoom + graph_rectangle.getSize().x / 2.0 + offset.x / zoom),
			float((points[i].pos.y+10.0 * zoom)*-1.0 / zoom + graph_rectangle.getSize().x / 2 + offset.y / zoom * -1.0)
		));
		shape.setFillColor(points[i].color);
		graph.draw(shape);
	}

	graph.display();
}

bool st::FunctionGraph::activityZoneCheck(sf::Vector2f cursor)
{
	return background.getGlobalBounds().contains(cursor);
}