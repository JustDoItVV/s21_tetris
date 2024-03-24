#include "gui/cli/cli.h"

int main() {
  initGui();
  gameLoop();
  destroyGui();
  return 0;
}
