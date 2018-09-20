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
#include "gameplay.h"
#include "graphic.h"
#include <SDL_mixer.h>

using namespace std;

const char theme[] = "Theme_song.wav" ;
const char easytheme[] = "Easy_theme.wav";
const char hardtheme[] = "Hard_theme.wav";

int main(int arc, char* argv[]) {
    srand(time(0));

    //Graphic setup
    SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture *background;
	SDL_Texture *TemptImage = NULL;
	SDL_Texture *text = NULL;
	Mix_Music *music = NULL;
	SDL_Event e;
	Location position[sizeOfMatrix + 2][sizeOfMatrix + 2];
	initSDL(window, renderer);
	SDL_Init(SDL_INIT_AUDIO);

    PlayMusic(music,theme,-1);

    int frameX = SCREEN_WIDTH / 2 - 333 / 2;
    int frameY = SCREEN_HEIGHT / 2 - 333 / 2;

    //Get location for the elements of the grid
    position[0][0].x = frameX + thickOfFrame;
    position[0][0].y = frameY + thickOfFrame;
    for (int i = 0; i < sizeOfMatrix + 1; i++) {
        for (int j = 0; j < sizeOfMatrix + 1; j++) {
            position[i][j + 1].x = position[i][j].x + distanceNumbers;
            position[i][j + 1].y = position[i][j].y;
        }
        position[i + 1][0].y = position[i][0].y + distanceNumbers;
        position[i + 1][0].x = position[i][0].x;
    }

    //Begin
    bool isRun = true;
    while (isRun) {
        //Menu
        SDL_RenderClear(renderer);
        background = loadTexture("MenuBackground.bmp", renderer);
        renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        renderText(text, renderer, "2048", 120, 460, 80);
        renderText(text, renderer, "EASY MODE", 30, 200, 300);
        renderText(text, renderer, "HARDCORE ?? ARE YOU SURE ??", 30, 200, 400);
        renderText(text, renderer, "Exit", 30, 200, 500);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(text);

        bool mouseClick = false;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_MOUSEBUTTONDOWN) { //Get option
                mouseClick = true;
                break;
            }
        }
        if (mouseClick) {
            //Easy mode
            if (200 <= e.motion.x && e.motion.x <= 381 && 309 <= e.motion.y && e.motion.y <= 327) {
                Mix_HaltMusic();
                PlayMusic(music,easytheme,-1);
                //Set conditions
                endGame = false;
                isGameOver = false;
                isGameWon = false;

                //Prepare for the game

                //Render background
                SDL_RenderClear(renderer);
                background = loadTexture("Background.bmp", renderer);
                renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(background);

                renderText(text, renderer, "Your Score: ", 30, 800, 150);
                SDL_DestroyTexture(text);
                renderText(text, renderer, "High Score: ", 30, 800, 250);
                SDL_DestroyTexture(text);


                int sc = 0;
                string score;
                string showHScore;
                ifstream file;
                int text1W, text1H;
                //Get high score
                file.open("HighScore.txt");
                int HighScore;
                file >> HighScore;
                file.close();

                //Render score and high score
                TemptImage = loadTexture("null.bmp", renderer);
                renderTexture(TemptImage, renderer, 980, 140, 1000, 60);
                SDL_DestroyTexture(TemptImage);

                score = convertInt(sc);
                text = loadText(renderer,score,"data-latin.ttf", 40);
                SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                renderTexture(text,renderer, 1000, 140,text1W,text1H);
                SDL_DestroyTexture(text);

                TemptImage = loadTexture("null.bmp", renderer);
                renderTexture(TemptImage, renderer, 980, 240, 1000, 60);
                SDL_DestroyTexture(TemptImage);


                showHScore = convertInt(HighScore);
                text = loadText(renderer,showHScore,"data-latin.ttf", 40);
                SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                renderTexture(text,renderer, 1000, 240,text1W,text1H);
                SDL_DestroyTexture(text);

                renderText(text, renderer, "Exit", 40, 900, 500);
                SDL_Event input;
                initiate(a,undo);
                renderGrid(a,TemptImage,renderer,position,NumberSquareH,NumberSquareW,thickOfFrame,distanceNumbers,sizeOfMatrix);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(text);
                //Main loop
                while (!endGame && !isGameOver && !isGameWon) {
                    //Get the event from the keyboard
                    bool keyPressed = false;
                    while (SDL_PollEvent(&input)) {
                        if (input.type == SDL_KEYDOWN) {
                            keyPressed = true;
                            break;
                        }
                        else if (input.type == SDL_MOUSEBUTTONDOWN) {
                            if (900 <= input.motion.x && input.motion.x<= 1030 && 500 <= input.motion.y && input.motion.y <= 530) {
                                Mix_HaltMusic();
                                endGame = true;
                                break;
                            }
                        }
                    }

                    //Process the game
                    if (keyPressed) {
                        if (sc > HighScore) {
                            TemptImage = loadTexture("null.bmp", renderer);
                            renderTexture(TemptImage, renderer, 980, 240, 1000, 60);
                            SDL_DestroyTexture(TemptImage);

                            HighScore = sc;
                            showHScore = convertInt(HighScore);
                            text = loadText(renderer,showHScore,"data-latin.ttf", 40);
                            SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                            renderTexture(text,renderer, 1000, 240,text1W,text1H);
                            SDL_DestroyTexture(text);
                        }

                        process(input,a,endGame);
                        if (isChanged(a,undo)) {
                            sc+=5;
                            TemptImage = loadTexture("null.bmp", renderer);
                            renderTexture(TemptImage, renderer, 980,140, 1000, 60);
                            SDL_DestroyTexture(TemptImage);

                            score = convertInt(sc);
                            text = loadText(renderer,score,"data-latin.ttf", 40);
                            SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                            renderTexture(text,renderer, 1000, 140,text1W,text1H);
                            SDL_DestroyTexture(text);
                        }
                        createRandom(a,undo);
                        renderGrid(a,TemptImage,renderer,position,NumberSquareH,NumberSquareW,thickOfFrame,distanceNumbers,sizeOfMatrix);
                        isGameOver = gameOver(a);
                        isGameWon = gameWon(a);
                        backUp(a,undo);
                    }
                }

                //Ending message
                if (isGameOver) {
                    Mix_HaltMusic();
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message","You Lost :(",NULL);
                }
                else if (isGameWon) {
                    Mix_HaltMusic();
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message","You Won :*",NULL);
                }

                if (sc >= HighScore) {
                    ofstream file1;
                    file1.open("HighScore.txt");
                    file1 << score;
                    file1.close();
                }
                SDL_DestroyTexture(background);
                SDL_DestroyTexture(TemptImage);
                SDL_DestroyTexture(text);
            }
            //Hard mode
            else if (200 <=e.motion.x && e.motion.x <= 648 && 403 <= e.motion.y && e.motion.y <= 429) {
                Mix_HaltMusic();
                PlayMusic(music,hardtheme,-1);
                //Set conditions
                endGame = false;
                isGameOver = false;
                isGameWon = false;

                //Set standards
                int setMoves = 22;
                int countMoves = 0;

                //Prepare for the game

                //Render background
                SDL_RenderClear(renderer);
                background = loadTexture("Background.bmp", renderer);
                renderTexture(background, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(background);

                renderText(text, renderer, "Your Score: ", 30, 800, 150);
                SDL_DestroyTexture(text);
                renderText(text, renderer, "High Score: ", 30, 800, 250);
                SDL_DestroyTexture(text);

                int sc = 0;
                string score;
                string showHScore;
                ifstream file;
                int text1W, text1H;
                file.open("HighScore2.txt");
                int HighScore;
                file >> HighScore;
                file.close();

                TemptImage = loadTexture("null.bmp", renderer);
                renderTexture(TemptImage, renderer, 980, 140, 1000, 60);
                SDL_DestroyTexture(TemptImage);

                score = convertInt(sc);
                text = loadText(renderer,score,"data-latin.ttf", 40);
                SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                renderTexture(text,renderer, 1000, 140,text1W,text1H);
                SDL_DestroyTexture(text);

                TemptImage = loadTexture("null.bmp", renderer);
                renderTexture(TemptImage, renderer, 980, 240, 1000, 60);
                SDL_DestroyTexture(TemptImage);


                showHScore = convertInt(HighScore);
                text = loadText(renderer,showHScore,"data-latin.ttf", 40);
                SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                renderTexture(text,renderer, 1000, 240,text1W,text1H);
                SDL_DestroyTexture(text);

                renderText(text, renderer, "Exit", 40, 900, 500);

                SDL_Event input;
                initiate(a,undo);
                renderGrid(a,TemptImage,renderer,position,NumberSquareH,NumberSquareW,thickOfFrame,distanceNumbers,sizeOfMatrix);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(text);
                //Main loop
                while (!endGame && !isGameOver && !isGameWon) {
                    //Get the event from the keyboard
                    bool keyPressed = false;
                    while (SDL_PollEvent(&input)) {
                        if (input.type == SDL_KEYDOWN) {
                            keyPressed = true;
                            break;
                        }
                        else if (input.type == SDL_MOUSEBUTTONDOWN) {
                            if (900 <= input.motion.x && input.motion.x <= 1030 && 500 <= input.motion.y && input.motion.y <= 530) {
                                Mix_HaltMusic();
                                endGame = true;
                                break;
                            }
                        }
                    }
                    //Process the game
                    if (keyPressed) {
                        if (countMoves == setMoves) {
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message","Everything will be mixed now !!",NULL);
                            mix(a,setMoves,countMoves);
                        }

                        if (sc > HighScore) {
                            TemptImage = loadTexture("null.bmp", renderer);
                            renderTexture(TemptImage, renderer, 980, 240, 1000, 60);
                            SDL_DestroyTexture(TemptImage);

                            HighScore = sc;
                            showHScore = convertInt(HighScore);
                            text = loadText(renderer,showHScore,"data-latin.ttf", 40);
                            SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                            renderTexture(text,renderer, 1000, 240,text1W,text1H);
                            SDL_DestroyTexture(text);
                        }

                        process(input,a,endGame);
                        if (isChanged(a,undo)) {
                            sc+=5;
                            TemptImage = loadTexture("null.bmp", renderer);
                            renderTexture(TemptImage, renderer, 980,140, 1000, 60);
                            SDL_DestroyTexture(TemptImage);

                            score = convertInt(sc);
                            text = loadText(renderer,score,"data-latin.ttf", 40);
                            SDL_QueryTexture(text, NULL, NULL, &text1W, &text1H);
                            renderTexture(text,renderer, 1000, 140,text1W,text1H);
                            SDL_DestroyTexture(text);
                        }
                        countMoves++;
                        createRandom(a,undo);
                        renderGrid(a,TemptImage,renderer,position,NumberSquareH,NumberSquareW,thickOfFrame,distanceNumbers,sizeOfMatrix);
                        isGameOver = gameOver(a);
                        isGameWon = gameWon(a);
                        backUp(a,undo);
                    }
                }
                if (isGameOver) {
                    Mix_HaltMusic();
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message","You Lost :(",NULL);
                }
                else if (isGameWon) {
                    Mix_HaltMusic();
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Message","You Won :*",NULL);
                }
                if (sc >= HighScore) {
                    ofstream file1;
                    file1.open("HighScore2.txt");
                    file1 << score;
                    file1.close();
                }
                SDL_DestroyTexture(background);
                SDL_DestroyTexture(TemptImage);
                SDL_DestroyTexture(text);
            }
            //Exit
            else if (200 <= e.motion.x && e.motion.x <=305 && 505 <= e.motion.y && e.motion.y <= 529) {
                isRun = false;
            }
        }
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(text);
        Mix_FreeMusic( music );
    }

    //Free texture
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(TemptImage);
	SDL_DestroyTexture(text);
	Mix_FreeMusic( music );
    Mix_Quit();
	//Free window and renderer
	quitSDL(window, renderer);


    return 0;
}
