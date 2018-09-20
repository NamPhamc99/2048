#include <iostream>
#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <fstream>
#include <algorithm>
#include <ostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "mmsystem.h"

//**************************************************************

struct Location
{
	int x;
	int y;
};

using namespace std;

const int SCREEN_WIDTH = 1152;
const int SCREEN_HEIGHT = 576;
const string WINDOW_TITLE = "2048";

//**************************************************************

void logSDLError(ostream& os,const string &msg, bool fatal) {
	os << msg << " Error: " << SDL_GetError() << endl;
	if (fatal) {
		SDL_Quit();
		exit(1);
	}
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture = nullptr;

	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    texture = SDL_CreateTextureFromSurface(ren, loadedImage);
    SDL_FreeSurface(loadedImage);
	return texture;
}


void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		logSDLError(std::cout, "SDL_Init", true);
	TTF_Init();

	window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Texture *loadText(SDL_Renderer * renderer, const string &text, const string &font, int size) {
	TTF_Font *Font = TTF_OpenFont(font.c_str(), size);
	SDL_Color color = { 0, 168, 0, 0 };
	SDL_Surface *surface = TTF_RenderText_Solid(Font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void renderText(SDL_Texture *text, SDL_Renderer *renderer, string name, int sizeText, int x, int y) {
	text = loadText(renderer, name, "data-latin.ttf", sizeText);
	int textW, textH;
	SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
	renderTexture(text, renderer, x, y, textW, textH);
}

void renderGrid(const vector< vector <int> >& a,SDL_Texture* TemptImage,SDL_Renderer* renderer,Location position[6][6],const int& NumberSquareH,const int& NumberSquareW,const int& thickOfFrame,const int& distanceNumbers,const int& sizeOfMatrix) {
    //Render grid
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            switch (a[i][j]) {
                case 0:
                    TemptImage = loadTexture("null.bmp", renderer);
                    break;
                case 2:
                    TemptImage = loadTexture("2.bmp", renderer);
                    break;
                case 4:
                    TemptImage = loadTexture("4.bmp", renderer);
                    break;
                case 8:
                    TemptImage = loadTexture("8.bmp", renderer);
                    break;
                case 16:
                    TemptImage = loadTexture("16.bmp", renderer);
                    break;
                case 32:
                    TemptImage = loadTexture("32.bmp", renderer);
                    break;
                case 64:
                    TemptImage = loadTexture("64.bmp", renderer);
                    break;
                case 128:
                    TemptImage = loadTexture("128.bmp", renderer);
                    break;
                case 256:
                    TemptImage = loadTexture("256.bmp", renderer);
                    break;
                case 512:
                    TemptImage = loadTexture("512.bmp", renderer);
                    break;
                case 1024:
                    TemptImage = loadTexture("1024.bmp", renderer);
                    break;
                case 2048:
                    TemptImage = loadTexture("2048.bmp", renderer);
                    break;
                }
            renderTexture(TemptImage, renderer, position[i][j].x, position[i][j].y, NumberSquareW, NumberSquareH);
            SDL_RenderPresent(renderer);
            SDL_DestroyTexture(TemptImage);
        }
    }
}
