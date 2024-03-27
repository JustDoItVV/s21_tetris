#include "tetris.h"

// Rows - states, cols - user input
funcPointer fsmTable[STATES_COUNT][SIGNALS_COUNT] = {
  // Start
  {startGame, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
  // Game
  {NULL, NULL, removeParams, moveLeft, moveRight, NULL, moveDown, NULL},
  // Gameover
  {startGame, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
};

void userInput(UserAction_t action, bool hold) {
  if (hold) hold = hold;
  GameParams_t *params = updateParams(NULL);
  GameState_t state = params->state;
  funcPointer func = fsmTable[state][action];

  if (func) func(params);
}

GameInfo_t updateCurrentState() {
  GameParams_t *params = updateParams(NULL);
  
  shift(params);

  return *params->data;
}

GameParams_t *updateParams(GameParams_t *params) {
  static GameParams_t *data;

  if (params != NULL)
    data = params;
  
  return data;
}

void initializeParams(GameParams_t *params) {
  params->data->field = allocate2DArray(FIELD_HEIGHT, FIELD_WIDTH);
  resetField(params);  
  params->data->next = allocate2DArray(FIGURE_HEIGHT, FIGURE_WIDTH);
  params->data->score = 0;
  params->data->high_score = 0;
  params->data->level = 1;
  params->data->speed = 1;
  params->data->pause = 1;
  params->figure->typeNext = generateRandomFigure(params->data->next);
  params->state = START;
  params->isActive = true;
}

void resetField(GameParams_t *params) {
  for (int row = 0; row < FIELD_HEIGHT; row++)
    for (int col = 0; col < FIELD_WIDTH; col++)
      params->data->field[row][col] = (row > 22 || col < 3 || col > 12) ? 1 : 0;
}

void removeParams(GameParams_t *params) {
  if (params->data->field) {
    for (size_t rowIdx = 0; rowIdx < FIELD_HEIGHT; rowIdx++) {
      free(params->data->field[rowIdx]);
      params->data->field[rowIdx] = NULL;
    }
    free(params->data->field);
    params->data->field = NULL;
  }

  if (params->data->next) {
    for (size_t rowIdx = 0; rowIdx < FIGURE_HEIGHT; rowIdx++) {
      free(params->data->next[rowIdx]);
      params->data->next[rowIdx] = NULL;
    }
    free(params->data->next);
    params->data->next = NULL;
  }
  
  params->state = GAMEOVER;
  params->isActive = false;
}

int **allocate2DArray(int nRows, int nCols) {
  int **arr = (int **)calloc(nRows, sizeof(int *));
  for (size_t rowIdx = 0; rowIdx < (size_t)nRows; rowIdx++) {
    arr[rowIdx] = (int *)calloc(nCols, sizeof(int));
  }
  return arr;
}

int figures[7][8] = {
  {0, -1, 0, 0, 0, 1, 0, 2},
  {-1, -1, 0, -1, 0, 0, 0, 1},
  {0, -1, 0, 0, 0, 1, -1, 1},
  {-1, 0, -1, 1, 0, 0, 0, 1},
  {0, -1, 0, 0, -1, 0, -1, 1},
  {0, -1, 0, 0, -1, 0, 0, 1},
  {-1, -1, -1, 0, 0, 0, 0, 1},
};

int generateRandomFigure(int **next) {
  srand(time(NULL));
  int type = rand() % 7;

  for (int row = 0; row < FIGURE_HEIGHT; row++)
    for (int col = 0; col < FIGURE_WIDTH; col++)
      next[row][col] = 0;
  
  for (int i = 1; i < 8; i += 2)
    next[figures[type][i - 1] + 1][figures[type][i] + 1] = 1;
  
  return type;
}

void startGame(GameParams_t *params) {
  resetField(params);
  params->data->score = 0;
  params->data->speed = 1;
  params->data->level = 1;
  params->state = GAME;
  spawnNextFigure(params);
}

void spawnNextFigure(GameParams_t *params) {
  int y = 2;
  int x = FIELD_WIDTH / 2;
  int type = params->figure->typeNext;
  params->figure->type = type;
  params->figure->x = x;
  params->figure->y = y;
  
  for (int i = 1; i < 8; i += 2) {
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  }

  params->figure->typeNext = generateRandomFigure(params->data->next);
}

void shift(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;

  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 0;
  
  y++;
  bool canShift = true;
  for (int i = 1; i < 8 && canShift; i += 2)
    if (params->data->field[figures[type][i - 1] + y][figures[type][i] + x])
      canShift = false;
  
  if (!canShift)
    y--;
  
  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  
  params->figure->y = y;

  if (!canShift)
    attach(params);
}

void attach(GameParams_t *params) {
  spawnNextFigure(params);
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;

  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 0;
  
  y++;
  bool canShift = true;
  for (int i = 1; i < 8 && canShift; i += 2)
    if (params->data->field[figures[type][i - 1] + y][figures[type][i] + x])
      canShift = false;
  
  if (!canShift)
    y--;
  
  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  
  params->figure->y = y;

  if (!canShift) {
    params->state = GAMEOVER;
  }
}

void moveLeft(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;

  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 0;
  
  x--;
  bool canMove = true;
  for (int i = 1; i < 8 && canMove; i += 2)
    if (params->data->field[figures[type][i - 1] + y][figures[type][i] + x])
      canMove = false;
  
  if (!canMove) {
    x++;
  }
  
  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  
  params->figure->x = x;
}

void moveRight(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;

  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 0;
  
  x++;
  bool canMove = true;
  for (int i = 1; i < 8 && canMove; i += 2)
    if (params->data->field[figures[type][i - 1] + y][figures[type][i] + x])
      canMove = false;
  
  if (!canMove) {
    x--;
  }
  
  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  
  params->figure->x = x;
}

void moveDown(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;

  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 0;
  
  bool canMove = true;
  while (canMove) {
    y++;
  
    for (int i = 1; i < 8 && canMove; i += 2)
      if (params->data->field[figures[type][i - 1] + y][figures[type][i] + x])
        canMove = false;
    
    if (!canMove) {
      y--;
    }
  }
  
  for (int i = 1; i < 8; i += 2)
    params->data->field[figures[type][i - 1] + y][figures[type][i] + x] = 1;
  
  params->figure->y = y;
  attach(params);
}