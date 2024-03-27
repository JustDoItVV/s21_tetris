#include "cli.h"

void initGui() {
  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, true);
  timeout(READ_DELAY);
}

void destroyGui() {
  printw("\nThe Game is ended. Closing application...\n");
  refresh();
  sleep(4);
  clear();
  refresh();
  endwin();
}

void gameLoop() {
  GameParams_t params;
  GameInfo_t data;
  Figure_t figure;
  params.data = &data;
  params.figure = &figure;
  UserAction_t action;
  int pressed_key;
  double counter = 0.;
  bool hold = false;

  initializeParams(&params);
  updateParams(&params);

  while (params.isActive) {
    pressed_key = getch();
    action = getAction(pressed_key);
    if (action != Action) {
      userInput(action, hold);
    }

    if (params.state != GAMEOVER && counter >= 1.50 - params.data->speed * SPEED_RATE) {
      if (params.state != START) {
        updateCurrentState();
      }
      counter = 0.;
    }

    counter = counter + READ_DELAY * 1E-3;

    if (params.state == START)
      drawStartScreen(params.data);
    else if (params.state != GAMEOVER) {
      drawGui();
      drawInfo(params.data);
      drawField(params.data->field);
    }
    printw("\nstate: %d\n", params.state);
    printw("\ncounter: %f\n", counter);
  }
}

UserAction_t getAction(int pressedKey) {
  UserAction_t action = Action;

  if (pressedKey == 10)
    action = Start;
  else if (pressedKey == ' ')
    action = Pause;
  else if (pressedKey == 27)
    action = Terminate;
  else if (pressedKey == KEY_LEFT)
    action = Left;
  else if (pressedKey == KEY_RIGHT)
    action = Right;
  else if (pressedKey == KEY_UP)
    action = Up;
  else if (pressedKey == KEY_DOWN)
    action = Down;
  
  return action;
}

void drawGui() {
  clear();
  
  mvhline(0, 0, ACS_HLINE, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvhline(FIELD_SIZE_Y + 1, 0, ACS_HLINE, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2);
  mvvline(1, 0, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + 1, ACS_VLINE, FIELD_SIZE_Y);
  mvvline(1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_VLINE, FIELD_SIZE_Y);

  mvaddch(0, 0, ACS_ULCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_URCORNER);
  mvaddch(FIELD_SIZE_Y + 1, 0, ACS_LLCORNER);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 2, ACS_LRCORNER);
  mvaddch(0, FIELD_SIZE_X * 2 + 1, ACS_TTEE);
  mvaddch(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + 1, ACS_BTEE);

  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawField(int **field) {
  for (int row = 0; row < FIELD_SIZE_Y; row++)
    for (int col = 0; col < FIELD_SIZE_X; col++)
      if (field[row + 3][col + 3]) {
        mvaddch(1 + row, 1 + col * 2, ACS_CKBOARD);
        mvaddch(1 + row, 1 + col * 2 + 1, ACS_CKBOARD);
      }
  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);      
}

void drawInfo(GameInfo_t *data) {
  mvprintw(2, FIELD_SIZE_X * 2 + 3, "HIGH SCORE: %d", data->high_score);
  mvprintw(4, FIELD_SIZE_X * 2 + 3, "SCORE: %d", data->score);
  mvprintw(6, FIELD_SIZE_X * 2 + 3, "LEVEL: %d", data->level);
  mvprintw(8, FIELD_SIZE_X * 2 + 3, "SPEED: %d", data->speed);
  mvprintw(10, FIELD_SIZE_X * 2 + 3, "NEXT");
  for (int row = 0; row < FIGURE_HEIGHT; row++)
    for (int col = 0; col < FIGURE_WIDTH; col++) {
      if (data->next[row][col]) {
        mvaddch(12 + row, FIELD_SIZE_X * 2 + 3 + col * 2, ACS_CKBOARD);
        mvaddch(12 + row, FIELD_SIZE_X * 2 + 3 + col * 2 + 1, ACS_CKBOARD);
      }
    }
  move(FIELD_SIZE_Y + 1, FIELD_SIZE_X * 2 + INFO_SIZE_X * 2 + 3);
}

void drawStartScreen(GameInfo_t *data) {
  drawGui();
  drawInfo(data);

  mvprintw(1 + FIELD_SIZE_Y / 2, 1, "Press ENTER to start");

  mvaddch(1 + FIELD_SIZE_Y - 3, 1 + FIELD_SIZE_X, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3, 1 + FIELD_SIZE_X + 1, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 1, 1 + FIELD_SIZE_X, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 1, 1 + FIELD_SIZE_X + 1, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 1, 1 + FIELD_SIZE_X + 1, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 1, 1 + FIELD_SIZE_X + 1 + 1, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 1, 1 + FIELD_SIZE_X + 1 + 1 + 1, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 2, 1 + FIELD_SIZE_X, ACS_CKBOARD);
  mvaddch(1 + FIELD_SIZE_Y - 3 + 2, 1 + FIELD_SIZE_X + 1, ACS_CKBOARD);

  move(1 + FIELD_SIZE_Y, 1 + FIELD_SIZE_X * 2 + 1 + INFO_SIZE_X * 2 + 1);
}
