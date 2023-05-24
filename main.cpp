#include <iostream>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include "gridview.h"
#include "commands.h"
#include <cstdlib>

const int WIDTH = 700, HEIGHT = 700;

SDL_Window *window = SDL_CreateWindow("Transformation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
Grid grid;

bool is_animating = false;
bool is_listening = false;

bool running = true;

typedef std::chrono::milliseconds ms;

std::string input = "";
bool is_input_given = false;
bool is_input_received = true;

std::vector<std::string> input_queue = {};

std::vector<std::string> parsed_input;

void readInput()
{
	while (running)
	{
		std::getline(std::cin, input);
		input_queue.push_back(input);
	}
}

void parseInput(ms t)
{
	input.push_back(' '); // prevent error at parsing

	parsed_input = Command().split(input, " ");

	// new command
	if (parsed_input[0] == "new")
	{
		// new point
		if (parsed_input[1] == "point")
		{
			std::vector<int> point = {};
			std::string id = "";
			for (int i = 2; i < parsed_input.size(); i++)
			{
				if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
				{
					parsed_input[i].erase(0, 1).pop_back();
					id = parsed_input[i];
				}
				if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
				{
					parsed_input[i].erase(0, 1).pop_back();
					try
					{
						point = {stoi(Command().split(parsed_input[i], ",")[0]), stoi(Command().split(parsed_input[i], ",")[1])};
					}
					catch (const std::invalid_argument &e)
					{
						std::cout << "Parsing command failed: Invalid Argument" << std::endl;
						return;
					}
					catch (const std::out_of_range &e)
					{
						std::cout << "Parsing command failed: Out of Range!" << std::endl;
						return;
					}
				}
			}
			if (point.empty())
			{
				std::cout << "Missing input: point" << std::endl;
			}
			if (id == "")
			{
				std::cout << "Missing input: id" << std::endl;
			}
			if (!point.empty() && id != "")
			{
				grid.addPoint(point[0] * 100, point[1] * -100, id, t.count());
			}
		}

		// new poly
		if (parsed_input[1] == "poly")
		{
			std::vector<std::vector<int>> points = {};
			std::vector<int> point = {};
			std::string id = "";
			for (int i = 2; i < parsed_input.size(); i++)
			{
				if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
				{
					parsed_input[i].erase(0, 1).pop_back();
					id = parsed_input[i];
				}
				if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
				{
					parsed_input[i].erase(0, 1).pop_back();
					try
					{
						point = {stoi(Command().split(parsed_input[i], ",")[0])*100, stoi(Command().split(parsed_input[i], ",")[1])*-100};
					}
					catch (const std::invalid_argument &e)
					{
						std::cout << "Parsing command failed: Invalid Argument" << std::endl;
						return;
					}
					catch (const std::out_of_range &e)
					{
						std::cout << "Parsing command failed: Out of Range!" << std::endl;
						return;
					}
					points.push_back(point);
				}
			}
			if (points.empty())
			{
				std::cout << "Missing input: points" << std::endl;
			}
			if (id == "")
			{
				std::cout << "Missing input: id" << std::endl;
			}
			if (!points.empty() && id != "")
			{
				grid.addPoly(id, points, t.count());
			}
		}

		// new poly
		if (parsed_input[1] == "random")
		{
			std::string id = "";
			std::vector<std::vector<int>> points = {};
			int value = 4;
			for (int i = 2; i < parsed_input.size(); i++)
			{
				if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
				{
					parsed_input[i].erase(0, 1).pop_back();
					id = parsed_input[i];
				}
				try
				{
					value = stoi(parsed_input[i]);
				}
				catch (const std::invalid_argument &e)
				{
					continue;
				}
				catch (const std::out_of_range &e)
				{
					continue;
				}
			}
			if (id == "")
			{
				std::cout << "Missing input: id" << std::endl;
			}
			if (id != "")
			{
				std::srand(t.count());
				for (int i = 0; i < value; i++) {
					std::vector<int> point = { ((std::rand() % 7) - 3)*100, ((std::rand() % 7) - 3)*100 };
					points.push_back(point);
				}
				grid.addPoly(id, points, t.count());
			}
		}

		// new circle
		if (parsed_input[1] == "circle")
		{
			std::string id = "";
			std::vector<float> point = { 0, 0 };
			float rad = 1;
			int subdiv = 16;
			for (int i = 2; i < parsed_input.size(); i++)
			{
				if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
				{
					parsed_input[i].erase(0, 1).pop_back();
					id = parsed_input[i];
				}

				if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
				{
					parsed_input[i].erase(0, 1).pop_back();
					try
					{
						point = {stof(Command().split(parsed_input[i], ",")[0]), stof(Command().split(parsed_input[i], ",")[1])};
					}
					catch (const std::invalid_argument &e)
					{
						std::cout << "Parsing command failed: Invalid Argument" << std::endl;
						return;
					}
					catch (const std::out_of_range &e)
					{
						std::cout << "Parsing command failed: Out of Range!" << std::endl;
						return;
					}
				}

				// rad
				if (parsed_input[i][0] == 'r')
				{
					parsed_input[i].erase(0, 1);
					try
					{
						rad = stof(parsed_input[i]);
					}
					catch (const std::invalid_argument &e)
					{
						continue;
					}
					catch (const std::out_of_range &e)
					{
						continue;
					}
				}

				// subdivide
				if (parsed_input[i][0] == 's')
				{
					parsed_input[i].erase(0, 1);
					try
					{
						subdiv = stoi(parsed_input[i]);
					}
					catch (const std::invalid_argument &e)
					{
						continue;
					}
					catch (const std::out_of_range &e)
					{
						continue;
					}
				}
				
			}
			if (id == "")
			{
				std::cout << "Missing input: id" << std::endl;
			}
			else
			{
				std::vector<std::vector<int>> points;
				float angleIncrement = 2 * M_PI / subdiv;
				for (int i = 0; i < subdiv; i++) {
					float angle = i * angleIncrement;
					float x = point[0] + rad * cos(angle);
					float y = -point[1] + rad * sin(angle);
					points.push_back({ (int)(x*100), (int)(y*100) });
				}
				grid.addPoly(id, points, t.count());
			}
		}
	}
	else if (parsed_input[0] == "translate" || parsed_input[0] == "tr")
	{
		bool err = false;
		std::vector<float> point = {};
		std::string id = "";
		for (int i = 1; i < parsed_input.size(); i++)
		{
			if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
			{
				parsed_input[i].erase(0, 1).pop_back();
				id = parsed_input[i];
			}
			if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
			{
				parsed_input[i].erase(0, 1).pop_back();
				try
				{
					point = {stof(Command().split(parsed_input[i], ",")[0]), stof(Command().split(parsed_input[i], ",")[1])};
				}
				catch (const std::invalid_argument &e)
				{
					std::cout << "Parsing command failed: Invalid Argument" << std::endl;
					return;
				}
				catch (const std::out_of_range &e)
				{
					std::cout << "Parsing command failed: Out of Range!" << std::endl;
					return;
				}
			}
		}
		if (point.empty())
		{
			std::cout << "Missing input: point" << std::endl;
		}
		if (id == "")
		{
			std::cout << "Missing input: id" << std::endl;
		}
		if (!point.empty() && id != "" && !err)
		{
			grid.translate(id, point[0] * 100, point[1] * -100, t.count());
		}
	}
	else if (parsed_input[0] == "rotate" || parsed_input[0] == "rot")
	{
		float rot = 0;
		std::vector<float> point = {};
		std::string id = "";
		for (int i = 1; i < parsed_input.size(); i++)
		{
			if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
			{
				parsed_input[i].erase(0, 1).pop_back();
				id = parsed_input[i];
				continue;
			}
			if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
			{
				parsed_input[i].erase(0, 1).pop_back();
				try
				{
					point = {stof(Command().split(parsed_input[i], ",")[0]), stof(Command().split(parsed_input[i], ",")[1])};
				}
				catch (const std::invalid_argument &e)
				{
					std::cout << "Parsing command failed: Invalid Argument" << std::endl;
					return;
				}
				catch (const std::out_of_range &e)
				{
					std::cout << "Parsing command failed: Out of Range!" << std::endl;
					return;
				}
				continue;
			}
			try
			{
				rot = stof(parsed_input[i]);
			}
			catch (const std::invalid_argument &e)
			{
				continue;
			}
			catch (const std::out_of_range &e)
			{
				continue;
			}
		}
		if (id == "")
		{
			std::cout << "Missing input: id" << std::endl;
		}
		else if (point.size() == 0)
		{
			grid.rotate(id, -rot, t.count());
		}
		else {
			input_queue.push_back("tr ["+ id +"] (" + to_string(-point[0]) + "," + to_string(-point[1]) + ")");
			std::cout << "tr ["+ id +"] (" + to_string(-point[0]) + "," + to_string(-point[1]) + ")" << std::endl;
			input_queue.push_back("rot ["+ id + "] " + to_string(rot));
			std::cout << "rot ["+ id + "] " + to_string(rot) << std::endl;
			input_queue.push_back("tr ["+ id +"] (" + to_string(point[0]) + "," + to_string(point[1]) + ")");
			std::cout << "tr ["+ id +"] (" + to_string(point[0]) + "," + to_string(point[1]) + ")" << std::endl;
		}
	}
	else if (parsed_input[0] == "scale" || parsed_input[0] == "sc")
	{
		std::vector<float> point = {};
		std::string id = "";
		for (int i = 1; i < parsed_input.size(); i++)
		{
			if (parsed_input[i][0] == '[' && parsed_input[i][parsed_input[i].size() - 1] == ']')
			{
				parsed_input[i].erase(0, 1).pop_back();
				id = parsed_input[i];
			}
			if (parsed_input[i][0] == '(' && parsed_input[i][parsed_input[i].size() - 1] == ')')
			{
				parsed_input[i].erase(0, 1).pop_back();
				try
				{
					point = {stof(Command().split(parsed_input[i], ",")[0]), stof(Command().split(parsed_input[i], ",")[1])};
				}
				catch (const std::invalid_argument &e)
				{
					std::cout << "Parsing command failed: Invalid Argument" << std::endl;
					return;
				}
				catch (const std::out_of_range &e)
				{
					std::cout << "Parsing command failed: Out of Range!" << std::endl;
					return;
				}
			}
		}
		if (id == "")
		{
			std::cout << "Missing input: id" << std::endl;
		}
		else if (!point.empty())
		{
			grid.scale(id, point[0], point[1], t.count());
		}
	}
	else if (parsed_input[0] == "debug")
	{
		grid.debug();
	}
	else if (parsed_input[0] == "help")
	{
		if (parsed_input[1] == "new") {
			std::cout << "Command usage: new type (point(s)) [id]" << std::endl;
			std::cout << std::endl;
			std::cout << "Available args for type:" << std::endl;
			std::cout << "  point \tTakes one point input and create a point in the grid." << std::endl;
			std::cout << "  poly \tTakes one or more points input and create a polygon in the grid." << std::endl;
			std::cout << "  circle \tTakes 2 numbers and one point input and create a circle in the grid by subdivision." << std::endl;
			std::cout << "         \tArgs: r(radius) s(subdivision) (center point)." << std::endl;
			std::cout << "  random \tTakes 1 number input and create a polygon with random points in the grid." << std::endl;
			std::cout << std::endl;
			std::cout << "(point(s)) is a coordinate fomatted like this:" << std::endl;
			std::cout << "  (x,y)" << std::endl;
			std::cout << "Make sure there is no space." << std::endl;
			std::cout << std::endl;
			std::cout << "[id] is an id given for the geometry." << std::endl;
			std::cout << std::endl;
			std::cout << "Arguments or parameters can be in any order" << std::endl;
			std::cout << std::endl;
			std::cout << "Examples:" << std::endl;
			std::cout << "  new point (0,0) [origin]" << std::endl;
			std::cout << "  new poly (1,1) (-1,1) (-1,-1) (1,-1) [square]" << std::endl;
			std::cout << "  new circle r2 s24 (1,1) [circle]" << std::endl;
			std::cout << "  new random 5 [random]" << std::endl;
		}
		else if (parsed_input[1] == "translate" || parsed_input[1] == "tr") {
			std::cout << "Command usage: translate [id] (point)" << std::endl;
			std::cout << std::endl;
			std::cout << "Aliases:" << std::endl;
			std::cout << "  tr" << std::endl;
			std::cout << std::endl;
			std::cout << "(point) is a coordinate fomatted like this:" << std::endl;
			std::cout << "  (x,y)" << std::endl;
			std::cout << "Make sure there is no space." << std::endl;
			std::cout << std::endl;
			std::cout << "[id] is an id given for the geometry." << std::endl;
			std::cout << std::endl;
			std::cout << "Arguments or parameters can be in any order" << std::endl;
			std::cout << std::endl;
			std::cout << "Examples:" << std::endl;
			std::cout << "  translate [square] (1,2)" << std::endl;
			std::cout << "  tr (1,2) [foo]" << std::endl;
		}
		else if (parsed_input[1] == "rotate" || parsed_input[1] == "rot") {
			std::cout << "Command usage: rotate [id] degrees (point)" << std::endl;
			std::cout << std::endl;
			std::cout << "Aliases:" << std::endl;
			std::cout << "  rot" << std::endl;
			std::cout << std::endl;
			std::cout << "(point) is a coordinate fomatted like this:" << std::endl;
			std::cout << "  (x,y)" << std::endl;
			std::cout << "Make sure there is no space." << std::endl;
			std::cout << "Only integers are currently supported." << std::endl;
			std::cout << "Optional, default value is (0,0)." << std::endl;
			std::cout << std::endl;
			std::cout << "[id] is an id given for the geometry." << std::endl;
			std::cout << std::endl;
			std::cout << "degrees is magnitude of rotation in degrees." << std::endl;
			std::cout << "Note that it doesn't do full circle rotation for 360 degrees or more." << std::endl;
			std::cout << std::endl;
			std::cout << "Arguments or parameters can be in any order" << std::endl;
			std::cout << std::endl;
			std::cout << "Examples:" << std::endl;
			std::cout << "  rotate [square] 45" << std::endl;
			std::cout << "  rot  15 [foo] (1,1)" << std::endl;
		}
		else {
			std::cout << "Transform Visualizer is a command-based 2D transformation visualizer." << std::endl;
			std::cout << std::endl;
			std::cout << "Quick start commands to create and rotate a square:" << std::endl;
			std::cout << "  new poly (1,1) (-1,1) (-1,-1) (1,-1) [square]" << std::endl;
			std::cout << "  rot [square] 45" << std::endl;
			std::cout << std::endl;
			std::cout << "Available commands:" << std::endl;
			std::cout << "  help \tShows this text." << std::endl;
			std::cout << "  new \tCreates a new geometry." << std::endl;
			std::cout << "  translate \tTranslates or move a geometry with a point input.\tAlias: tr" << std::endl;
			std::cout << "  rotate \tRotates a geometry with a point input and a number input.\tAlias: rot" << std::endl;
			std::cout << std::endl;
			std::cout << "For more about a command, type \"help [command]\"" << std::endl;
		}
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);

	// startup text
	std::cout << "Transform Visualizer by hisazakura" << std::endl;
	std::cout << "Type \"help\" for help." << std::endl;

	std::thread inputThread(readInput);

	if (window == NULL)
	{
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Event windowEvent;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	

	auto start_time = std::chrono::system_clock::now();
	ms t = std::chrono::duration_cast<ms>(std::chrono::system_clock::now() - start_time);
	while (running)
	{
		t = std::chrono::duration_cast<ms>(std::chrono::system_clock::now() - start_time);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
			{
				running = false;
			}
		}
		is_animating = grid.drawGrid(renderer, t.count(), 1000);
		if (!is_animating)
			is_animating = grid.drawGeometry(renderer, t.count(), 500);
		if (!is_animating)
			is_animating = grid.transform(t.count(), 1000);

		if (!is_animating) {
			if (input_queue.size() > 0) {
				input = input_queue[0];
				input_queue.erase(input_queue.begin());
				is_input_given = true;
			}
			is_listening = false;
		}

		// animation end event
		else if (!is_animating && !is_listening)
		{
			is_listening = true;
		}

		// input received event
		if (is_input_given)
		{
			
			parseInput(t);

			input = "";

			is_input_given = false;
			is_input_received = true;
		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	inputThread.join();

	return 0;
}