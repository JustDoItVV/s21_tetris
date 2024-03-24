#include "tetris.h"

// Rows - states, cols - user input
funcPointer fsmTable[6][8] = {
  // Start
  {startGame, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
  // Spawn
  {NULL, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
  // Moving - user input processing
  {NULL, NULL, removeParams, moveFigureLeft, moveFigureRight, NULL, moveFigureDown, NULL},
  // Shifting - move current block 1 pixel down
  {NULL, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
  // Attaching
  {NULL, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
  // Gameover
  {NULL, NULL, removeParams, NULL, NULL, NULL, NULL, NULL},
};

void userInput(UserAction_t action, bool hold, GameParams_t *params) {
  if (hold)
    hold = hold;

  GameState_t state = params->state;
  funcPointer func = fsmTable[state][action];

  if (func)
    func(params);
}

GameInfo_t updateCurrentState(GameParams_t *params) {
  // static GameInfo_t data;
  if (canMoveDown(params))
    moveFigureDown(params);
  else
    spawnNextFigure(params);

  return *params->data;
}

void initializeParams(GameParams_t *params) {
  params->data->field = allocate2DArray(FIELD_HEIGHT, FIELD_WIDTH);
  params->data->next = allocate2DArray(FIGURE_HEIGHT, FIGURE_WIDTH);
  params->data->score = 0;
  params->data->high_score = 0;
  params->data->level = 1;
  params->data->speed = 1;
  params->data->pause = 1;
  // generateRandomFigure(gameInfo->next);
  generatePixel(params->data->next);
  params->state = START;
  params->isActive = true;
  params->figureRowIdx = 0;
  params->figureColIdx = FIELD_WIDTH / 2;
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

void generatePixel(int **figure) {
  figure[0][0] = 1;
}

void startGame(GameParams_t *params) {
  params->state = SPAWN;
  spawnNextFigure(params);
}

void spawnNextFigure(GameParams_t *params) {
  params->figureRowIdx = 0;
  params->figureColIdx = FIELD_WIDTH / 2;
  
  for (size_t figRowIdx = 0; figRowIdx < 1; figRowIdx++) {
    for (size_t figColIdx = 0; figColIdx < 1; figColIdx++) {
      int rowIdx = params->figureRowIdx;
      int colIdx = params->figureColIdx;
      params->data->field[rowIdx][colIdx] = params->data->next[figRowIdx][figColIdx];
    }
  }

  // generateRandomFigure(gameInfo->next);
  generatePixel(params->data->next);
  params->state = SHIFTING;
}

bool canMoveDown(GameParams_t *params) {
  bool response = true;

  for (int row = 0; row < 1; row++) {
    for (int col = 0; col < 1; col++) {
      if (params->data->next[row][col]) {
        if ((row + 1 + params->figureRowIdx) > (FIELD_HEIGHT - 1) || params->data->field[row + params->figureRowIdx + 1][col + params->figureColIdx]) {
          response = false;
        }
      }
    }
  }
  
  return response;
}

void moveFigureDown(GameParams_t *params) {
  if (canMoveDown(params)) {
    params->data->field[params->figureRowIdx][params->figureColIdx] = 0;
    params->figureRowIdx += 1;
    params->data->field[params->figureRowIdx][params->figureColIdx] = 1;
  }
  params->state = SHIFTING;
}

void moveFigureLeft(GameParams_t *params) {
  if (params->figureColIdx > 0) {
    params->data->field[params->figureRowIdx][params->figureColIdx] = 0;
    params->figureColIdx -= 1;
    params->data->field[params->figureRowIdx][params->figureColIdx] = 1;
  }
  params->state = SHIFTING;
}

void moveFigureRight(GameParams_t *params) {
  if (params->figureColIdx < FIELD_WIDTH - 1) {
    params->data->field[params->figureRowIdx][params->figureColIdx] = 0;
    params->figureColIdx += 1;
    params->data->field[params->figureRowIdx][params->figureColIdx] = 1;
  }
  params->state = SHIFTING;
}


// void rotateFigure(GameInfo_t *gameInfo) {
//   int temp[FIGURE_HEIGHT][FIGURE_WIDTH] = {0};
//   for (int i = 0; i < FIGURE_HEIGHT; ++i) {
//     for (int j = 0; j < FIGURE_WIDTH; ++j) {
//       temp[j][FIGURE_HEIGHT - 1 - i] = gameInfo->next[i][j];
//     }
//   }
//   for (int i = 0; i < FIGURE_HEIGHT; ++i) {
//     for (int j = 0; j < FIGURE_WIDTH; ++j) {
//       gameInfo->next[i][j] = temp[i][j];
//     }
//   }
// }

// bool canRotate(GameInfo_t *gameInfo) {
//   // Создаем временную копию для фигуры для проверки после вращения
//   int temp[FIGURE_HEIGHT][FIGURE_WIDTH] = {0};

//   // Предполагаемое вращение (заполнение временного массива)
//   for (int i = 0; i < FIGURE_HEIGHT; ++i) {
//     for (int j = 0; j < FIGURE_WIDTH; ++j) {
//       temp[i][j] = gameInfo->next[FIGURE_WIDTH - j - 1][i];
//     }
//   }

//   // Проверяем, не выходит ли вращенная фигура за границы поля или не
//   // сталкивается ли с другими фигурами
//   for (int row = 0; row < FIGURE_HEIGHT; ++row) {
//     for (int col = 0; col < FIGURE_WIDTH; ++col) {
//       if (temp[row][col]) {
//         int globalRow = row + gameInfo->figureRowIdx;
//         int globalCol = col + gameInfo->figureColIdx;

//         // Проверяем границы поля
//         if (globalCol < 0 || globalCol >= FIELD_WIDTH ||
//             globalRow >= FIELD_HEIGHT) {
//           return false; // Вращение приведет к выходу за границы поля
//         }

//         // Проверяем столкновения с уже уложенными фигурами
//         if (gameInfo->field[globalRow][globalCol]) {
//           return false; // Вращение приведет к столкновению
//         }
//       }
//     }
//   }

//   return true; // Вращение возможно
// }
