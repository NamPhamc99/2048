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

//***********************************************

vector< vector<int> > a(4, vector <int> (4));
vector< vector<int> > undo(4, vector <int> (4));
bool isGameOver = false;
bool endGame = false;
bool isGameWon = false;

const int SCREEN_WIDTH = 1152;
const int SCREEN_HEIGHT = 576;
const string WINDOW_TITLE = "2048";
const int thickOfFrame = 9;
const int NumberSquareW = 72;
const int NumberSquareH = 72;
const int distanceNumbers = 80;
const int sizeOfMatrix = 4;


//************************************************

void PlayMusic (Mix_Music * music ,const char * file ,  int loop  );

string convertInt(int number);

bool gameWon(const vector< vector <int> >& a);

void mix(vector< vector <int> >& a,int& setMoves,int& countMoves);

void backUp(const vector< vector <int> >& a,vector< vector <int> >& undo);

bool isChanged(const vector< vector <int> >& a,const vector< vector <int> >& undo);

bool gameOver(const vector< vector <int> >& a);

void initiate (vector< vector <int> >& a,vector< vector <int> >& undo);

void createRandom (vector< vector <int> >& a,const vector< vector <int> >& undo);

void slideLeft (vector< vector <int> >& a);

void joinLeft (vector< vector <int> >& a);

void flipRow(vector< vector <int> >& a);

void slideUp(vector< vector <int> >& a);

void joinUp(vector< vector <int> >& a);

void flipCol(vector< vector <int> >& a);

void process (SDL_Event button,vector< vector <int> >& a,bool& endGame);



