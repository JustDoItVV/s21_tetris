#ifndef TETRIS_H
#define TETRIS_H

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define FIGURE_HEIGHT 4
#define FIGURE_WIDTH 4
#define PIXEL_EMPTY 0
#define PIXEL_FIGURE 1

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    START = 0,
    SPAWN,
    MOVING,
    SHIFTING,
    ATTACHING,
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
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct gameParams {
    GameInfo_t *data;
    GameState_t state;
    bool isActive;
    int figureRowIdx;
    int figureColIdx;
} GameParams_t;

void userInput(UserAction_t action, bool hold, GameParams_t *params);
GameInfo_t updateCurrentState(GameParams_t *params);

typedef void (*funcPointer)(GameParams_t *params);

void initializeParams(GameParams_t *params);
void removeParams(GameParams_t *params);

void startGame(GameParams_t *params);

void spawnNextFigure(GameParams_t *params);
// bool canRotate(GameParams_t *params);
// void rotateFigure(GameParams_t *params);
bool canMoveDown(GameParams_t *params);
void moveFigureLeft(GameParams_t *params);
void moveFigureRight(GameParams_t *params);
void moveFigureDown(GameParams_t *params);

int **allocate2DArray(int nRows, int nCols);
void generatePixel(int **figure);

#endif
