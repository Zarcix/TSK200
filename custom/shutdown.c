#include <linux/reboot.h>
#include <sys/reboot.h>

int main() {
  reboot(LINUX_REBOOT_CMD_POWER_OFF);
  return 0;
}
