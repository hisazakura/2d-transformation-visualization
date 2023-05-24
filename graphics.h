#pragma once

#include <SDL2/SDL.h>
#include <math.h>
#include "colors.h"

/**
 * Draws a rectangle.
 *
 * Draws a rectangle with specified specifications.
 *
 * @param renderer SDL Renderer to render the rectangle.
 * @param x X.
 * @param y Y.
 * @param centered If true then x and y will be the center point.
 * @param w Width.
 * @param h Height.
 * @param fill If true will fill the rectangle (ignoring line width value).
 * @param lw Line width.
 * @param color Color encoded in 32bit uint.
 */
void drawRect(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, bool centered = false, unsigned int w = 0, unsigned int h = 0, bool fill = false, unsigned int lw = 1, unsigned int color = 0) {
	Color Color(color);
	SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
	if (centered) {
		x -= w/2;
		y -= h/2;
	}
	SDL_Rect rect = { (int)x, (int)y, (int)w, (int)h };
	if (fill) {
		SDL_RenderFillRect(renderer, &rect);
		return;
	}
	if (lw < 1) {
		lw = 1;
	}
	for (int i = 1; i <= lw; i++){
		SDL_RenderDrawRect(renderer, &rect);
		if (rect.w - 2 < 0 || rect.h - 2 < 0) {
			break;
		}
		rect.w -= 2;
		rect.h -=2;
		rect.x++;
		rect.y++;
	}
}

/**
 * Draws a one width circle.
 *
 * Draws a circle with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param radius radius.
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawSingleCircle(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int radius = 0, bool is_even = false, unsigned int color = 0) {
    Color Color(color);
	SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);

	int d = (5 - radius * 4) / 4;
    int xPos = 0;
    int yPos = radius;

    do {
		if (is_even) {
			SDL_RenderDrawPoint(renderer, x + xPos, y + yPos);
			SDL_RenderDrawPoint(renderer, x - xPos - 1, y + yPos);
			SDL_RenderDrawPoint(renderer, x + xPos, y - yPos - 1);
			SDL_RenderDrawPoint(renderer, x - xPos - 1, y - yPos - 1);
			SDL_RenderDrawPoint(renderer, x + yPos, y + xPos);
			SDL_RenderDrawPoint(renderer, x - yPos - 1, y + xPos);
			SDL_RenderDrawPoint(renderer, x + yPos, y - xPos - 1);
			SDL_RenderDrawPoint(renderer, x - yPos - 1, y - xPos - 1);
		}
		else{
			SDL_RenderDrawPoint(renderer, x + xPos, y + yPos);
			SDL_RenderDrawPoint(renderer, x - xPos, y + yPos);
			SDL_RenderDrawPoint(renderer, x + xPos, y - yPos);
			SDL_RenderDrawPoint(renderer, x - xPos, y - yPos);
			SDL_RenderDrawPoint(renderer, x + yPos, y + xPos);
			SDL_RenderDrawPoint(renderer, x - yPos, y + xPos);
			SDL_RenderDrawPoint(renderer, x + yPos, y - xPos);
			SDL_RenderDrawPoint(renderer, x - yPos, y - xPos);
		}

        if (d < 0) {
            d += 2 * xPos + 1;
        } else {
            d += 2 * (xPos - yPos) + 1;
            yPos--;
        }
        xPos++;
    } while (xPos <= yPos);
}

/**
 * Draws a filled circle.
 *
 * Draws a circle with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param radius radius
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawFilledCircle(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int radius = 0, bool is_even = false, unsigned int color = 0) {
	Color Color(color);
	SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
	if (is_even){
		for (int w = radius * 2; w >= 0; w--) {
			int h = sqrt((radius * radius) - ((radius - w) * (radius - w)));
			SDL_RenderDrawLine(renderer, x - h, y - radius + w, x + h + 1, y - radius + w);
		}
	}
	else {
		for (int w = radius * 2; w >= 0; w--) {
			int h = sqrt((radius * radius) - ((radius - w) * (radius - w)));
			SDL_RenderDrawLine(renderer, x - h, y - radius + w, x + h, y - radius + w);
		}
	}
}

/**
 * Draws a circle.
 *
 * Draws a circle with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param redius radius.
 * @param fill If true will fill the ellipse (ignoring line width value).
 * @param lw Line width.
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawCircle(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int radius = 0, bool fill = false, unsigned int lw = 1, bool is_even = false, unsigned int color = 0) {
	if (fill) {
		drawFilledCircle(renderer, x, y, radius, is_even, color);
		return;
	}
	if (lw < 1) {
		lw = 1;
	}
	for (int i = 1; i <= lw; i++) {
		drawSingleCircle(renderer, x, y, radius, is_even, color);
		radius--;
	}
}

/**
 * Draws a filled ellipse.
 *
 * Draws an ellipse with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param width x radius.
 * @param height y radius.
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawFilledEllipse(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int width = 0, unsigned int height = 0, bool is_even = false, unsigned int color = 0) {
	if (width == height) {
		drawFilledCircle(renderer, x, y, width, is_even, color);
		return;
	}
	Color Color(color);
	SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
	
	long a2 = width * width;
	long b2 = height * height;
	long fa2 = 4 * a2, fb2 = 4 * b2;
	long x1, y1, sigma;

	// Draw the first half of the ellipse
	for (x1 = 0, y1 = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2*x1 <= a2*y1; x1++) {
		if (is_even) {
			SDL_RenderDrawLine(renderer, x - x1 - 1, y - y1 - 1, x + x1, y - y1 - 1);
			SDL_RenderDrawLine(renderer, x - x1 - 1, y + y1, x + x1, y + y1);
		}
		else {
			SDL_RenderDrawLine(renderer, x - x1, y - y1, x + x1, y - y1);
			SDL_RenderDrawLine(renderer, x - x1, y + y1, x + x1, y + y1);
		}
		if (sigma >= 0) {
			sigma += fa2 * (1 - y1);
			y1--;
		}
		sigma += b2 * ((4 * x1) + 6);
	}

	// Draw the second half of the ellipse
	for (x1 = width, y1 = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2*y1 <= b2*x1; y1++) {
		if (is_even) {
			SDL_RenderDrawLine(renderer, x - x1 - 1, y - y1 - 1, x + x1, y - y1 - 1);
			SDL_RenderDrawLine(renderer, x - x1 - 1, y + y1, x + x1, y + y1);
		}
		else {
			SDL_RenderDrawLine(renderer, x - x1, y - y1, x + x1, y - y1);
			SDL_RenderDrawLine(renderer, x - x1, y + y1, x + x1, y + y1);
		}
		if (sigma >= 0) {
			sigma += fb2 * (1 - x1);
			x1--;
		}
		sigma += a2 * ((4 * y1) + 6);
	}
}

/**
 * Draws a one width ellipse.
 *
 * Draws an ellipse with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param width x radius.
 * @param height y radius.
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawSingleEllipse(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int width = 0, unsigned int height = 0, bool is_even = false, unsigned int color = 0) {
	Color Color(color);
	SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
	
	long a2 = width * width;
	long b2 = height * height;
	long fa2 = 4 * a2, fb2 = 4 * b2;
	long x1, y1, sigma;

	// Draw the first half of the ellipse
	for (x1 = 0, y1 = height, sigma = 2 * b2 + a2 * (1 - 2 * height); b2*x1 <= a2*y1; x1++) {
		if (is_even) {
			SDL_RenderDrawPoint(renderer, x - x1 - 1, y - y1 - 1);
			SDL_RenderDrawPoint(renderer, x + x1, y - y1 - 1);
			SDL_RenderDrawPoint(renderer, x - x1 - 1, y + y1);
			SDL_RenderDrawPoint(renderer, x + x1, y + y1);
		} 
		else {
			SDL_RenderDrawPoint(renderer, x - x1, y - y1);
			SDL_RenderDrawPoint(renderer, x + x1, y - y1);
			SDL_RenderDrawPoint(renderer, x - x1, y + y1);
			SDL_RenderDrawPoint(renderer, x + x1, y + y1);
		}
		if (sigma >= 0) {
			sigma += fa2 * (1 - y1);
			y1--;
		}
		sigma += b2 * ((4 * x1) + 6);
	}

	// Draw the second half of the ellipse
	for (x1 = width, y1 = 0, sigma = 2 * a2 + b2 * (1 - 2 * width); a2*y1 <= b2*x1; y1++) {
		if (is_even) {
			SDL_RenderDrawPoint(renderer, x - x1 - 1, y - y1 - 1);
			SDL_RenderDrawPoint(renderer, x + x1, y - y1 - 1);
			SDL_RenderDrawPoint(renderer, x - x1 - 1, y + y1);
			SDL_RenderDrawPoint(renderer, x + x1, y + y1);
		} 
		else {
			SDL_RenderDrawPoint(renderer, x - x1, y - y1);
			SDL_RenderDrawPoint(renderer, x + x1, y - y1);
			SDL_RenderDrawPoint(renderer, x - x1, y + y1);
			SDL_RenderDrawPoint(renderer, x + x1, y + y1);
		}
		if (sigma >= 0) {
			sigma += fb2 * (1 - x1);
			x1--;
		}
		sigma += a2 * ((4 * y1) + 6);
	}
}

/**
 * Draws an ellipse.
 *
 * Draws an ellipse with specified specifications.
 *
 * @param renderer SDL Renderer to render the ellipse.
 * @param x X.
 * @param y Y.
 * @param width x radius.
 * @param height y radius.
 * @param fill If true will fill the ellipse (ignoring line width value).
 * @param lw Line width.
 * @param is_even Fixes ellipse for symmetrical look
 * @param color Color encoded in 32bit uint.
 */
void drawEllipse(SDL_Renderer* renderer, unsigned int x = 0, unsigned int y = 0, unsigned int width = 0, unsigned int height = 0, bool fill = false, unsigned int lw = 1, bool is_even = false, unsigned int color = 0) {
	if (width == height) {
		drawCircle(renderer, x, y, width, fill, lw, is_even, color);
		return;
	}
	if (fill) {
		drawFilledEllipse(renderer, x, y, width, height, is_even, color);
		return;
	}
	if (lw < 1) {
		lw = 1;
	}
	for (int i = 1; i <= lw; i++) {
		drawSingleEllipse(renderer, x, y, width, height, is_even, color);
		width--;
		height--;
	}
}