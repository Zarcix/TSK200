#include <ncurses.h>
#include <stdio.h>

int main() {
  initscr();

  move(10, 20);
  printw("Starting TSK OS");

  refresh();

  getch();

  endwin();
  return 0;
}
