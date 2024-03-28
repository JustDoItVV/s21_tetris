#include "tetris.h"

funcPointer fsmTable[STATES_COUNT][SIGNALS_COUNT] = {
  {startGame, NULL, removeParams, NULL, NULL, NULL, NULL, NULL, NULL}, // Start
  {NULL, NULL, removeParams, moveLeft, moveRight, NULL, moveDown, rotate, NULL}, // Game
  {startGame, NULL, removeParams, NULL, NULL, NULL, NULL, NULL, NULL}, // Gameover
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
    next[figures[type][i - 1] + 1][figures[type][i] + 1] = type + 1;
  
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
  params->figure->rotation = 0;
  addFigure(params);
  params->figure->typeNext = generateRandomFigure(params->data->next);
}

void addFigure(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(M_PI_2 * rotation) + figures[type][i - 1] * sin(M_PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(M_PI_2 * rotation) + figures[type][i - 1] * cos(M_PI_2 * rotation));
    params->data->field[yy + y][xx + x] = type + 1;
  }
}

bool isFigureNotCollide(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  bool isNotCollide = true;
  for (int i = 1; i < 8 && isNotCollide; i += 2) {
    int xx = (int)round(figures[type][i] * cos(M_PI_2 * rotation) + figures[type][i - 1] * sin(M_PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(M_PI_2 * rotation) + figures[type][i - 1] * cos(M_PI_2 * rotation));
    if (params->data->field[yy + y][xx + x])
      isNotCollide = false;
  }

  return isNotCollide;
}

void clearFigure(GameParams_t *params) {
  int y = params->figure->y;
  int x = params->figure->x;
  int type = params->figure->type;
  int rotation = params->figure->rotation;

  for (int i = 1; i < 8; i += 2) {
    int xx = (int)round(figures[type][i] * cos(M_PI_2 * rotation) + figures[type][i - 1] * sin(M_PI_2 * rotation));
    int yy = (int)round(-figures[type][i] * sin(M_PI_2 * rotation) + figures[type][i - 1] * cos(M_PI_2 * rotation));
    params->data->field[yy + y][xx + x] = 0;
  }
}

void shift(GameParams_t *params) {
  clearFigure(params);
  params->figure->y++;
  bool canShift = isFigureNotCollide(params);
  
  if (!canShift)
    params->figure->y--;
  
  addFigure(params);

  if (!canShift)
    attach(params);
}

void attach(GameParams_t *params) {
  int rows = 0;
  for (int row = FIELD_HEIGHT - 4; row > 2; row--) {
    int rowBlocks;
    bool cycle = true;
    while (cycle) {
      rowBlocks = 0;
      for (int col = 3; col < FIELD_WIDTH - 3; col++)
        if (params->data->field[row][col])
          rowBlocks++;

      if (rowBlocks == FIELD_WIDTH - 6) {
        rows++;
        for (int i = row; i > 1; i--)
          for (int col = 3; col < FIELD_WIDTH - 3; col++)
            params->data->field[i][col] = params->data->field[i - 1][col];
      } else
        cycle = false;
    }
  }

  if (rows == 1)
    params->data->score += 100;
  else if (rows == 2)
    params->data->score += 300;
  else if (rows == 3)
    params->data->score += 700;
  else if (rows >= 4)
    params->data->score += 1500;
  
  params->data->level = params->data->score / 600 + 1;
  if (params->data->level > 10)
    params->data->level = 10;
  params->data->speed = params->data->score / 600 + 1;
  if (params->data->speed > 10)
    params->data->speed = 10;

  spawnNextFigure(params);
  clearFigure(params);
  
  params->figure->y++;
  bool canShift = isFigureNotCollide(params);
  
  if (!canShift)
    params->figure->y--;
  
  addFigure(params);

  if (!canShift) {
    params->state = GAMEOVER;
  }
}

void moveLeft(GameParams_t *params) {
  clearFigure(params);
  
  params->figure->x--;
  bool canMove = isFigureNotCollide(params);  
  
  if (!canMove) {
    params->figure->x++;
  }
  
  addFigure(params);
}

void moveRight(GameParams_t *params) {
  clearFigure(params);
  
  params->figure->x++;
  bool canMove = isFigureNotCollide(params);  
  
  if (!canMove) {
    params->figure->x--;
  }
  
  addFigure(params);
}

void moveDown(GameParams_t *params) {
  clearFigure(params);
  
  bool canMove = true;
  while (canMove) {
    params->figure->y++;
    canMove = isFigureNotCollide(params);
    
    if (!canMove) {
      params->figure->y--;
    }
  }
  
  addFigure(params);
  attach(params);
}

void rotate(GameParams_t *params) {
  clearFigure(params);

  params->figure->rotation++;
  if (params->figure->rotation > 3)
    params->figure->rotation = 0;

  bool canRotate = isFigureNotCollide(params);
  
  if (!canRotate)
    params->figure->rotation--;
  if (params->figure->rotation < 0)
    params->figure->rotation = 3;  
  
  addFigure(params);
}