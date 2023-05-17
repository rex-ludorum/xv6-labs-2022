#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[512];
  char *p, *oldP, *pAhead, *pEnd;

  char* newArgv[MAXARG];
  int i, j;

  for (i = 1; i < argc; i++) {
    newArgv[i - 1] = argv[i];
  }
  i--;

  int k = 0, tmp = 0;
  while ((tmp = read(0, buf + k, 512)) > 0) {
    k += tmp;
  }

  for (pEnd = buf; *pEnd != '\0'; pEnd++) ;

  pAhead = buf;
  p = buf;

  while (pAhead < pEnd) {
    j = i;
    for (; *pAhead == '\n' || *pAhead == ' '; pAhead++) ;
    
    for (; *pAhead != '\n' && *pAhead != ' '; pAhead++) {
      if (pAhead >= pEnd) exit(0);
    }
    while (p < pAhead) {
      for (; *p == ' ' || *p == '\n'; p++) ;
      oldP = p;
      for (; *p != ' ' && *p != '\n'; p++) ;
      newArgv[j++] = oldP;
      *p++ = '\0';
    }
    newArgv[j] = 0;
    if (fork() == 0) {
      exec(newArgv[0], newArgv);
    } else {
      wait(0);
    }
  }

  exit(0);
}
