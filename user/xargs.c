#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[512], tmp;
  char *p, *oldP, *pAhead, *pEnd;

  char* newArgv[MAXARG];
  int i, j;

  for (i = 1; i < argc; i++) {
    newArgv[i - 1] = argv[i];
  }
  i--;

  int k = 0;
  while (read(0, &tmp, 512) > 0) {
    buf[k++] = tmp;
  }

  for (pEnd = buf; *pEnd != '\0'; pEnd++) ;

  pAhead = buf;
  oldP = buf;
  p = buf;
  int shouldExec;

  while (pAhead < pEnd) {
    shouldExec = 0;
    j = i;
    for (; *pAhead != '\n'; pAhead++) {
      if (pAhead > pEnd) exit(0);
    }
    while (p < pAhead) {
      for (; *p != ' ' && *p != '\n'; p++) ;
      newArgv[j++] = oldP;
      if (*p == '\n') {
        shouldExec = 1;
      }
      *p++ = '\0';
      oldP = p;
    }
    if (shouldExec) {
      newArgv[j] = 0;
      if (fork() == 0) {
        exec(newArgv[0], newArgv);
      } else {
        wait(0);
      }
    }
  }

  exit(0);
}
