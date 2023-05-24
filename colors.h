#pragma once

class Color {
	public:
		unsigned int color;
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color(unsigned int rgba) {
			this->color = rgba;
			this->a = (rgba & 0xff);
			this->b = ((rgba >> 8) & 0xff);
			this->g = ((rgba >> 16) & 0xff);
			this->r = ((rgba >> 24) & 0xff);
		}

		operator unsigned int() { return this->color; }
};

Color COLOR_RED = Color(0xff0000ff);
Color COLOR_BLUE = Color(0x0000ffff);
Color COLOR_GREEN = Color(0x00ff00ff);
Color COLOR_WHITE = Color(0xffffffff);
Color COLOR_BLACK = Color(0x000000ff);