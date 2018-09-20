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

void PlayMusic (Mix_Music * music ,const char * file ,  int loop  )
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
    {
        printf("%s", Mix_GetError());
    }

    music = Mix_LoadMUS(file) ;
    if (music == NULL)
    {
        printf("%s", Mix_GetError());

    }

    if (!Mix_Playing(-1)) Mix_PlayMusic(music , loop) ;
}

string convertInt(int number) {
    if (number == 0)
        return "0";
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}

void initiate (vector< vector <int> >& a,vector< vector <int> >& undo) {
	//Create grids
	for (int row=0;row<4;row++) {
		for (int col=0;col<4;col++) {
			a[row][col] = 0;
			undo[row][col] = 0; //Create a backup grid
		}
	}

    //Random 2 first positions
    int rowFirst = 0, colFirst = 0, rowSec = 0, colSec = 0;
	rowFirst = rand() % 4;
	colFirst = rand() % 4;
	do {
		rowSec = rand() % 4;
		rowSec = rand() % 4;
	} while (rowSec == rowFirst && rowSec == rowFirst);

    // Set values
	a[rowFirst][colFirst] = 2;
	a[rowSec][colSec] = 2;
}

bool isChanged(const vector< vector <int> >& a,const vector< vector <int> >& undo) {
    //Check if anything changed after processing
    for (int row=0;row<4;row++) {
        for(int col=0;col<4;col++) {
            if (a[row][col] != undo[row][col]) return true;
        }
    }
    return false;
}

void createRandom (vector< vector <int> >& a,const vector< vector <int> >& undo) {
	//Check for blank space
	bool create = false;
	for (int row = 0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (a[row][col] == 0) {
                create = true;
                break;
            }
        }
        if (create) break;
	}

    //Create a new number
	if (create == true && isChanged(a,undo) == true ) { //Check if there's a blank space and if anything moved
        int row = 0,col = 0;
        do {
            row = rand() % 4;
            col = rand() % 4;
        } while (a[row][col] != 0); //Random positions

        int rand_percent;
        rand_percent = rand() % 101;
        if (rand_percent >=60) { //60%->2, 40%->4
            a[row][col] = 2;
        }
        else {
            a[row][col] = 4;
        }
	}
}

void backUp(const vector< vector <int> >& a,vector< vector <int> >& undo) {
    //Backup the grid to check if anything changed
    for (int row=0;row<4;row++) {
        for(int col=0;col<4;col++) {
            undo[row][col] = a[row][col];
        }
    }
}

bool gameWon(const vector< vector <int> >& a) {
    for (int row=0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (a[row][col] == 2048) return true; //check if the game is won
        }
    }
    return false;
}

bool gameOver(const vector< vector <int> >& a) {
    for (int row=0;row<4;row++) {
        for (int col=0;col<4;col++) {
            if (a[row][col] == 0) {
                return false; //Check for any 0
            }
            if (col != 3 && a[row][col] == a[row][col+1]) {
                return false; //Check on rows
            }
            if (row != 3 && a[row][col] == a[row+1][col]) {
                return false; //Check on columns
            }
        }
    }
    return true; //No blank space and every position nearby is different
}

void slideLeft (vector< vector <int> >& a) {
    for (int i=0;i<3;i++) {
        for (int row=0;row<4;row++) {
            for (int col=3;col>=1;col--) {
                if(a[row][col-1] == 0) {
                    a[row][col-1] = a[row][col];
                    a[row][col] = 0;
                }
            }
        }
    }
}

void joinLeft (vector< vector <int> >& a) {
    for (int row=0;row<4;row++) {
        for (int col=0;col<3;col++) {
            if(a[row][col] == a[row][col+1]) {
                a[row][col] += a[row][col+1];
                a[row][col+1] = 0;
            }
        }
    }
}

void flipRow(vector< vector <int> >& a) {
	for (int row=0;row<4;row++) { // Flip every elements on a row
		for (int col=0;col<2;col++) {
            swap(a[row][col],a[row][3-col]);
		}
	}
}

void slideUp(vector< vector <int> >& a) {
    for (int i=0;i<3;i++) {
        for (int col=0;col<4;col++) {
            for (int row=3;row>=1;row--) {
                if (a[row-1][col] == 0) {
                    a[row-1][col] = a[row][col];
                    a[row][col] = 0;
                }
            }
        }
    }
}

void joinUp(vector< vector <int> >& a) {
    for (int col=0;col<4;col++) {
        for (int row=0;row<3;row++) {
            if(a[row][col] == a[row+1][col]) {
                a[row][col] += a[row+1][col];
                a[row+1][col] = 0;
            }
        }
    }
}

void flipCol(vector< vector <int> >& a) {

		reverse(a.begin(),a.end());

}

void process (SDL_Event button,vector< vector <int> >& a,bool& endGame) {
    switch (button.key.keysym.sym) {
            case SDLK_LEFT:
                slideLeft(a);
                joinLeft(a);
                slideLeft(a);
                break;
            case SDLK_RIGHT:
                flipRow(a); //flip the row to use the slideLeft()
                slideLeft(a);
                joinLeft(a);
                slideLeft(a);
                flipRow(a); //flip back
                break;
            case SDLK_UP:
                slideUp(a);
                joinUp(a);
                slideUp(a);
                break;
            case SDLK_DOWN:
                flipCol(a);
                slideUp(a);
                joinUp(a);
                slideUp(a);
                flipCol(a);
                break;
            case SDLK_ESCAPE:
                endGame = true;
                break;
            default:
                break;
    }
}

void mix(vector< vector <int> >& a,int& setMoves,int& countMoves) {
        for (int row=0;row<4;row++) {
            for (int col=0;col<4;col++) {
                //random the position that will be mixed
                int newPosx = rand() % 4;
                int newPosy = rand() % 4;
                //mix 2 positions
                swap(a[row][col],a[newPosx][newPosy]);
            }
        }

        //Adjust the standards
        countMoves = 0;
        setMoves-=2;
        if (setMoves <= 0) setMoves = rand() % 30;
}
