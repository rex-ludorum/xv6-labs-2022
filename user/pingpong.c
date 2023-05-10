#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);
  if (fork() == 0) {
    int byte;
    read(p2[0], &byte, 1);
    printf("%d: received ping\n", getpid());
    write(p1[1], &byte, 1);
    exit(0);
  }
  int byte = 5;
  write(p2[1], &byte, 1);
  read(p1[0], &byte, 1);
  printf("%d: received pong\n", getpid());

  exit(0);
}
