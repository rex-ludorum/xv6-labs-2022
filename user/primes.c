#include "kernel/types.h"
#include "user/user.h"

void primes(int* buf) {
  int first = 1, firstPrime = 1;
  int prime, num;
  int p[2];
  while (read(buf[0], &num, 4) > 0) {
    if (first) {
      prime = num;
      printf("prime %d\n", prime);
      first = 0;
    } else if (num % prime != 0) {
      if (firstPrime) {
        pipe(p);
      }
      write(p[1], &num, 4);
      if (firstPrime) {
	if (fork() == 0) {
	  firstPrime = 0;
          primes(p);
        }
      }
    }
  }
  close(buf[0]);
  close(buf[1]);
  close(p[0]);
  close(p[1]);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  if (fork() == 0) {
    primes(p);
  } else {
    for (int i = 2; i < 36; i++) {
      write(p[1], &i, 4);
    }
  }
  close(p[0]);
  close(p[1]);

  exit(0);
}
