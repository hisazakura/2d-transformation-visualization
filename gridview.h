#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <vector>
#include <string>
#include "easings.h"
#include "graphics.h" // extended graphics
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "algs.h"

using json = nlohmann::json;

class Grid
{
public:
	std::vector<std::string> list_id;
	std::vector<unsigned int> list_create_time;
	std::vector<std::vector<std::vector<int>>> list_geometry;
	unsigned int transform_start;
	// { type, magnitude1, magnitude2 }
	// none = 0, 0 mag
	// translation = 1, 2 mag
	// rotation = 2, 1 mag
	// scaling = 3, 2 mag
	// shearing = 4, 2 mag
	std::vector<float> transform_type = { 0, 0, 0 };
	std::string transform_id;
	std::vector<std::vector<int>> transform_previous;

	Grid();

	bool drawGrid(SDL_Renderer *renderer, unsigned int t, unsigned int duration);

	bool drawGeometry(SDL_Renderer *renderer, unsigned int t, unsigned int duration);

	bool transform(unsigned int t, unsigned int duration);

	void translate(std::string id, float x, float y, unsigned int t);

	void rotate(std::string id, float rot_deg, unsigned int t);

	void scale(std::string id, float x, float y, unsigned int t);

	void addPoint(int x, int y, std::string id, unsigned int t);

	void addPoly(std::string id, std::vector<std::vector<int>> points, unsigned int t);

	void debug() {
		json j;
		j["list_id"] = this->list_id;
		j["list_create_time"] = this->list_create_time;
		j["list_geometry"] = this->list_geometry;
		j["transform_type"] = this->transform_type;
		j["transform_id"] = this->transform_id;
		j["transform_start"] = this->transform_start;
		j["transform_previous"] = this->transform_previous;
		std::ofstream file("log.json");
		if (file.is_open()) {
			file << j;
			file.close();
			std::cout << "Log can be seen in log.json" << std::endl;
		} else {
			std::cerr << "Failed to open file for writing" << std::endl;
		}
	}

	~Grid();
};

bool Grid::transform(unsigned int t, unsigned int duration) {
	if (t < this->transform_start) return false;
	if (this->transform_type[0] == 0) return false;
	else if (t - transform_start < duration) {
		int id_index = -1;
		for (int i = 0;i < this->list_id.size();i++) {
			if (list_id[i] == transform_id) {
				id_index = i;
				break;
			}
		}
		if ((int)this->transform_type[0] == 1) {
			for (int i = 0; i < this->list_geometry[id_index].size(); i++) {
				this->list_geometry[id_index][i][0] = transform_previous[i][0] + (int)((float)(this->transform_type[1])*easeInOutCubic((float)(t - transform_start)/(float)duration));
				this->list_geometry[id_index][i][1] = transform_previous[i][1] + (int)((float)(this->transform_type[2])*easeInOutCubic((float)(t - transform_start)/(float)duration));
			}
		}
		if ((int)this->transform_type[0] == 2) {
			std::vector<std::vector<float>> rot_matrix = {
				{ 	
					(float)cos((float)transform_type[1]*easeInOutCubic((float)(t - transform_start)/(float)duration)*3.14159f/180), 
					(float)sin((float)transform_type[1]*easeInOutCubic((float)(t - transform_start)/(float)duration)*-3.14159f/180), 0 },
				{ 	
					(float)sin((float)transform_type[1]*easeInOutCubic((float)(t - transform_start)/(float)duration)*3.14159f/180), 
					(float)cos((float)transform_type[1]*easeInOutCubic((float)(t - transform_start)/(float)duration)*3.14159f/180), 0 },
				{ 0, 0, 1 } };
			for (int i = 0; i < this->list_geometry[id_index].size(); i++) {
				std::vector<int> point = { transform_previous[i][0], transform_previous[i][1], 1 };
				std::vector<int> rotated = matmul3x1(rot_matrix, point);
				this->list_geometry[id_index][i][0] = rotated[0];
				this->list_geometry[id_index][i][1] = rotated[1];
			}
		}
		if ((int)this->transform_type[0] == 3) {
			std::vector<std::vector<float>> scale_matrix = {
				{ (float)transform_type[1]*easeInOutCubic((float)(t - transform_start)/(float)duration) + 1.0f, 0, 0 },
				{ 0, (float)transform_type[2]*easeInOutCubic((float)(t - transform_start)/(float)duration) + 1.0f, 0 },
				{ 0, 0, 1 } };
			for (int i = 0; i < this->list_geometry[id_index].size(); i++) {
				std::vector<int> point = { transform_previous[i][0], transform_previous[i][1], 1 };
				std::vector<int> scaled = matmul3x1(scale_matrix, point);
				this->list_geometry[id_index][i][0] = scaled[0];
				this->list_geometry[id_index][i][1] = scaled[1];
			}
		}
		return true;
	}
	else return false;
}

void Grid::translate(std::string id, float x, float y, unsigned int t) {
	int id_index = -1;
	for (int i = 0;i < this->list_id.size();i++) {
		if (list_id[i] == id) {
			id_index = i;
			break;
		}
	}
	if (id_index == -1) {
		std::cout << "No such id!" << std::endl;
		return;
	}
	this->transform_id = id;
	this->transform_type = { 1, x, y };
	this->transform_start = t;
	this->transform_previous = list_geometry[id_index];
}

void Grid::rotate(std::string id, float rot_deg, unsigned int t) {
	int id_index = -1;
	for (int i = 0;i < this->list_id.size();i++) {
		if (list_id[i] == id) {
			id_index = i;
			break;
		}
	}
	if (id_index == -1) {
		std::cout << "No such id!" << std::endl;
		return;
	}
	this->transform_id = id;
	this->transform_type = { 2, rot_deg, 0 };
	this->transform_start = t;
	this->transform_previous = list_geometry[id_index];
}

void Grid::scale(std::string id, float x, float y, unsigned int t) {
	int id_index = -1;
	for (int i = 0;i < this->list_id.size();i++) {
		if (list_id[i] == id) {
			id_index = i;
			break;
		}
	}
	if (id_index == -1) {
		std::cout << "No such id!" << std::endl;
		return;
	}
	this->transform_id = id;
	this->transform_type = { 3, x - 1.0f, y - 1.0f };
	this->transform_start = t;
	this->transform_previous = list_geometry[id_index];
}

Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::addPoint(int x, int y, std::string id, unsigned int t) {
	this->list_id.push_back(id);
	this->list_create_time.push_back(t);

	std::vector<int> pos = { x , y };
	std::vector<std::vector<int>> points = { pos };
	this->list_geometry.push_back(points);
}

void Grid::addPoly(std::string id, std::vector<std::vector<int>> points, unsigned int t) {
	this->list_id.push_back(id);
	this->list_create_time.push_back(t);

	this->list_geometry.push_back(points);
}

bool Grid::drawGeometry(SDL_Renderer* renderer, unsigned int t, unsigned int duration) {
	for (int geometry_index = 0; geometry_index < this->list_geometry.size(); geometry_index++) {
		std::vector<int> prev_point = this->list_geometry[geometry_index][this->list_geometry[geometry_index].size()-1];
		for (int points_index = 0; points_index < this->list_geometry[geometry_index].size(); points_index++) {
			if (t - list_create_time[geometry_index] < duration){
				drawFilledEllipse(renderer, 
					this->list_geometry[geometry_index][points_index][0] + 350,
					this->list_geometry[geometry_index][points_index][1] + 350,
					(unsigned int)(easeOutElastic((float)(t - list_create_time[geometry_index])/(float)duration)*7.0f),
					(unsigned int)(easeOutElastic((float)(t - list_create_time[geometry_index])/(float)duration)*7.0f),
					true,
					0xffffffff);
				int offsetX = this->list_geometry[geometry_index][points_index][0] - prev_point[0];
				int offsetY = this->list_geometry[geometry_index][points_index][1] - prev_point[1];
				SDL_RenderDrawLine(renderer, 
					prev_point[0] + 350, 
					prev_point[1] + 350, 
					prev_point[0] + 350 + (int)(easeOutExpo((float)(t - list_create_time[geometry_index])/(float)duration)*(float)offsetX), 
					prev_point[1] + 350 + (int)(easeOutExpo((float)(t - list_create_time[geometry_index])/(float)duration)*(float)offsetY));
			}
			else if (t - list_create_time[geometry_index] >= 0) {
				drawFilledEllipse(renderer, 
					this->list_geometry[geometry_index][points_index][0] + 350,
					this->list_geometry[geometry_index][points_index][1] + 350,
					7, 7, true,
					0xffffffff);
				SDL_RenderDrawLine(renderer, 
					prev_point[0] + 350, 
					prev_point[1] + 350, 
					this->list_geometry[geometry_index][points_index][0] + 350,
					this->list_geometry[geometry_index][points_index][1] + 350);
			}
			prev_point = this->list_geometry[geometry_index][points_index];
		}
	}
}

bool Grid::drawGrid(SDL_Renderer *renderer, unsigned int t, unsigned int duration)
{
	if (t < duration)
	{

		// sub grid 2
		unsigned int interval = (unsigned int)((float)duration / 70.0f);
		SDL_SetRenderDrawColor(renderer, 17, 40, 48, 255);
		for (int i = 0; i < 75; i++)
		{
			SDL_RenderDrawLine(renderer, 0, 20 * i - 50, (int)(easeInOutCubic((float)std::max((unsigned int)0, t - i * interval) / (float)(duration - i * interval)) * 700.0f), 20 * i - 50);
			SDL_RenderDrawLine(renderer, 20 * i - 50, 0, 20 * i - 50, (int)(easeInOutCubic((float)std::max((unsigned int)0, t - i * interval) / (float)(duration - i * interval)) * 700.0f));
		}

		// sub grid 1
		interval = (unsigned int)((float)duration / 14.0f);
		SDL_SetRenderDrawColor(renderer, 39, 148, 187, 255);
		for (int i = 0; i < 15; i++)
		{
			SDL_RenderDrawLine(renderer, 0, 100 * i - 50, (int)(easeInOutCubic((float)std::max((unsigned int)0, t - i * interval) / (float)(duration - i * interval)) * 700.0f), 100 * i - 50);
			SDL_RenderDrawLine(renderer, 100 * i - 50, 0, 100 * i - 50, (int)(easeInOutCubic((float)std::max((unsigned int)0, t - i * interval) / (float)(duration - i * interval)) * 700.0f));
		}

		// main axis
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect = {350, (int)((1 - easeInOutCubic((float)t / (float)duration)) * 350), 4, (int)(easeInOutCubic((float)t / (float)duration) * 700)};
		SDL_RenderFillRect(renderer, &rect);
		rect = {(int)((1 - easeInOutCubic((float)t / (float)duration)) * 350), 350, (int)(easeInOutCubic((float)t / (float)duration) * 700), 4};
		SDL_RenderFillRect(renderer, &rect);

		return true;
	}
	else
	{

		// sub grid 2
		SDL_SetRenderDrawColor(renderer, 17, 40, 48, 255);
		for (int i = 0; i < 75; i++)
		{
			SDL_RenderDrawLine(renderer, 0, 20 * i - 50, 700, 20 * i - 50);
			SDL_RenderDrawLine(renderer, 20 * i - 50, 0, 20 * i - 50, 700);
		}

		// sub grid 1
		SDL_SetRenderDrawColor(renderer, 39, 148, 187, 255);
		for (int i = 0; i < 14; i++)
		{
			SDL_RenderDrawLine(renderer, 0, 100 * i - 50, 700, 100 * i - 50);
			SDL_RenderDrawLine(renderer, 100 * i - 50, 0, 100 * i - 50, 700);
		}

		// main axis
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect = {350, 0, 4, 700};
		SDL_RenderFillRect(renderer, &rect);
		rect = {0, 350, 700, 4};
		SDL_RenderFillRect(renderer, &rect);

		return false;
	}
}