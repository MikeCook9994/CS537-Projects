#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

int
main(int argc, char *argv[])
{
	printf(1, "The number of processes currently running on this system: %d\n", getprocs());
  exit();
}
