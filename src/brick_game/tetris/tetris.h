#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M_PI_2 1.57079632679489661923

#define FIELD_HEIGHT 26
#define FIELD_WIDTH 16
#define FIGURE_HEIGHT 2
#define FIGURE_WIDTH 4
#define PIXEL_EMPTY 0
#define PIXEL_FIGURE 1

#define STATES_COUNT 6
#define SIGNALS_COUNT 9

typedef enum {
    START = 0,
    GAME,
    GAMEOVER,
} GameState_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
} UserAction_t;

/************************************************************
 * @brief Game data struct
 * 
 * Game logic data
 * 
 * @param field Game field with borders
 * @param next Next spafn figure for preview
 * @param score Game score
 * @param high_score Game high score from file
 * @param level Gamel level, max 10
 * @param speed Game speed, max 10
 * @param pause Pause flag
 ************************************************************/
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/************************************************************
 * @brief Figure data struct
 * 
 * Current and next figure parameters
 * 
 * @param typeNext Type of next figure
 * @param type Type of current figure
 * @param rotation Number of rotation to PI/2 angle: 0..3
 * @param x Field x coordinate of figure center
 * @param y Field y coordinate of figure center
 ************************************************************/
typedef struct {
  int typeNext;
  int type;
  int rotation;
  int x;
  int y;
} Figure_t;

/************************************************************
 * @brief Game parameters struct
 * 
 * All game parameters with current field, figure, state
 * 
 * @param data Game data with field
 * @param state Game current state
 * @param isActive Flag for activate game loop
 * @param figure Current figure data
 ************************************************************/
typedef struct {
    GameInfo_t *data;
    GameState_t state;
    bool isActive;
    Figure_t *figure;
} GameParams_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

typedef void (*funcPointer)(GameParams_t *params);

GameParams_t *updateParams(GameParams_t *params);
void initializeParams(GameParams_t *params);
void removeParams(GameParams_t *params);
void resetField(GameParams_t *params);

void startGame(GameParams_t *params);
int generateRandomFigure(int **next);
void spawnNextFigure(GameParams_t *params);
void moveLeft(GameParams_t *params);
void moveRight(GameParams_t *params);
void rotate(GameParams_t *params);
void moveDown(GameParams_t *params);
void shift(GameParams_t *params);
void attach(GameParams_t *params);

void addFigure(GameParams_t *params);
bool isFigureNotCollide(GameParams_t *params);
void clearFigure(GameParams_t *params);

int **allocate2DArray(int nRows, int nCols);

#endif
