#include <SDL.h>
#include <iostream>
#include <vector>
#include <stdlib.h>

const int WIDTH = 512;
const int HEIGHT = 512;
std::vector<int> lines;

class Quad {
public:
	int maxX;
	int maxY;
	int minX;
	int minY;
	Quad(int mx, int my, int x, int y) {
		maxX = x;
		maxY = y;
		minX = mx;
		minY = my;
		topLeft = NULL;
		topRight = NULL;
		botLeft = NULL;
		botRight = NULL;
		lines.push_back(minX);
		lines.push_back(minY);
		lines.push_back(maxX);
		lines.push_back(maxY);
	}
	void insert(int x, int y) {
		if (!inBoundary(x, y)) {
			return;
		}
		if (maxX - minX <= 1) {
			return;
		}
		topLeft = new Quad(minX, minY, (minX + maxX)/2, (minY + maxY)/2);
		botLeft = new Quad(minX, (minY + maxY)/2, (minX + maxX) / 2, maxY);
		topRight = new Quad((minX + maxX) / 2, minY, maxX, (minY + maxY) / 2);
		botRight = new Quad((minX + maxX) / 2, (minY + maxY) / 2, maxX, maxY);
		topLeft->insert(x, y);
		botLeft->insert(x, y);
		topRight->insert(x, y);
		botRight->insert(x, y);
		return;
	}
private:
	Quad* topLeft;
	Quad* topRight;
	Quad* botLeft;
	Quad* botRight;
	bool inBoundary(int x, int y) {
		if (x > maxX - 1 || y > maxY - 1) {
			return false;
		}
		if (x < minX || y < minY) {
			return false;
		}
		return true;
	}
};

int main(int argc, char** argv) {
	SDL_Window* window = SDL_CreateWindow("quads", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer* rend = SDL_CreateRenderer(window, 0, 0);
	bool running = true;
	Quad quadtree(0, 0, WIDTH, HEIGHT);
	int mouseX, mouseY;
	bool newMouseEvent = false;
	while (running) {
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouseX, &mouseY);
				// std::cout << mouseX << " " << mouseY << std::endl;
				newMouseEvent = true;
			}
		}
		if (newMouseEvent) {
			lines.clear();
			quadtree.insert(mouseX, mouseY);
			SDL_Delay(5);
			if (mouseY < 400) {
				mouseY += 1;
			}
			// newMouseEvent = false;
		}
		// drawQuads
		SDL_RenderClear(rend);
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		for (int i = 0; i < lines.size() - 4; i += 4) {
			SDL_RenderDrawLine(rend, lines.at(0 + i), lines.at(1 + i), lines.at(0 + i), lines.at(3 + i));
			SDL_RenderDrawLine(rend, lines.at(0 + i), lines.at(1 + i), lines.at(2 + i), lines.at(1 + i));
			SDL_RenderDrawLine(rend, lines.at(2 + i), lines.at(1 + i), lines.at(2 + i), lines.at(3 + i));
			SDL_RenderDrawLine(rend, lines.at(0 + i), lines.at(3 + i), lines.at(2 + i), lines.at(3 + i));
		}
		SDL_RenderPresent(rend);
	}
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
