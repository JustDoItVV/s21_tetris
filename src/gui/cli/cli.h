#ifndef GUI_CLI_H
#define GUI_CLI_H

#include <locale.h>
#include <ncurses.h>
#include <unistd.h>

#include "../../brick_game/tetris/tetris.h"

#define FRAME_RATE 60 // fps
#define READ_DELAY 50 // milliseconds
#define FIELD_SIZE_X 10
#define FIELD_SIZE_Y 20
#define INFO_SIZE_X 10
#define INFO_SIZE_Y 20
#define GUI_SCALE 1
#define SPEED_RATE 0.5

void initGui();
void destroyGui();
void gameLoop();

UserAction_t getAction(int pressedKey);

void drawGui();
void drawField(int **field);
void drawInfo(GameInfo_t *data);
void drawStartScreen();

#endif