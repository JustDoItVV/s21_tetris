/************************************************************
 * @mainpage
 * @author lucamorv
 * @version 1.0
 * @file tetris_cli.c
 * @brief Game logic  library
 ************************************************************/

#include "gui/cli/cli.h"

int main() {
  initGui();
  gameLoop();
  destroyGui();
  return 0;
}
