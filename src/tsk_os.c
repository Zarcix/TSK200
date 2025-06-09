#include <linux/reboot.h>
#include <ncurses.h>
#include <sys/reboot.h>

void tsk_shutdown() { reboot(LINUX_REBOOT_CMD_POWER_OFF); }

int main() {
  initscr();

  move(10, 20);
  printw("Starting TSK OS");

  refresh();

  char ch = getch();

  if (ch == 'q') {
    tsk_shutdown();
  }

  endwin();
  return 0;
}
