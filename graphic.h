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

using namespace std;

//******************************************

struct Location
{
	int x;
	int y;
};

//*******************************************

void logSDLError(ostream& os,const string &msg, bool fatal);

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

SDL_Texture *loadText(SDL_Renderer * renderer, const string &text, const string &font, int size);

void renderText(SDL_Texture *text, SDL_Renderer *renderer, string name, int sizeText, int x, int y);

void renderGrid(const vector< vector <int> >& a,SDL_Texture* TemptImage,SDL_Renderer* renderer,Location position[6][6],const int& NumberSquareH,const int& NumberSquareW,const int& thickOfFrame,const int& distanceNumbers,const int& sizeOfMatrix);
